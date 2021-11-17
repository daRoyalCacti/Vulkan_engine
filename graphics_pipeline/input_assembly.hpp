//
// Created by jacob on 6/11/21.
//

#ifndef VULKAN_ENGINE_INPUT_ASSEMBLY_HPP
#define VULKAN_ENGINE_INPUT_ASSEMBLY_HPP

#include <vulkan/vulkan.h>

//what kind of geometry will be drawn
//and if the possible to break up line and triangle strips
struct InputAssembly {
    //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkPipelineInputAssemblyStateCreateInfo.html
    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};

    explicit InputAssembly(const VkPrimitiveTopology topology, const VkBool32 restart = VK_FALSE) {
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;  //sType must be VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO
        inputAssembly.topology = topology;                  //the type of data. E.g. triangle strip, or points
                                                            // - see https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkPrimitiveTopology.html
        inputAssembly.primitiveRestartEnable = restart;     //is it possible to break up lines and triangles in the _STRIP topology modes by using a special index of 0xFFFF or 0xFFFFFFFF
    }

    [[nodiscard]] VkPipelineInputAssemblyStateCreateInfo& get_pipeline_stage() {return inputAssembly;}
};


#endif //VULKAN_ENGINE_INPUT_ASSEMBLY_HPP
