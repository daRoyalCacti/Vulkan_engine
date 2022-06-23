//
// Created by jacob on 17/11/21.
//

#include "render_pass.hpp"
#include <stdexcept>

void RenderPass::setup() {
    //specifying some features of the framebuffer
    // - https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkAttachmentDescription.html
    //Specifying the colour attachment
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = swap_chain.surface_format.format;  //specifies the format of the image view that will be used
                                                                // - VkFormat is like RGB, RBGA, ect. https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkFormat.html
                                                                //want this to be the same as the swapchain images
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;            //the number of samples in the image (c.f. multisampling)
                                                                // - possible values are https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkSampleCountFlagBits.html
                                                                //not doing anything with multisampling yet so this is just 1
    //what to do with the data before and after rendering
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;       //what to do with the data that already exists in the frame buffer
                                                                // - VK_ATTACHMENT_LOAD_OP_LOAD : preserve the contents
                                                                // - VK_ATTACHMENT_LOAD_OP_CLEAR : clear the values
                                                                // - VK_ATTACHMENT_LOAD_OP_DONT_CARE : don't care (contents are undefined)
                                                                //want to clear the screen in this case
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;     //what to do with the data that is to be written to the frame buffer
                                                                // - VK_ATTACHMENT_STORE_OP_STORE : save the contents in memory
                                                                // - VK_ATTACHMENT_STORE_OP_DONT_CARE : don't care (frame buffer is undefined after rendering)
                                                                //rendering to screen so want to save the contents
    //what to do with the stencil before and after rendering
    //not using a stencil buffer so don't care about the format
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    //for an image, the layout of the pixels in memory depends on what operation the image is to be used for
    // - VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL: Images used as color attachment
    // - VK_IMAGE_LAYOUT_PRESENT_SRC_KHR: Images to be presented in the swap chain
    // - VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL: Images to be used as destination for a memory copy operation
    //initial layout represents the layout of the image before rendering
    //final layout represents the layout of the image after rendering
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;      //we do not care about the format of the image since we are going to be clearing the image
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;  //the rendered images go straight to the swapchain


    //specifying the depth attachment
    VkAttachmentDescription depthAttachment{};
    depthAttachment.format = VK_FORMAT_D32_SFLOAT;                                      //this will have to update if I even include stencils
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;                                    //don't want to do multisampling on the depth buffer
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;                               //want to clear the depth buffer
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;                         //doesn't matter what happens to the image after the renderpass is done, it's just there to help with writing to the colour attacment
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;                    //no stencil
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;                  //no stencil
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;                          //don't care about the format as we're clearing it
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;     //the depth buffer is unsurprisingly going to be a depth attachment

    //specifying how to attach the colour attachment
    VkAttachmentReference colorAttachmentRef{}; //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkAttachmentReference.html
    colorAttachmentRef.attachment = 0;          //index of the attachment (in the VkRenderPassCreateInfo::pAttachments below)
                                                // - colour attachment is first (followed by depth)
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;   //will give the best performance

    //specifying how to attach the depth buffer
    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 1;          //depth is second, after the colour attachment
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;   //will give the best performance


    //setting up subpasses
    // - Subpasses are subsequent rendering operations that depend on the contents of framebuffers in previous passes (e.g. post processing efects)
    //sticking to a single subpass RN
    //specifying the number of subpasses --- i.e. the number of times the fragment shader is run
    //and the colour attachments used for these subpasses
    VkSubpassDescription subpass{}; //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkSubpassDescription.html
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;    //the subpass is for graphics (and not for compute or raytracing)
                                                                    // - https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkSubpassDescription.html
    subpass.colorAttachmentCount = 1;                               //the number of colour attachments
                                                                    // - this refers to the `layout(location = 0) out vec4 outColor' in the fragment shader
    subpass.pColorAttachments = &colorAttachmentRef;                //specifying how we want the subpass to write its data
    subpass.pDepthStencilAttachment = &depthAttachmentRef;
    //is also possible to have
    // - pInputAttachments: Attachments that are read from a shader
    // - pResolveAttachments: Attachments used for multisampling color attachments
    // - pPreserveAttachments: Attachments that are not used by this subpass, but for which the data must be preserved




    //creating subpass dependencies
    // - currently taking care of the image layout transition at the start of the render pass (for the images in the framebuffer)
    VkSubpassDependency dependency{};   //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkSubpassDependency.html
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;    //the subpass index of the first subpass
                                                    // - VK_SUBPASS_EXTERNAL refers to the implicit subpass before (or after) a render pas
    dependency.dstSubpass = 0;                      //Refers to the first (and only) subpass
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;    //the operations to wait on (finish depth testing and writing to the colour attachment)
                                                                                                                             // - https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkPipelineStageFlagBits.html
    dependency.srcAccessMask = 0;                                               //the stages where the above operation occurs (in this case the only stage)
                                                                                // - https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkAccessFlagBits.html
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;       //The operations that should wait on this are in the color attachment and depth testing stage
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;             // - this prevents the transition from happening until it's actually necessary (and allowed)




    //actually creating the render pass
    std::array<VkAttachmentDescription, 2> attachments = {colorAttachment, depthAttachment};
    VkRenderPassCreateInfo renderPassInfo{};    //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkRenderPassCreateInfo.html
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;   //sType must be VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO
    renderPassInfo.attachmentCount = attachments.size();  //total number of attachments (the subpasses reference the attachments)
    renderPassInfo.pAttachments = attachments.data();     //pointer to an array of VkAttachmentDescription structures
    renderPassInfo.subpassCount = 1;                    //the total number of subpasses (the graphics pipeline uses these subasses)
    renderPassInfo.pSubpasses = &subpass;               //pointer to any array of VkSubpassDependency structures
    renderPassInfo.dependencyCount = 1;                 //the number of memory dependencies between subpasses
    renderPassInfo.pDependencies = &dependency;         //the array of memory dependencies


    const auto create_pass_res = vkCreateRenderPass(device.get_device(), &renderPassInfo, nullptr, &render_pass);
    if (create_pass_res != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass");
    }

}

void RenderPass::cleanup() {
    vkDestroyRenderPass(device.get_device(), render_pass, nullptr);
}
