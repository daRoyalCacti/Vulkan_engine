//
// Created by jacob on 17/11/21.
//

#ifndef VULKAN_ENGINE_COLOR_BLEND_HPP
#define VULKAN_ENGINE_COLOR_BLEND_HPP

#include <vulkan/vulkan.h>

/* Classes that determine how the value from the fragment shader should be written to the framebuffer
    Then blending is equivalent to the following pseudo code
    |    if (blendEnable) {
    |        finalColor.rgb = (srcColorBlendFactor * newColor.rgb) <colorBlendOp> (dstColorBlendFactor * oldColor.rgb);
    |        finalColor.a = (srcAlphaBlendFactor * newColor.a) <alphaBlendOp> (dstAlphaBlendFactor * oldColor.a);
    |    } else {
    |        finalColor = newColor;
    |    }
    |    finalColor = finalColor & colorWriteMask;
    The ColorBlendAttachmentState struct determines the mixing and ColorBlendState combines values using binary operations
    ColorBlendAttachmentState is per frame buffer whereas ColorBlendState is global
    Enabling ColorBlendState automatically disables ColorBlendAttachmentState
 */

struct ColorBlendAttachmentState{
    //https://khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkPipelineColorBlendAttachmentState.html
    VkPipelineColorBlendAttachmentState colorBlendAttachment{};

    ColorBlendAttachmentState(const VkBool32 blend, const VkBlendFactor source_blend_factor, const VkBlendFactor destination_blend_factor, const VkBlendOp color_blend_operation,
                              const VkBlendFactor source_alpha_blend_factor, const VkBlendFactor destination_alpha_blend_factor, const VkBlendOp alpha_blend_operation) {
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;    //which component should be written
        //see the pseudo-code above for how these work
        // - for the VkBlendFactors possible values see https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkBlendFactor.html
        // - for the VkBlendOps possible values see https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkBlendOp.html
        colorBlendAttachment.blendEnable = blend;
        colorBlendAttachment.srcColorBlendFactor = source_blend_factor;
        colorBlendAttachment.dstColorBlendFactor = destination_blend_factor;
        colorBlendAttachment.colorBlendOp = color_blend_operation;
        colorBlendAttachment.srcAlphaBlendFactor = source_alpha_blend_factor;
        colorBlendAttachment.dstAlphaBlendFactor = destination_alpha_blend_factor;
        colorBlendAttachment.alphaBlendOp = alpha_blend_operation;
    }

    [[nodiscard]] VkPipelineColorBlendAttachmentState& get_pipeline_stage() {return colorBlendAttachment;}
};


struct ColorBlendState{
    VkPipelineColorBlendStateCreateInfo colorBlending{};

    explicit ColorBlendState(ColorBlendAttachmentState& attachmentState) {
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;     //to do any logical operation to the output
        colorBlending.logicOp = VK_LOGIC_OP_COPY;   // a valid logicalOp (see the pseudo-code above)
                                                    // - https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkLogicOp.html
        colorBlending.attachmentCount = 1;          //the number of attachments (remember attachments are per framebuffer)
        colorBlending.pAttachments = &attachmentState.get_pipeline_stage(); //the array of attachments (only 1 framebuffer so this is just a reference)
        //the component of the blend constant that are used in blending
        colorBlending.blendConstants[0] = 0.0f;
        colorBlending.blendConstants[1] = 0.0f;
        colorBlending.blendConstants[2] = 0.0f;
        colorBlending.blendConstants[3] = 0.0f;
    }

    [[nodiscard]] VkPipelineColorBlendStateCreateInfo& get_pipeline_stage() {return colorBlending;}
};

#endif //VULKAN_ENGINE_COLOR_BLEND_HPP
