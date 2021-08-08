#include "vulkan_pipeline_builder.h"
#include <iostream>

namespace Kent {

    VkPipeline VulkanPipelineBuilder::BuildPipeline(VkDevice device, VkRenderPass pass) 
    {
        VkPipelineViewportStateCreateInfo viewportState { VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO };
        viewportState.viewportCount = 1;
        viewportState.pViewports = &m_Viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = & m_Scissor;

        VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo { VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO };
        colorBlendStateCreateInfo.logicOpEnable = VK_FALSE;
        colorBlendStateCreateInfo.logicOp = VK_LOGIC_OP_COPY;
        colorBlendStateCreateInfo.attachmentCount = 1;
        colorBlendStateCreateInfo.pAttachments = &m_ColorBlendAttachment;

        VkGraphicsPipelineCreateInfo pipelineCreateInfo { VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO };
        pipelineCreateInfo.stageCount = m_ShaderStages.size();
        pipelineCreateInfo.pStages = m_ShaderStages.data();
        pipelineCreateInfo.pVertexInputState = &m_VertexInputInfo;
        pipelineCreateInfo.pInputAssemblyState = &m_InputAssembly;
        pipelineCreateInfo.pViewportState = &viewportState;
        pipelineCreateInfo.pRasterizationState = &m_Rasterizer;
        pipelineCreateInfo.pMultisampleState = &m_Multisampling;
        pipelineCreateInfo.pColorBlendState = &colorBlendStateCreateInfo;
        pipelineCreateInfo.layout = m_PipelineLayout;
        pipelineCreateInfo.renderPass = pass;
        pipelineCreateInfo.subpass = 0;
        pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;

        VkPipeline newPipeline;

        if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &newPipeline) != VK_SUCCESS) {
            std::cout << "Failed to create pipeline\n";
            return VK_NULL_HANDLE;
        }

        return newPipeline;
    }

}