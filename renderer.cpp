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
}


void Renderer::cleanup() {
    //destroying the debug messenger
    // - debug messenger must be destroyed before the instance because the debug messenger depends on the instance
#ifdef VALDIATION_LAYERS
    debug_messenger.cleanup();
#endif

    //closing the instance
    instance.cleanup();
}