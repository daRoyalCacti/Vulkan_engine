//
// Created by jacob on 19/11/21.
//

#ifndef VULKAN_ENGINE_COMMAND_BUFFERS_HPP
#define VULKAN_ENGINE_COMMAND_BUFFERS_HPP

#include <vulkan/vulkan.h>
#include "command_pool.hpp"
#include "framebuffers.hpp"
#include "logical_device.hpp"
#include "render_pass.hpp"
#include "swap_chain.hpp"
#include "graphics_pipeline.hpp"

//all commands in vulkan must be submitted using a command buffer
// - command buffers are allocated from command pools
struct CommandBuffers {
    CommandBuffers(LogicalDevice &d, CommandPool &c, Framebuffers &f, RenderPass &r, SwapChain &s, GraphicsPipeline &g) : device(d), command_pool(c), frame_buffers(f), render_pass(r),
        swap_chain(s), graphics_pipeline(g){}

    //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkCommandBuffer.html
    std::vector<VkCommandBuffer> commandBuffers;    //need a command buffer for every framebuffer

    void setup();

    //the colour the screen gets cleared to
    static constexpr VkClearValue clear_colour = {{{0.0f, 0.0f, 0.0f, 1.0f}}};

private:
    LogicalDevice &device;
    CommandPool &command_pool;
    Framebuffers &frame_buffers;
    RenderPass &render_pass;
    SwapChain &swap_chain;
    GraphicsPipeline &graphics_pipeline;
};


#endif //VULKAN_ENGINE_COMMAND_BUFFERS_HPP
