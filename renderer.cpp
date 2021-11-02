//
// Created by jacob on 31/10/21.
//

#include "renderer.hpp"


void Renderer::initVulkan() {
    instance.create();
#ifdef VALDIATION_LAYERS
    //setting up the debug messenger requires the instance to be set
    debug_messenger.setup();
#endif
}


void Renderer::mainLoop() {
    //having the main loop run units the (x) is pressed
    while (!glfwWindowShouldClose(window.get_window())) {
        glfwPollEvents();   //checking for events (like (x) being pressed)
    }
}

void Renderer::cleanup() {
    //destroying the debug messenger
#ifdef VALDIATION_LAYERS
debug_messenger.cleanup();
#endif

    //closing the instance
    instance.cleanup();
}