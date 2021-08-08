#pragma once 

#define GLFW_INCLUDE_VULKAN
#include <kent/renderer/renderer.h>



#include <vulkan/vulkan.h>
#include <vector>

namespace Kent {

    class VulkanRenderer : public Renderer {
    public:
        virtual void Init(RendererSettings settings, GLFWwindow* window, int width, int height) override;
        virtual void Shutdown() override;
        virtual void RenderFrame() override;

        virtual void DrawSurface(std::unordered_map<SurfaceArgs, std::any> args, GLFWwindow* window) override;

    private:
        void InitCore(GLFWwindow* window);
        void CreateSwapchain();
        void CreateCommands();
        void CreateDefaultRenderPass();
        void CreateFramebuffer();
        void CreateSyncStructures();
        void CreatePipelines();

    private:
        uint64_t m_FrameNumber {0};

        VkInstance m_Instance;
        VkDebugUtilsMessengerEXT m_DebugMessenger;
        VkPhysicalDevice m_PhysicalDevice;  // physical device
        VkDevice m_Device;  // logical device
        VkSurfaceKHR m_Surface;

        RendererSettings m_RendererSettings = {};

        VkSwapchainKHR m_Swapchain;
        VkFormat m_SwapchainImageFormat;

        std::vector<VkImage> m_SwapchainImages;
        std::vector<VkImageView> m_SwapchainImageViews;
        VkExtent2D m_WindowExtent;

        VkQueue m_GraphicsQueue;
        uint32_t m_GraphicsQueueFamily;

        VkCommandPool m_CommandPool;
        VkCommandBuffer m_MainCommandBuffer;

        VkRenderPass m_RenderPass;
        std::vector<VkFramebuffer> m_Framebuffer { 3 };

        VkSemaphore m_PresentSemaphore, m_RenderSemaphore;
        VkFence m_RenderFence;

        VkPipelineLayout m_TrianglePipelineLayout;
        VkPipeline m_TrianglePipeline;
    };

}

