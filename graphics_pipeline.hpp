//
// Created by jacob on 4/11/21.
//

#ifndef VULKAN_ENGINE_GRAPHICS_PIPELINE_HPP
#define VULKAN_ENGINE_GRAPHICS_PIPELINE_HPP

#include <string_view>
#include "shader.hpp"
#include "logical_device.hpp"

struct GraphicsPipeline {
    GraphicsPipeline(LogicalDevice &d) : device(d) {}

    void setup();
    void cleanup();

    static constexpr std::string_view vert_loc = "../shader_bytecode/triangle_vert.spv";
    static constexpr std::string_view frag_loc = "../shader_bytecode/triangle_frag.spv";

private:
    LogicalDevice &device;
};


#endif //VULKAN_ENGINE_GRAPHICS_PIPELINE_HPP
