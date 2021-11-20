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
    GraphicsPipeline(LogicalDevice &d, SwapChain &s, RenderPass &r, const std::string_view vertex_shader_loc, const std::string_view frag_shader_loc)
        : device(d), swap_chain(s), render_pass(r), vert_loc(vertex_shader_loc), frag_loc(frag_shader_loc) {}

    void setup();
    void cleanup();

    const std::string_view vert_loc;
    const std::string_view frag_loc;

    VkPipelineLayout pipeline_layout{};
    VkPipeline grahics_pipeline{};

    [[nodiscard]] VkPipeline& get_pipeline(){ return grahics_pipeline; }

private:
    LogicalDevice &device;
    SwapChain &swap_chain;
    RenderPass &render_pass;
};


#endif //VULKAN_ENGINE_GRAPHICS_PIPELINE_HPP
