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

    //creating the render pass -- must be done before creating the graphics pipeline
    render_pass.setup();

    //creating the graphics pipeline
    graphics_pipeline.setup();

    //creating the framebufers
    framebuffers.setup();

    //creating command pools
    command_pool.setup();

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

    //destoying semaphores
    semaphores.cleanup();

    //destroying the command pool
    command_pool.cleanup();

    //destroying the framebuffers
    framebuffers.cleanup();

    //destroying the graphics pipeline
    graphics_pipeline.cleanup();

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
    vkAcquireNextImageKHR(logical_device.get_device(), swap_chain.get_swap_chain(), UINT64_MAX, semaphores.imageAvailableSemaphore[currentFrame], VK_NULL_HANDLE, &imageIndex);
    // - the 3rd parameter specifies a timeout in nanoseconds to wait for an image to become available (setting it to UINT64_MAX disables the timeout)
    // - 4th and 5th parameters specify synchronisation objects that are to be signaled when the presentation engine is finished using the image (That's the point in time where we can start drawing to it.)
    // - the final parameter is the index in the swap chain of the current image (use this to pick the correct command buffer

    //Check if a previous frame is using this image (i.e. there is its fence to wait on)
    // - if so wait for the image to be finished rendering to
    if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
        vkWaitForFences(logical_device.get_device(), 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
    }
    // Mark the image as now being in use by this frame
    imagesInFlight[imageIndex] = fences.get_fences()[currentFrame];

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
    vkQueuePresentKHR(logical_device.present_queue, &presentInfo);

    //storing the current frame that is being worked on
    currentFrame = (currentFrame + 1) % max_frames_in_flight;
}

void Renderer::endDrawFrame() {
    //do not want to start cleaning up while drawing is still going on
    vkDeviceWaitIdle(logical_device.get_device());
}
