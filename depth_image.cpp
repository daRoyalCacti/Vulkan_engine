//
// Created by jacob on 21/6/22.
//

#include "depth_image.hpp"
#include "image_views.hpp"

void DepthImage::setup() {
    VkFormat depthFormat = VK_FORMAT_D32_SFLOAT;    //the tutorial has a whole section on finding the best format
                                                    // - all the other formats use stencils which I'm not using yet so those formats aren't useful
                                                    // - https://vulkan-tutorial.com/Depth_buffering
    //depth image should be the same size as the images in the swapchain
    create_image(device, swap_chain.extent.width, swap_chain.extent.height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                 depthImage, depthImageMemory);
    createImageView(device, depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, depthImageView);
}
