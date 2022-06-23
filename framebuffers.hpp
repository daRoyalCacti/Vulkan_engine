//
// Created by jacob on 18/11/21.
//

#ifndef VULKAN_ENGINE_FRAMEBUFFERS_HPP
#define VULKAN_ENGINE_FRAMEBUFFERS_HPP

#include "render_pass.hpp"
#include "swap_chain.hpp"
#include "logical_device.hpp"
#include "image_views.hpp"
#include <vulkan/vulkan.h>
#include <vector>
#include "depth_image.hpp"

//Framebuffers are sections of memory that are attached to a render pass
// - these framebuffers are stored in the swap chain (which decides when to render/display the framebuffers)
struct Framebuffers {
    Framebuffers(LogicalDevice &d, ImageViews &i, RenderPass &r, SwapChain &s, DepthImage &di) : device(d), image_views(i), render_pass(r), swap_chain(s), depth_image(di) {}

    void setup();
    void cleanup();

    //the actual frame buffers
    // - https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkFramebuffer.html
    std::vector<VkFramebuffer> swapChainFramebuffers;

private:
    RenderPass &render_pass;
    SwapChain &swap_chain;
    LogicalDevice &device;
    ImageViews &image_views;
    DepthImage &depth_image;
};


#endif //VULKAN_ENGINE_FRAMEBUFFERS_HPP
