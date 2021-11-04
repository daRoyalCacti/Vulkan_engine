//
// Created by jacob on 31/10/21.
//

#include "renderer.hpp"


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
    physical_device.setup();

    //setting up the interface to the physical device
    logical_device.setup();

    //setting up the framebuffers
    swap_chain.setup();

    //creating views into the swapchain images
    image_views.setup();

    //creating the graphics pipeline
    graphics_pipeline.setup();

}


void Renderer::cleanup() {
    //destroying the graphics pipeline
    graphics_pipeline.cleanup();

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