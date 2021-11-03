//
// Created by jacob on 4/11/21.
//

#include "swap_chain.hpp"
#include "swap_chain_details.hpp"
#include "queue_family.hpp"
#include <stdexcept>

void SwapChain::setup() {
    //firstly getting all supported swap chains
    SwapChainDetails swap_chain_details;
    swap_chain_details.query_swap_chain_support(device.physical_device.get_device(), surface.get_surface());

    //then selecting the best swap chain features that are supported
    // - these best features are defined in the SwapChainDetails struct
    surface_format = swap_chain_details.chooseSwapSurfaceFormat();
    present_mode = swap_chain_details.chooseSwapPresentMode();
    extent = swap_chain_details.chooseSwapExtent(window.get_window());

    //number of images to use in the swap chain
    //recommended to use 1 more than the minimum
    unsigned no_images = swap_chain_details.capabilities.minImageCount + 1;
    //making sure that 1 more than the minimum is posible.
    //if not just taking the minimum
    if (swap_chain_details.capabilities.maxImageCount > 0 && no_images > swap_chain_details.capabilities.maxImageCount) {  //maxImageCount=0 means no maximum
        no_images = swap_chain_details.capabilities.maxImageCount;
    }

    //finding the queue indices
    QueueFamily queue_indices;
    queue_indices.findQueueFamilies(device.physical_device.get_device(), surface.get_surface());
    //putting the queue indices in an array (as required by vulkan)
    const unsigned queue_family_indices[] = {queue_indices.graphicsFamily.value(), queue_indices.presentFamily.value()};


    //filling the struct to create the swapchain
    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface.get_surface(); //the surface the swap chain should use
    //specifying the best settings found
    createInfo.minImageCount = no_images;                       //number of images in the swapchain
    createInfo.imageFormat = surface_format.format;             //the colour format (e.g. rgba) to sued
    createInfo.imageColorSpace = surface_format.colorSpace;     //the colour space to use (e.g. srbg)
    createInfo.imageExtent = extent;                            //the size of the framebuffer images
    createInfo.presentMode = present_mode;                      //the presentation mode to use (e.g. vsync)

    createInfo.imageArrayLayers = 1;                            //number of image layers (almost always 1)
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;//the kind of operations the images will be used for
                                                                // - it is possible to not render directly to the swap chain in which case you'd want to use VK_IMAGE_USAGE_TRANSFER_DST_BIT
    //specifying the queues to use
    // - need to consider when the present and graphics queues are the same and different
    if (queue_indices.graphicsFamily != queue_indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;   //saying the images can be used across multiple queues (can have the mode be exclusive, but then we would have to consider ownership)
        createInfo.queueFamilyIndexCount = 2;                   //number of queue families having access to the images of the swapchain
        createInfo.pQueueFamilyIndices = queue_family_indices;  //pointer the array of indices having access to the swapchain
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;    //saying the images can be used by 1 queue
        createInfo.queueFamilyIndexCount = 0;       //only relevant if sharing mode is VK_SHARING_MODE_CONCURRENT
        createInfo.pQueueFamilyIndices = nullptr;   //only relevant if sharing mode is VK_SHARING_MODE_CONCURRENT
    }

    //transformations to the image
    // - e.g. rotate by 90deg or horizontal flip
    //note that the transforms must be in the supportedTransforms member of capabilities
    createInfo.preTransform = swap_chain_details.capabilities.currentTransform; //don't want any transformations so just using the current transform

    //if want to blend the alpha channel with other windows in the system
    // - never want this (and so its not even a setting)
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    //whether to delete pixels if they obscured (e.g. by a window)
    createInfo.clipped = clip_pixels;   //clip pixels defined in the main struct

    //only useful when recreating the swapchain
    // - swapchain needs to be recreated when the window is resized
    //this would be a reference to old swapchain that the new one is created from
    createInfo.oldSwapchain = VK_NULL_HANDLE;


    //creation struct is filled, so the swapchain can be created
    const auto creation_result = vkCreateSwapchainKHR(device.get_device(), &createInfo, nullptr, &swapChain);
    if (creation_result != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain");
    }

    //getting handles to the images created by the swapchain
    unsigned image_count;   //holds the number of images
                            //we only specified the minimum number of images for the swap chain to create --- it can crate more
    vkGetSwapchainImagesKHR(device.get_device(), swapChain, &image_count, nullptr); //querying the number of images in the swapchain
    swapChainImages.resize(image_count);
    vkGetSwapchainImagesKHR(device.get_device(), swapChain, &image_count, swapChainImages.data());  //querying the images in the swapchain


}

