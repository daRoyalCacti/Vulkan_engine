//
// Created by jacob on 21/6/22.
//

#ifndef VULKAN_ENGINE_DEPTH_IMAGE_HPP
#define VULKAN_ENGINE_DEPTH_IMAGE_HPP

#include "logical_device.hpp"
#include "texture.hpp"
#include "swap_chain.hpp"

struct DepthImage {
    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

    DepthImage(LogicalDevice &d, SwapChain &c) : device(d), swap_chain(c) {}

    void setup();
    void cleanup() {
        vkDestroyImageView(device.get_device(), depthImageView, nullptr);
        vkDestroyImage(device.get_device(), depthImage, nullptr);
        vkFreeMemory(device.get_device(), depthImageMemory, nullptr);
    }

private:
    LogicalDevice &device;
    SwapChain &swap_chain;
};


#endif //VULKAN_ENGINE_DEPTH_IMAGE_HPP
