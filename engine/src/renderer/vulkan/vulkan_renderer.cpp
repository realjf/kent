#include "vulkan_renderer.h"

#include <VkBootstrap.h>
#include "vulkan_initializer.h"
#include "vulkan_types.h"
#include "vulkan_utilities.h"
#include "vulkan_pipeline_builder.h"

#include <iostream>

namespace Kent
{

    Scope<Renderer> Renderer::CreateRenderer()
    {
        return CreateScope<VulkanRenderer>();
    }

    void VulkanRenderer::Init(RendererSettings settings, GLFWwindow* window, int width, int height)
    {
        m_RendererSettings = settings;
        m_WindowExtent.width = width;
        m_WindowExtent.height = height;

        InitCore(window);
        CreateSwapchain();
        CreateCommands();
        CreateDefaultRenderPass();
        CreateFramebuffer();
        CreateSyncStructures();
        CreatePipelines();
    }

    void VulkanRenderer::Shutdown()
    {
        vkDeviceWaitIdle(m_Device);

        vkDestroyPipeline(m_Device, m_TrianglePipeline, nullptr);
        vkDestroyPipelineLayout(m_Device, m_TrianglePipelineLayout, nullptr);

        vkDestroyFence(m_Device, m_RenderFence, nullptr);
        vkDestroySemaphore(m_Device, m_PresentSemaphore, nullptr);
        vkDestroySemaphore(m_Device, m_RenderSemaphore, nullptr);

        for(auto framebuffer : m_Framebuffer)
        {
            vkDestroyFramebuffer(m_Device, framebuffer, nullptr);
        }

        vkDestroyRenderPass(m_Device, m_RenderPass, nullptr);

        vkDestroyCommandPool(m_Device, m_CommandPool, nullptr);
        vkDestroySwapchainKHR(m_Device, m_Swapchain, nullptr);

        for(auto imageView : m_SwapchainImageViews)
        {
            vkDestroyImageView(m_Device, imageView, nullptr);
        }

        vkDestroyDevice(m_Device, nullptr);
        vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
        vkb::destroy_debug_utils_messenger(m_Instance, m_DebugMessenger);
        vkDestroyInstance(m_Instance, nullptr);
    }

    void VulkanRenderer::RenderFrame()
    {
        VK_CHECK(vkWaitForFences(m_Device, 1, &m_RenderFence, true, 1000000000));
        VK_CHECK(vkResetFences(m_Device, 1, &m_RenderFence));

        uint32_t swapchainImageIndex;
        VK_CHECK(vkAcquireNextImageKHR(m_Device, m_Swapchain, 100000000, m_PresentSemaphore, nullptr, &swapchainImageIndex));

        VK_CHECK(vkResetCommandBuffer(m_MainCommandBuffer, 0));

        VkCommandBuffer cmd = m_MainCommandBuffer;
        VkCommandBufferBeginInfo beginInfo { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        VK_CHECK(vkBeginCommandBuffer(cmd, &beginInfo));

        float flashColor = abs(sin((float)m_FrameNumber / 120.0f));
        VkClearValue clearValue { 
            .color = { 0.0f, flashColor, flashColor, 1.0f },
        };

        VkRenderPassBeginInfo renderPassBeginInfo { VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO };
        renderPassBeginInfo.renderPass = m_RenderPass;
        renderPassBeginInfo.renderArea = {
            .offset = { 
                .x = 0,
                .y = 0
            },
            .extent = m_WindowExtent
        };

        renderPassBeginInfo.framebuffer = m_Framebuffer[swapchainImageIndex];
        renderPassBeginInfo.clearValueCount = 1;
        renderPassBeginInfo.pClearValues = &clearValue;

        vkCmdBeginRenderPass(cmd, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        // Draw calls
        vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, m_TrianglePipeline);
        vkCmdDraw(cmd, 3, 1, 0, 0);


        vkCmdEndRenderPass(cmd);
        VK_CHECK(vkEndCommandBuffer(cmd));

        VkSubmitInfo submit { VK_STRUCTURE_TYPE_SUBMIT_INFO };

        VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        submit.pWaitDstStageMask = &waitStage;
        submit.waitSemaphoreCount = 1;
        submit.pWaitSemaphores = &m_PresentSemaphore;
        submit.signalSemaphoreCount = 1;
        submit.pSignalSemaphores = &m_RenderSemaphore;
        submit.commandBufferCount = 1;
        submit.pCommandBuffers = &m_MainCommandBuffer;

        VK_CHECK(vkQueueSubmit(m_GraphicsQueue, 1, &submit, m_RenderFence));

        VkPresentInfoKHR presentInfoKhr { VK_STRUCTURE_TYPE_PRESENT_INFO_KHR};
        presentInfoKhr.swapchainCount = 1;
        presentInfoKhr.pSwapchains = &m_Swapchain;

        presentInfoKhr.waitSemaphoreCount = 1;
        presentInfoKhr.pWaitSemaphores = &m_RenderSemaphore;
        presentInfoKhr.pImageIndices = &swapchainImageIndex;

        VK_CHECK(vkQueuePresentKHR(m_GraphicsQueue, &presentInfoKhr));
        m_FrameNumber++;
    }

    void VulkanRenderer::DrawSurface(std::unordered_map<SurfaceArgs, std::any> args, GLFWwindow* window)
    {
        try
        {
            auto vkInstance = std::any_cast<VkInstance>(args[SurfaceArgs::INSTANCE]);
            auto *allocationCallbacks = args[SurfaceArgs::ALLOCATORS].has_value() ? std::any_cast<VkAllocationCallbacks *>(args[SurfaceArgs::ALLOCATORS]) : nullptr;
            auto *outSurface = std::any_cast<VkSurfaceKHR *>(args[SurfaceArgs::OUT_SURFACE]);

            if(vkInstance == VK_NULL_HANDLE)
            {
                throw std::runtime_error("Must provide a instance");
            }
  
            if (glfwCreateWindowSurface(vkInstance, window, allocationCallbacks, outSurface) != VK_SUCCESS)
            {
                throw std::runtime_error("Failed to create window surface");
            }
        }
        catch (std::bad_any_cast& e)
        {
            std::cout << "Failed to cast window surface arguments: " << e.what() << std::endl;
        }
    }

    void VulkanRenderer::InitCore(GLFWwindow* window)
    {
        // init the instance
        vkb::InstanceBuilder builder;

        auto builderInstance = builder.set_app_name(m_RendererSettings.ApplicationName.c_str())
                                   .request_validation_layers(true)
                                   .require_api_version(1, 1, 0)
                                   .use_default_debug_messenger()
                                   .build();

        vkb::Instance vkbInst = builderInstance.value();
        m_Instance = vkbInst.instance;
        m_DebugMessenger = vkbInst.debug_messenger;

        // vulkan surface implement in windowswindow.h DrawSurface()
        std::unordered_map<SurfaceArgs, std::any> surfaceArgs {
            { SurfaceArgs::INSTANCE, m_Instance },
            // { SurfaceArgs::ALLOCATORS, nullptr },
            { SurfaceArgs::OUT_SURFACE, &m_Surface },
        };
        DrawSurface(surfaceArgs, window);

        // select physical device
        vkb::PhysicalDeviceSelector selector{vkbInst};
        vkb::PhysicalDevice vkbPhysicalDevice{
            selector.set_minimum_version(1, 1)
                .set_surface(m_Surface)
                .select()
                .value()};

        // create the vulkan device
        vkb::DeviceBuilder deviceBuilder{vkbPhysicalDevice};
        vkb::Device vkbDevice{deviceBuilder.build().value()};

        m_Device = vkbDevice.device;
        m_PhysicalDevice = vkbPhysicalDevice.physical_device;

        m_GraphicsQueue = vkbDevice.get_queue(vkb::QueueType::graphics).value();
        m_GraphicsQueueFamily = vkbDevice.get_queue_index(vkb::QueueType::graphics).value();
    }
    
    void VulkanRenderer::CreateSwapchain() 
    {
        vkb::SwapchainBuilder swapchainBuilder { m_PhysicalDevice, m_Device, m_Surface };
        vkb::Swapchain vkbSwapchain = swapchainBuilder
        .use_default_format_selection()
        .set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
        .set_desired_extent(m_WindowExtent.width, m_WindowExtent.height)
        .build()
        .value();

        m_Swapchain = vkbSwapchain.swapchain;
        m_SwapchainImages = vkbSwapchain.get_images().value();
        m_SwapchainImageViews = vkbSwapchain.get_image_views().value();
        m_SwapchainImageFormat = vkbSwapchain.image_format;
    }
    
    void VulkanRenderer::CreateCommands() 
    {
        VkCommandPoolCreateInfo commandPoolCreateInfo = VulkanInitializer::CommandPoolCreateInfo(
            m_GraphicsQueueFamily, 
            VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO);
        VK_CHECK(vkCreateCommandPool(m_Device, &commandPoolCreateInfo, nullptr, &m_CommandPool));

        VkCommandBufferAllocateInfo commandBufferAllocateInfo = VulkanInitializer::CommandBufferAllocateInfo(m_CommandPool);
        VK_CHECK(vkAllocateCommandBuffers(m_Device, &commandBufferAllocateInfo, &m_MainCommandBuffer));
    }
    
    void VulkanRenderer::CreateDefaultRenderPass() 
    {
        VkAttachmentDescription colorAttachment{
            .format = m_SwapchainImageFormat,
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
            .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
        };

        VkAttachmentReference colorAttachmentRef { 
            .attachment = 0,
            .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        };

        VkSubpassDescription subpass {
            .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
            .colorAttachmentCount = 1,
            .pColorAttachments = &colorAttachmentRef,
        };


        VkRenderPassCreateInfo renderPassCreateInfo { VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO };
        renderPassCreateInfo.attachmentCount = 1;
        renderPassCreateInfo.pAttachments = &colorAttachment;
        renderPassCreateInfo.subpassCount = 1;
        renderPassCreateInfo.pSubpasses = &subpass;

        VK_CHECK(vkCreateRenderPass(m_Device, &renderPassCreateInfo, nullptr, &m_RenderPass));
    }
    
    void VulkanRenderer::CreateFramebuffer() 
    {
        VkFramebufferCreateInfo framebufferCreateInfo { VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO };
        framebufferCreateInfo.renderPass = m_RenderPass;
        framebufferCreateInfo.attachmentCount = 1;
        framebufferCreateInfo.width = m_WindowExtent.width;
        framebufferCreateInfo.height = m_WindowExtent.height;
        framebufferCreateInfo.layers = 1;

        const uint32_t swapchainImageCount = m_SwapchainImages.size();
        m_Framebuffer.resize(swapchainImageCount);

        for(int i = 0; i < swapchainImageCount; i++)
        {
            framebufferCreateInfo.pAttachments = &m_SwapchainImageViews[i];
            VK_CHECK(vkCreateFramebuffer(m_Device, &framebufferCreateInfo, nullptr, &m_Framebuffer[i]));
        }
    }
    
    void VulkanRenderer::CreateSyncStructures() 
    {
        VkFenceCreateInfo fenceCreateInfo { VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };
        fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        VK_CHECK(vkCreateFence(m_Device, &fenceCreateInfo, nullptr, &m_RenderFence));

        VkSemaphoreCreateInfo semaphoreCreateInfo { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
        VK_CHECK(vkCreateSemaphore(m_Device, &semaphoreCreateInfo, nullptr, &m_PresentSemaphore));
        VK_CHECK(vkCreateSemaphore(m_Device, &semaphoreCreateInfo, nullptr, &m_RenderSemaphore));
    }
    
    void VulkanRenderer::CreatePipelines() 
    {
        VkShaderModule triangleFragShader;
        if(!VulkanUtilities::LoadShaderModule("../shaders/triangle.frag.spv", m_Device, triangleFragShader))
        {
            std::cout << "Failed to load triangle fragment shader module\n";
        }else{
            std::cout << "Successfully loaded triangle fragment shader module\n";
        }

        VkShaderModule triangleVertShader;
        if(!VulkanUtilities::LoadShaderModule("../shaders/triangle.vert.spv", m_Device, triangleVertShader))
        {
            std::cout << "Failed to load triangle vertex shader module\n";
        }else{
            std::cout << "Successfully loaded triangle vertex shader module\n";
        }

        auto pipelineLayoutInfo = VulkanInitializer::PipelineLayoutCreateInfo();
        VK_CHECK(vkCreatePipelineLayout(m_Device, &pipelineLayoutInfo, nullptr, &m_TrianglePipelineLayout));

        /*
         * TEMPORARY PIPELINE BUILDING
         */

        VulkanPipelineBuilder pipelineBuilder;
        pipelineBuilder.m_ShaderStages.push_back(
                VulkanInitializer::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_VERTEX_BIT, triangleVertShader));
        pipelineBuilder.m_ShaderStages.push_back(
                VulkanInitializer::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT, triangleFragShader));

        pipelineBuilder.m_VertexInputInfo = VulkanInitializer::PipelineVertexInputStateCreateInfo();
        pipelineBuilder.m_InputAssembly = VulkanInitializer::PipelineInputAssemblyStateCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

        // build the viewport
        pipelineBuilder.m_Viewport = {
                .x = 0.f,
                .y = 0.f,
                .width = static_cast<float>(m_WindowExtent.width),
                .height = static_cast<float>(m_WindowExtent.height),
                .minDepth = 0.f,
                .maxDepth = 1.f
        };

        pipelineBuilder.m_Scissor = {
                .offset = {0 , 0},
                .extent = m_WindowExtent
        };

        pipelineBuilder.m_Rasterizer = VulkanInitializer::PipelineRasterizationStateCreateInfo(VK_POLYGON_MODE_FILL);
        pipelineBuilder.m_Multisampling = VulkanInitializer::PipelineMultisampleStateCreateInfo();
        pipelineBuilder.m_ColorBlendAttachment = VulkanInitializer::PipelineColorBlendAttachmentState();
        pipelineBuilder.m_PipelineLayout = m_TrianglePipelineLayout;

        m_TrianglePipeline = pipelineBuilder.BuildPipeline(m_Device, m_RenderPass);

        vkDestroyShaderModule(m_Device, triangleFragShader, nullptr);
        vkDestroyShaderModule(m_Device, triangleVertShader, nullptr);
    }

}
