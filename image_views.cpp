//
// Created by jacob on 4/11/21.
//

#include "image_views.hpp"
#include <stdexcept>

void ImageViews::setup() {
    swapChainImageViews.resize(swap_chain.swapChainImages.size()); //the number of image views is the same as the number of images in the swapchain

    //creating the image views for each image
    for (unsigned i = 0; i <  swap_chain.swapChainImages.size(); i++) {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;    //sType must be VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO
        createInfo.image = swap_chain.swapChainImages[i];   //the image to create the view for
        //the interpretation of the image
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;    //the type of the image. See https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkImageViewType.html
        createInfo.format = swap_chain.surface_format.format;   //the format of the image (e.g. rgba). See https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkFormat.html
                                                                //format of the view is clearly the same as the image
        //you can swizzle the colours arouund (e.g. set the red component to 0)
        // - see https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkComponentSwizzle.html
        //don't want to do anything fancy
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        //what the purpose of the image is and which part of the image should be accessed
        // - see https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkImageSubresourceRange.html
        //doing nothing fancy so these values are all very simple
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT; //which aspects of the image to take the view
        createInfo.subresourceRange.baseMipLevel = 0;   //first mipmap level accessible to the view
        createInfo.subresourceRange.levelCount = 1;     //number of mipmap levels, starting from baseMipLevel, accessible to the view
        createInfo.subresourceRange.baseArrayLayer = 0; //first array layer accessible to the view
        createInfo.subresourceRange.layerCount = 1; //the number of array layers, starting from baseArrayLayer, accessible

        //now actually creating the image
        const auto create_res = vkCreateImageView(device.get_device(), &createInfo, nullptr, &swapChainImageViews[i]);
        if (create_res != VK_SUCCESS) {
            throw std::runtime_error("failed to create image views");
        }
    }

}

void ImageViews::cleanup() {
    for (const auto &imageView : swapChainImageViews) {
        vkDestroyImageView(device.get_device(), imageView, nullptr);
    }
}
