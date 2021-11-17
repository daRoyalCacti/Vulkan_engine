//
// Created by jacob on 17/11/21.
//

#ifndef VULKAN_ENGINE_MULTISAMPLING_HPP
#define VULKAN_ENGINE_MULTISAMPLING_HPP

#include <vulkan/vulkan.h>

//holds the information on multisampling
//more than this needs to be changed when changing multisampling
// - performs anti-aliasing by taking more samples per pixel
// - (very similar to downscaling a higher resolution image)
struct Multisample{
    //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkPipelineMultisampleStateCreateInfo.html
    VkPipelineMultisampleStateCreateInfo multisampling{};

    Multisample(const VkSampleCountFlagBits samples, const VkBool32 sample_shading, const float min_sample_shading) {
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO; //sType must be VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO
        multisampling.sampleShadingEnable = sample_shading;     //improves the quality of textures at a performance cost
        multisampling.rasterizationSamples = samples;           //the number of samples per pixel to use
                                                                // - if true, logical device feature sampleShaingEnable must be active
        multisampling.minSampleShading = min_sample_shading;    //minimum fraction for sample shading --- closer to 1 means smoother image
        multisampling.pSampleMask = nullptr;                    //pointer to an array of VkSample masks (not entirely sure what these do)
        multisampling.alphaToCoverageEnable = VK_FALSE;         //multisampling writes to a temporary buffer before writing to the main frame buffer
                                                                //This controls whether the alpha component is that of the fragment or as specified in Multisample Coverage
                                                                // - https://www.khronos.org/registry/vulkan/specs/1.2-extensions/html/vkspec.html#fragops-covg
                                                                //practically to enable multisampling alpha blending using coverage masks (a way to do order-independent transparency)
                                                                // - https://en.wikipedia.org/wiki/Alpha_to_coverage
        multisampling.alphaToOneEnable = VK_FALSE;              //multisampling writes to a temporary buffer before writing to the main frame buffer
                                                                //This controls whether this buffer has alpha to 1
                                                                // - if true, requires logical device feature alphaToOneEnable to be active

    }

    [[nodiscard]] VkPipelineMultisampleStateCreateInfo& get_pipeline_stage() {return multisampling;}
};

#endif //VULKAN_ENGINE_MULTISAMPLING_HPP
