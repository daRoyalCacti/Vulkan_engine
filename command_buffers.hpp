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
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"
#include "descriptor_set.hpp"

//all commands in vulkan must be submitted using a command buffer
// - command buffers are allocated from command pools
struct CommandBuffers {
    CommandBuffers(LogicalDevice &d, CommandPool &c, Framebuffers &f, RenderPass &r, SwapChain &s, GraphicsPipeline &g1, GraphicsPipeline &g2, GraphicsPipeline &g3, VertexBuffer<Vertex::TWOD_VC> &v1,
                   VertexBuffer<Vertex::TWOD_VC> &v2, IndexBuffer<uint16_t> &i, DescriptorSet &set,
                   VertexBuffer<Vertex::TWOD_VC> &v3, DescriptorSet &set2)
        : device(d), command_pool(c), frame_buffers(f), render_pass(r), swap_chain(s), graphics_pipeline1(g1), graphics_pipeline2(g2), graphics_pipeline3(g3), vertex_buffer1(v1), vertex_buffer2(v2),
          index_buffer(i), descriptor_set(set), vertex_buffer3(v3), descriptor_set2(set2){}

    //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkCommandBuffer.html
    std::vector<VkCommandBuffer> commandBuffers;    //need a command buffer for every framebuffer

    void setup();

    [[nodiscard]] std::vector<VkCommandBuffer>& get_command_buffers() {return commandBuffers;}

    //the colour the screen gets cleared to
    static constexpr VkClearValue clear_colour = {{{0.0f, 0.0f, 0.0f, 1.0f}}};

private:
    LogicalDevice &device;
    CommandPool &command_pool;
    Framebuffers &frame_buffers;
    RenderPass &render_pass;
    SwapChain &swap_chain;
    GraphicsPipeline &graphics_pipeline1;
    GraphicsPipeline &graphics_pipeline2;
    GraphicsPipeline &graphics_pipeline3;
    VertexBuffer<Vertex::TWOD_VC>& vertex_buffer1;
    VertexBuffer<Vertex::TWOD_VC>& vertex_buffer2;
    VertexBuffer<Vertex::TWOD_VC>& vertex_buffer3;
    IndexBuffer<uint16_t>& index_buffer;
    DescriptorSet &descriptor_set;
    DescriptorSet &descriptor_set2;
};




#endif //VULKAN_ENGINE_COMMAND_BUFFERS_HPP
