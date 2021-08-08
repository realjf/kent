#include "vulkan_utilities.h"

#include <fstream>
#include <vector>


namespace Kent {
    bool VulkanUtilities::LoadShaderModule(const std::string& filePath, VkDevice device, VkShaderModule &outShaderModule)
    {
        std::ifstream file(filePath.c_str(), std::ios::ate | std::ios::binary);

        if(!file.is_open())
        {
            return false;
        }

        size_t filesize = static_cast<size_t>(file.tellg());
        std::vector<uint32_t> buffer(filesize/ sizeof(uint32_t));

        file.seekg(0);
        file.read((char *)buffer.data(), filesize);
        file.close();

        VkShaderModuleCreateInfo shaderModuleCreateInfo { VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO };
        shaderModuleCreateInfo.codeSize = buffer.size() * sizeof(uint32_t);
        shaderModuleCreateInfo.pCode = buffer.data();

        VkShaderModule shaderModule;
        if(vkCreateShaderModule(device, &shaderModuleCreateInfo, nullptr, &shaderModule) != VK_SUCCESS)
        {
            return false;
        }
        outShaderModule = shaderModule;
        return true;
    }
    
}

