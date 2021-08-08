#pragma once

#include <vulkan/vulkan.h>

namespace Kent {

    class VulkanInitializer {
    public:
        static VkCommandPoolCreateInfo CommandPoolCreateInfo(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flag = 0);
        static VkCommandBufferAllocateInfo CommandBufferAllocateInfo(VkCommandPool pool, uint32_t count = 1, VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);
    

        static VkPipelineShaderStageCreateInfo PipelineShaderStageCreateInfo(VkShaderStageFlagBits stage, VkShaderModule shaderModule);
        static VkPipelineVertexInputStateCreateInfo PipelineVertexInputStateCreateInfo();
        static VkPipelineInputAssemblyStateCreateInfo PipelineInputAssemblyStateCreateInfo(VkPrimitiveTopology topology);
        static VkPipelineRasterizationStateCreateInfo PipelineRasterizationStateCreateInfo(VkPolygonMode polygonMode);
        static VkPipelineMultisampleStateCreateInfo PipelineMultisampleStateCreateInfo();
        static VkPipelineColorBlendAttachmentState PipelineColorBlendAttachmentState();

        static VkPipelineLayoutCreateInfo PipelineLayoutCreateInfo();
    };

}
