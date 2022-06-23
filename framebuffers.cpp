//
// Created by jacob on 18/11/21.
//

#include "framebuffers.hpp"
#include <stdexcept>

void Framebuffers::setup() {
    const auto no_framebuffers = image_views.swapChainImageViews.size();
    swapChainFramebuffers.resize(no_framebuffers);   //need as many framebuffers as images in the swap chain

    //making each frame buffer
    for (size_t i = 0; i < no_framebuffers; i++) {
        std::array<VkImageView, 2> attachments = {
                image_views.swapChainImageViews[i],
                depth_image.depthImageView  //depth image can be used for all images because only a single subpass will run at once
        };

        //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkFramebufferCreateInfo.html
        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;      //sType must be VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO
        framebufferInfo.renderPass = render_pass.get_render_pass();             //the renderpass that the framebuffers are compatible with
        framebufferInfo.attachmentCount = attachments.size();                   //the number of elements in the pAttachments array
        framebufferInfo.pAttachments = attachments.data();                      //the image views that should be used -- want this to be the same as the swap chain
        framebufferInfo.width = swap_chain.extent.width;                        //the width of the image
        framebufferInfo.height = swap_chain.extent.height;                      //the height of the image
        framebufferInfo.layers = 1;                                             //the number of layers in the image

        const auto create_res = vkCreateFramebuffer(device.get_device(), &framebufferInfo, nullptr, &swapChainFramebuffers[i]);
        if (create_res != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
}

void Framebuffers::cleanup() {
    for (auto& framebuffer : swapChainFramebuffers) {
        vkDestroyFramebuffer(device.get_device(), framebuffer, nullptr);
    }

}
