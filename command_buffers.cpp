//
// Created by jacob on 19/11/21.
//

#include "command_buffers.hpp"

#include <stdexcept>

void CommandBuffers::setup() {
    commandBuffers.resize(frame_buffers.swapChainFramebuffers.size());  //command buffer for every frame buffer
    //allocating the command buffers
    //===============================
    VkCommandBufferAllocateInfo allocInfo{};    //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkCommandBufferAllocateInfo.html
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;       //sType must be VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO
    allocInfo.commandPool = command_pool.get_command_pool();                //the command pool with which to allocate the command buffer
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;                      //should the command buffer be primary or secondary
                                                                            // - VK_COMMAND_BUFFER_LEVEL_PRIMARY: Can be submitted to a queue for execution, but cannot be called from other command buffers
                                                                            // - VK_COMMAND_BUFFER_LEVEL_SECONDARY: Cannot be submitted directly, but can be called from primary command buffers.
                                                                            //secondary command buffers are useful for reusing common operations
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());        //the number of command buffers to allocate (1 for every framebuffer)

    if (vkAllocateCommandBuffers(device.get_device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }


    //recording the command buffers
    // -  all commands that are to be recorded have the vkCmd prefix
    //===============================
    for (size_t i = 0; i < commandBuffers.size(); i++) {
        VkCommandBufferBeginInfo beginInfo{};   //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkCommandBufferBeginInfo.html
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;      //sType must be VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO
        beginInfo.flags = 0;                                                //specifies the buffers usage - https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkCommandBufferUsageFlagBits.html
                                                                            // - VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT: The command buffer will be rerecorded right after executing it once.
                                                                            // - VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT: This is a secondary command buffer that will be entirely within a single render pass.
                                                                            // - VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT: The command buffer can be resubmitted while it is also already pending execution.
                                                                            //none of these are applicable RN
        beginInfo.pInheritanceInfo = nullptr;                               //only relevant to secondary command buffers
                                                                            // - It specifies which state to inherit from the calling primary command buffers.
                                                                            //https://khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkCommandBufferInheritanceInfo.html

        //start recording the command buffers
        // - If the command buffer was already recorded once, then a call to vkBeginCommandBuffer will implicitly reset it.
        // - It's not possible to append commands to a buffer at a later time.
        // - commands can either be inline or secondary: https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkSubpassContents.html
        //    > VK_SUBPASS_CONTENTS_INLINE: The render pass commands will be embedded in the primary command buffer itself and no secondary command buffers will be executed
        //    > VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS: The render pass commands will be executed from secondary command buffers.
        if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        //Drawing starts by beginning the render pass with vkCmdBeginRenderPass
        //configuring this
        VkRenderPassBeginInfo renderPassInfo{};     //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkRenderPassBeginInfo.html
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;    //sType must be VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO
        renderPassInfo.renderPass = render_pass.get_render_pass();          //the render pass to use
        renderPassInfo.framebuffer = frame_buffers.swapChainFramebuffers[i];    //the framebuffer containing the attachments that are used with the render pass
                                                                                //currently being used as a colour attachment
        //defining the render area
        // - it should match the size of the framebuffer for best performance
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = swap_chain.extent;
        //how the screen is cleared
        renderPassInfo.clearValueCount = 1;             //the number of clear colours
        renderPassInfo.pClearValues = &clear_colour;    //array that holds the clear value for each framebuffer
                                                        //array is indexed by attachment number

        //adding the render pass to the command buffer
        //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdBeginRenderPass.html
        vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        //bind the graphics pipeline
        // - VK_PIPELINE_BIND_POINT_GRAPHICS because for graphics and not for compute
        vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_pipeline.get_pipeline());

        //telling vulkan to draw the triangle
        vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);
        // - The first parameter is just binding to the command buffer
        // - The second parameter is the number of vertices (just 3 because using a triangle)
        // - The third parameter is the offset into the vertex buffer
        // - The final parameter is and offset used for instanced rendering

        //no longer recording to the render pass
        vkCmdEndRenderPass(commandBuffers[i]);

        //no longer recording the command buffer
        if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
    }

}