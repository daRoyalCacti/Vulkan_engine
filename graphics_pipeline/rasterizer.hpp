//
// Created by jacob on 6/11/21.
//

#ifndef VULKAN_ENGINE_RASTERIZER_HPP
#define VULKAN_ENGINE_RASTERIZER_HPP

#include <vulkan/vulkan.h>

//how the geometry should be turned into fragments
struct Rasterizer {
    //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkPipelineRasterizationStateCreateInfo.html
    VkPipelineRasterizationStateCreateInfo rasterizer{};

    Rasterizer(const VkPolygonMode polygon_mode, const float line_width, const VkCullModeFlags cull_mode, const VkFrontFace front_face) {
        //some settings requiring enabling device features. See https://www.khronos.org/registry/vulkan/specs/1.2-extensions/html/vkspec.html#features-depthClamp
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;  //sType must be VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO
        rasterizer.depthClampEnable = VK_FALSE;                 //are the fragments outside the near and far plane clamped or discarded
                                                                // - if this is true, the physical device feature  depthClamp needs to be enabled
        rasterizer.rasterizerDiscardEnable = VK_FALSE;          //if true, then the geometry never passes through the rasterizer stage
                                                                // - practically disables output to the frame buffer
        rasterizer.polygonMode = polygon_mode;          //how the geometry should be filled (e.g. wireframe rendering or not)
                                                                // - see https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkPolygonMode.html
                                                                // - using any non-filled modes requires enabling the physical device feature fillModeNonSolid
        rasterizer.lineWidth = line_width;                            //the thickness of the lines in terms of number of fragments
                                                                // - a line width bigger than 1 requires enabling a device feature wideLines
        rasterizer.cullMode = cull_mode;            //what side of the polygon to cull. Front, back, none, or both (both means all triangles are discarded)
                                                                // - see https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkCullModeFlagBits.html
        rasterizer.frontFace = front_face;         //how to determine if triangles are front-facing (the vertex order CC or CCW)
                                                                // - see https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkFrontFace.html
        //the rasterizer can alter the depth values by adding a constant value
        // or biasing them on the fragments slope.
        rasterizer.depthBiasEnable = VK_FALSE;
        rasterizer.depthBiasConstantFactor = 0.0f;
        rasterizer.depthBiasClamp = 0.0f;
        rasterizer.depthBiasSlopeFactor = 0.0f;
    }

    [[nodiscard]] VkPipelineRasterizationStateCreateInfo& get_pipeline_stage() {return rasterizer;}
};


#endif //VULKAN_ENGINE_RASTERIZER_HPP
