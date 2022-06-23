//
// Created by jacob on 19/11/21.
//

#include "command_buffers.hpp"

#include <stdexcept>
#include <iostream>

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
        // - need to clear both the colour and depth attachments
        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
        clearValues[1].depthStencil = {1.0f, 0};

        renderPassInfo.clearValueCount = clearValues.size();             //the number of clear colours
        renderPassInfo.pClearValues = clearValues.data();    //array that holds the clear value for each framebuffer
                                                        //array is indexed by attachment number

        //adding the render pass to the command buffer
        //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdBeginRenderPass.html
        vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        //bind the graphics pipeline
        // - VK_PIPELINE_BIND_POINT_GRAPHICS because for graphics and not for compute
        vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_pipeline1.get_pipeline());

        //drawing the triangle
        //========================================================
        //binding the buffer for drawing
        // - binding it to binding 0 (the only binding)
        VkBuffer vertexBuffers1[] = {vertex_buffer1.vertexBuffer};
        VkDeviceSize offsets1[] = {0};
        vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers1, offsets1);
        //The first two parameters, besides the command buffer, specify the offset and number of bindings we're going to specify vertex buffers for.
        //The last two parameters specify the array of vertex buffers to bind and the byte offsets to start reading vertex data from

        //telling vulkan to draw the triangle
        vkCmdDraw(commandBuffers[i], vertex_buffer1.vertices.size(), 1, 0, 0);
        // - The first parameter is just binding to the command buffer
        // - The second parameter is the number of vertices (just 3 because using a triangle)
        // - The third parameter is the offset into the vertex buffer
        // - The final parameter is and offset used for instanced rendering

        //drawing the square
        //==========================================================
        //using a different pipeline because using a different shader to draw this
        // - not can just have multiple calls to vkCmdDraw and/or vkCmdDrawIndexed in the same graphics pipeline
        vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_pipeline2.get_pipeline());

        //note this is no done optimally
        //should have the vertex and index buffers as one big buffer and use offsets
        VkBuffer vertexBuffers2[] = {vertex_buffer2.vertexBuffer};
        VkDeviceSize offsets2[] = {0};
        vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers2, offsets2);

        vkCmdBindIndexBuffer(commandBuffers[i], index_buffer.indexBuffer, 0, VK_INDEX_TYPE_UINT16);  //index buffer uses 16bit integers

        //binding the descriptor set
        // - i.e. updating the layout values in the shader
        vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_pipeline2.pipeline_layout, 0, 1, &descriptor_set.get_sets()[i], 0, nullptr);

        vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(index_buffer.indices.size()), 1, 0, 0, 0);


        //drawing the second square
        //==========================================================
        //using a different pipeline because using a different shader to draw this
        // - not can just have multiple calls to vkCmdDraw and/or vkCmdDrawIndexed in the same graphics pipeline
        vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_pipeline3.get_pipeline());

        //note this is no done optimally
        //should have the vertex and index buffers as one big buffer and use offsets
        VkBuffer vertexBuffers3[] = {vertex_buffer3.vertexBuffer};
        VkDeviceSize offsets3[] = {0};
        vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers3, offsets3);

        vkCmdBindIndexBuffer(commandBuffers[i], index_buffer.indexBuffer, 0, VK_INDEX_TYPE_UINT16);  //index buffer uses 16bit integers

        //binding the descriptor set
        // - i.e. updating the layout values in the shader
        vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_pipeline3.pipeline_layout, 0, 1, &descriptor_set2.get_sets()[i], 0, nullptr);

        vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(index_buffer.indices.size()), 1, 0, 0, 0);

        //binding the descriptor set for the other textured square
        // - i.e. updating the layout values in the shader
        vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_pipeline3.pipeline_layout, 0, 1, &descriptor_set3.get_sets()[i], 0, nullptr);

        vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(index_buffer.indices.size()), 1, 0, 0, 0);

        //no longer recording to the render pass
        vkCmdEndRenderPass(commandBuffers[i]);

        //no longer recording the command buffer
        if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
    }

}
