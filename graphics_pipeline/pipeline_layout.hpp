//
// Created by jacob on 17/11/21.
//

#ifndef VULKAN_ENGINE_PIPELINE_LAYOUT_HPP
#define VULKAN_ENGINE_PIPELINE_LAYOUT_HPP

#include <vulkan/vulkan.h>

struct PipelineLayout {
    //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkPipelineLayoutCreateInfo.html
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};

    PipelineLayout() = default;

    PipelineLayout(const uint32_t no_descriptor_sets, VkDescriptorSetLayout* descriptor_set_layouts, const uint32_t no_push_constants, const VkPushConstantRange* push_constant_ranges){
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;   //sType must be VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO
        pipelineLayoutInfo.setLayoutCount = no_descriptor_sets;         //the number of descriptor sets to include
        pipelineLayoutInfo.pSetLayouts = descriptor_set_layouts;        //a pointer to the descriptor set layouts (an array of VkDescriptorSetLayout objects)
        pipelineLayoutInfo.pushConstantRangeCount = no_push_constants;  //the number of push constants to include
        pipelineLayoutInfo.pPushConstantRanges = push_constant_ranges;  //pointer to an array of push constant ranges (an array of VkPushConstantRange objects)
    }

    [[nodiscard]] VkPipelineLayoutCreateInfo& get_pipeline_stage() {return pipelineLayoutInfo;}
};

#endif //VULKAN_ENGINE_PIPELINE_LAYOUT_HPP
