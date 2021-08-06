#pragma once 

#define GLFW_INCLUDE_VULKAN
#include <kent/renderer/renderer.h>



#include <vulkan/vulkan.h>
#include <vector>

namespace Kent {

    class VulkanRenderer : public Renderer {
    public:
        virtual void Init(RendererSettings settings, GLFWwindow* window) override;
        virtual void Shutdown() override;
        virtual void RenderFrame() override;

        virtual void CreateSwapchain(int width, int height) override;

        virtual void DrawSurface(std::unordered_map<SurfaceArgs, std::any> args, GLFWwindow* window) override;

    private:
        void InitCore(GLFWwindow* window);
        

    private:
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

    };

}

