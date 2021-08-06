#include "vulkan_renderer.h"

#include <VkBootstrap.h>
#include "vulkan_initializer.h"
#include "vulkan_types.h"
#include "vulkan_utilities.h"

#include <iostream>

namespace Kent
{

    Scope<Renderer> Renderer::CreateRenderer()
    {
        return CreateScope<VulkanRenderer>();
    }

    void VulkanRenderer::Init(RendererSettings settings, GLFWwindow* window)
    {
        m_RendererSettings = settings;
        InitCore(window);
    }

    void VulkanRenderer::Shutdown()
    {
    }

    void VulkanRenderer::RenderFrame()
    {
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
    }
    
    void VulkanRenderer::CreateSwapchain(int width, int height) 
    {
        vkb::SwapchainBuilder swapchainBuilder { m_PhysicalDevice, m_Device, m_Surface };
        vkb::Swapchain vkbSwapchain = swapchainBuilder
        .use_default_format_selection()
        .set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
        .set_desired_extent(width, height)
        .build()
        .value();

        m_Swapchain = vkbSwapchain.swapchain;
        m_SwapchainImages = vkbSwapchain.get_images().value();
        m_SwapchainImageViews = vkbSwapchain.get_image_views().value();
        m_SwapchainImageFormat = vkbSwapchain.image_format;
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

}
