//
// Created by jacob on 17/11/21.
//

#ifndef VULKAN_ENGINE_RENDER_PASS_HPP
#define VULKAN_ENGINE_RENDER_PASS_HPP

#include <vulkan/vulkan.h>
#include "logical_device.hpp"
#include "swap_chain.hpp"

//tells vulkan about the framebuffer attachments that'll be used.
// - specify the colour and depth bufferss
struct RenderPass {
    VkRenderPass render_pass{};

    explicit RenderPass(LogicalDevice &d, SwapChain &s) : device(d), swap_chain(s) {}

    void setup();
    void cleanup();

    [[nodiscard]] VkRenderPass& get_render_pass() {return render_pass;}

private:
    LogicalDevice& device;
    SwapChain &swap_chain;

};


#endif //VULKAN_ENGINE_RENDER_PASS_HPP
