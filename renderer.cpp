//
// Created by jacob on 31/10/21.
//

#include "renderer.hpp"
#include <stdexcept>


void Renderer::initVulkan() {
    //instance must be created first because this describes all the features from vulkan that we need
    instance.create();
#ifdef VALDIATION_LAYERS
    //setting up the debug messenger requires the instance to be set correct
    // - need to specify additional vulkan features to use the custom messenger.
    debug_messenger.setup();
#endif

    //setting up the surface to render to
    surface.setup();

    //selecting the graphics card that
    // 1. supports the features we need
    // 2. would give the best performance
    //must be done before creating the logical device
    physical_device.setup();

    //finding the present and graphics queues to use
    queue_family.setup();

    //setting up the interface to the physical device
    logical_device.setup();


    //setting up the framebuffers
    swap_chain.setup();

    //creating views into the swapchain images
    image_views.setup();

    //setting up the uniform buffer object
    // - must be created before the descriptor set
    uniform_buffer_object.setup();
    uniform_buffer_object2.setup();
    uniform_buffer_object3.setup();

    //creating command pools
    command_pool.setup();

    //creating the texture
    texture.setup();
    texture2.setup();

    //creating the views into the 1 texture
    texture_view.setup();
    texture_view2.setup();

    //creating how the shader accesses images (this is independent of any specific texture)
    texture_sampler.setup(VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_REPEAT);

    //creating the layout for passing data to the shaders
    // - must be done before pipeline is created
    descriptor_set_layout.setup();
    descriptor_set_layout2.setup();

    //creating the descriptor pool
    descriptor_pool.setup();
    descriptor_pool2.setup();

    //creating the descriptor sets
    descriptor_set.setup();
    descriptor_set2.setup();
    descriptor_set3.setup();

    //creating the render pass -- must be done before creating the graphics pipeline
    render_pass.setup();

    //creating the graphics pipeline
    graphics_pipeline3.setup();
    graphics_pipeline2.setup();
    graphics_pipeline1.setup();

    //creating the depth image
    depth_image.setup();

    //creating the framebufers
    framebuffers.setup();


    //creating and filling the vertex buffer
    // - must be done before command buffers are created
    vertex_buffer_triangle.setup();
    vertex_buffer_square.setup();
    vertex_buffer_square2.setup();

    //creating the index buffers
    // - must be done before command buffers are created
    index_buffer_square.setup();

    //creating and recording the drawing commands
    command_buffers.setup();

    //creating semaphores
    semaphores.setup();

    //creating the fences
    fences.setup();

    //want to be able to set that any image in the swapchain is in flight
    imagesInFlight.resize(swap_chain.swapChainImages.size(),VK_NULL_HANDLE);
}


void Renderer::cleanup() {
    //destroying the fences
    fences.cleanup();

    //destroying semaphores
    semaphores.cleanup();

    //destorying the UBOs
    uniform_buffer_object.cleanup();
    uniform_buffer_object2.cleanup();
    uniform_buffer_object3.cleanup();

    //destroying the descriptor pools
    descriptor_pool.cleanup();
    descriptor_pool2.cleanup();

    //destroying the descriptor set layout
    descriptor_set_layout.cleanup();
    descriptor_set_layout2.cleanup();

    //destroying the index buffers
    index_buffer_square.cleanup();

    //destroying the vertex buffers
    vertex_buffer_square2.cleanup();
    vertex_buffer_square.cleanup();
    vertex_buffer_triangle.cleanup();

    //destroying how the shader accesses images
    texture_sampler.cleanup();

    //destroying the view into a texture
    texture_view.cleanup();
    texture_view2.cleanup();

    //destroying the texture
    texture.cleanup();
    texture2.cleanup();

    //destroying the command pool
    command_pool.cleanup();

    //destroying the depth image
    depth_image.cleanup();

    //destroying the framebuffers
    framebuffers.cleanup();

    //destroying the graphics pipeline
    graphics_pipeline1.cleanup();
    graphics_pipeline2.cleanup();
    graphics_pipeline3.cleanup();

    //destroying the render pass
    render_pass.cleanup();

    //destroying views into the swapchain images
    image_views.cleanup();

    //destroying the framebuffers
    swap_chain.cleanup();

    //destroying the logical device (physical device does not need to be destroyed)
    logical_device.cleanup();

    //destroying the debug messenger
    // - debug messenger must be destroyed before the instance because the debug messenger depends on the instance
#ifdef VALDIATION_LAYERS
    debug_messenger.cleanup();
#endif

    //closing the surface
    surface.cleanup();

    //closing the instance
    instance.cleanup();
}

void Renderer::drawFrame() {
    //synchronsing the CPU and the GPU (so commands don't get submitted to the GPU while the GPU is still rendering the previous frame)
    // - reset fences is also called later
    vkWaitForFences(logical_device.get_device(), 1, &fences.get_fences()[currentFrame], VK_TRUE, UINT64_MAX);

    //get the next image from the swapchain
    //=====================================
    uint32_t imageIndex;
    const auto new_img_result = vkAcquireNextImageKHR(logical_device.get_device(), swap_chain.get_swap_chain(), UINT64_MAX, semaphores.imageAvailableSemaphore[currentFrame], VK_NULL_HANDLE, &imageIndex);
    // - the 3rd parameter specifies a timeout in nanoseconds to wait for an image to become available (setting it to UINT64_MAX disables the timeout)
    // - 4th and 5th parameters specify synchronisation objects that are to be signaled when the presentation engine is finished using the image (That's the point in time where we can start drawing to it.)
    // - the final parameter is the index in the swap chain of the current image (use this to pick the correct command buffer

    //if the swap chain is no longer adequate (i.e. the window was resized)
    // - VK_ERROR_OUT_OF_DATE_KHR: The swap chain has become incompatible with the surface and can no longer be used for rendering. Usually happens after a window resize
    // - VK_SUBOPTIMAL_KHR: The swap chain can still be used to successfully present to the surface, but the surface properties are no longer matched exactly.
    if (new_img_result == VK_ERROR_OUT_OF_DATE_KHR) {
        //if the window was resized, the swap chain needs to be recreated
        recreateSwapChain();
        return; //cannot present to current swapchain so need to return
    } else if (new_img_result != VK_SUCCESS && new_img_result != VK_SUBOPTIMAL_KHR) {
        //some other error
        throw std::runtime_error("failed to get next swap chain image");
    }

    //Check if a previous frame is using this image (i.e. there is its fence to wait on)
    // - if so wait for the image to be finished rendering to
    if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
        vkWaitForFences(logical_device.get_device(), 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
    }
    // Mark the image as now being in use by this frame
    imagesInFlight[imageIndex] = fences.get_fences()[currentFrame];

    //updating the uniform buffers
    uniform_buffer_object.update(imageIndex);
    uniform_buffer_object2.update(imageIndex);
    uniform_buffer_object3.update(imageIndex);

    //submitting the command buffer
    //=============================
    VkSubmitInfo submitInfo{};  //https://khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkSubmitInfo.html
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;   //sType must be VK_STRUCTURE_TYPE_SUBMIT_INFO
    //setting that writing to the images must take place once the image is available
    submitInfo.waitSemaphoreCount = 1;                                      //the number of semaphores to wait before executing the command buffers
    submitInfo.pWaitSemaphores = &semaphores.imageAvailableSemaphore[currentFrame];       //the semaphores to wait for before executing the command buffer
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};    //want rendering to happen after these pipeline stages
    submitInfo.pWaitDstStageMask = waitStages;                              //array of pipeline stages which the semaphores will wait
    //the command buffers to execute
    submitInfo.commandBufferCount = 1;                                                  //the number of command buffers to execute
    submitInfo.pCommandBuffers = &command_buffers.get_command_buffers()[imageIndex];    //want to execute the command buffer associated with the current framebuffer
    //setting the semaphores that trigger once rendering starts
    submitInfo.signalSemaphoreCount = 1;                                        //the number of semaphores to trigger
    submitInfo.pSignalSemaphores = &semaphores.renderFinishedSemaphore[currentFrame];         //array of semaphores to trigger

    //again synchronising the CPU and GPU
    // - needed here and not at the top of the loop because fences are used to make sure images in flight are not being rendered to
    vkResetFences(logical_device.get_device(), 1, &fences.get_fences()[currentFrame]);


    //submit the command buffer to the graphics queue for execution
    // - last argument is a fence that can be triggered when the command buffer is finished executing
    if (vkQueueSubmit(logical_device.graphics_queue, 1, &submitInfo, fences.get_fences()[currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    //presentation
    //============
    VkPresentInfoKHR presentInfo{}; //https://khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkPresentInfoKHR.html
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;     //sType must be VK_STRUCTURE_TYPE_PRESENT_INFO_KHR
    presentInfo.waitSemaphoreCount = 1;                         //the number of semaphores to wait on (before presentation can happen)
    presentInfo.pWaitSemaphores = &semaphores.renderFinishedSemaphore[currentFrame];  //the array of semaphores to wait on (before presentation can happen)
    presentInfo.swapchainCount = 1;                             //the number of swap chains to present the images to
    presentInfo.pSwapchains = &swap_chain.get_swap_chain();     //the array of swap chainsto present the images to
    presentInfo.pImageIndices = &imageIndex;                    //the image in the swapchain to present the images to
    presentInfo.pResults = nullptr;             //array of VkResult values to check if every swapchain presentation was successful
                                                // - not necessary because we are only using a single swapchain
    //submitting the request to present and image to the swap chain
    const auto pres_result = vkQueuePresentKHR(logical_device.present_queue, &presentInfo);

    //again if the swapchain is no longer adequate
    // - see above for code comments
    //also consider when the window is explicitly marked as being resized
    // - vulkan is not guaranteed to return errors if the swap chain is not valid (although it almost always will)
    if (pres_result == VK_ERROR_OUT_OF_DATE_KHR || pres_result == VK_SUBOPTIMAL_KHR || window.window_resized) {
        window.window_resized = false;
        recreateSwapChain();
    } else if (pres_result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    //storing the current frame that is being worked on
    currentFrame = (currentFrame + 1) % max_frames_in_flight;
}

void Renderer::endDrawFrame() {
    //do not want to start cleaning up while drawing is still going on
    vkDeviceWaitIdle(logical_device.get_device());
}

void Renderer::recreateSwapChain() {
    //catching the degenerate case of a window size of 0
    // - in this case just pausing the application until the window size is not 0
    int width, height;
    glfwGetFramebufferSize(window.get_window(), &width, &height);   //getting the size of thw window
    while (width == 0 || height == 0) { //don't do anything until the window is larger than 0
        glfwGetFramebufferSize(window.get_window(), &width, &height);
        glfwWaitEvents();
    }


    vkDeviceWaitIdle(logical_device.get_device());   //should not touch any resources that may be in flight

    //cleaning up old swap-chain
    // - cleaning up up everything that needs to be recreated (generally in the reverse order that they are created)
    //=========================
    descriptor_pool.cleanup();
    descriptor_pool2.cleanup();
    uniform_buffer_object.cleanup();
    uniform_buffer_object2.cleanup();
    uniform_buffer_object3.cleanup();
    depth_image.cleanup();
    framebuffers.cleanup();
    //we could recreate the command pool from scratch but this is wasteful
    //just cleaning up the existing command buffers
    // - can then just use the existing command pool to allocate the new command buffers
    vkFreeCommandBuffers(logical_device.get_device(), command_pool.get_command_pool(), static_cast<unsigned>(command_buffers.get_command_buffers().size()), command_buffers.get_command_buffers().data() );
    graphics_pipeline1.cleanup();
    graphics_pipeline2.cleanup();
    graphics_pipeline3.cleanup();
    render_pass.cleanup();
    image_views.cleanup();
    swap_chain.cleanup();


    //creating the new swap-chain
    //===========================
    swap_chain.setup();         //obviously have to re-create the swap-chain
    image_views.setup();        //image views are directly for the images in the swap chain and so need to be recreated
    render_pass.setup();        //render pass depends on the format of the swap-chain images
                                // - the format of the images shouldn't change during window resize but just catching the edge case
    graphics_pipeline1.setup(); //viewport and scissor changes so the graphics pipeline needs to be recreated
    graphics_pipeline2.setup(); // - could avoid this using dynamic states for the viewport and the scissors
    graphics_pipeline3.setup();
    depth_image.setup();        //size of the depth image depends on the size of the images in the swap chain
    framebuffers.setup();       //frame buffers and command buffers depend directly on the swap chain images
    uniform_buffer_object.setup();  //the UBOs depend on the number of images in the swapchain
    uniform_buffer_object2.setup();
    uniform_buffer_object3.cleanup();
    descriptor_pool.setup();        //depends on the number of images in the swapchain
    descriptor_pool2.setup();
    descriptor_set.setup();         //  ditto
    descriptor_set2.setup();
    descriptor_set3.setup();
    command_buffers.setup();
}
