#pragma once

#include <vulkan/vulkan.h>
#include <string>

#define VK_CHECK(x) \
    do { \
        VkResult err = x; \
        if(err) { \
            std::cout << "Detected Vulkan error: " << err << std::endl; \
            abort(); \
        } \
    } while(0) \

namespace Kent {

    class VulkanUtilities
    {
    public:
        static bool LoadShaderModule(const std::string& filePath, VkDevice device, VkShaderModule &outShaderModule);
    };

}
