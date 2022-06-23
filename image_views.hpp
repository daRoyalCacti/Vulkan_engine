//
// Created by jacob on 4/11/21.
//

#ifndef VULKAN_ENGINE_IMAGE_VIEWS_HPP
#define VULKAN_ENGINE_IMAGE_VIEWS_HPP

#include <vector>
#include <vulkan/vulkan.h>
#include "swap_chain.hpp"

//helper function for creating image views
void createImageView(LogicalDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VkImageView &imageView);

//to use any VkImage we have to use a VkImageView object
// - it is just a view into the image
// - it describes what part of the image to acess
//this stores the image views to all images in the swapchain
//image view is suffucient to start using the image as a texture
struct ImageViews {
    ImageViews(SwapChain &s, LogicalDevice& d) : swap_chain(s), device(d) {}

    std::vector<VkImageView> swapChainImageViews;   //array to store the image views

    SwapChain &swap_chain;
    LogicalDevice &device;

    void setup();
    void cleanup();
};


#endif //VULKAN_ENGINE_IMAGE_VIEWS_HPP
