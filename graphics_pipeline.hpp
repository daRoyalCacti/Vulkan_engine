//
// Created by jacob on 4/11/21.
//

#ifndef VULKAN_ENGINE_GRAPHICS_PIPELINE_HPP
#define VULKAN_ENGINE_GRAPHICS_PIPELINE_HPP

#include <string_view>
#include "graphics_pipeline/shader.hpp"
#include "logical_device.hpp"
#include "swap_chain.hpp"
#include "render_pass.hpp"

struct GraphicsPipeline {
    GraphicsPipeline(LogicalDevice &d, SwapChain &s, RenderPass &r) : device(d), swap_chain(s), render_pass(r) {}

    void setup();
    void cleanup();

    static constexpr std::string_view vert_loc = "../shader_bytecode/triangle_vert.spv";
    static constexpr std::string_view frag_loc = "../shader_bytecode/triangle_frag.spv";

    VkPipelineLayout pipeline_layout{};
    VkPipeline grahics_pipeline{};

private:
    LogicalDevice &device;
    SwapChain &swap_chain;
    RenderPass &render_pass;
};


#endif //VULKAN_ENGINE_GRAPHICS_PIPELINE_HPP
