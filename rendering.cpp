//
// Created by jacob on 31/10/21.
//

#include "rendering.hpp"
#include <stdexcept>
#include <iostream>


void Rendering::initVulkan() {
    instance.create();
#ifdef VALDIATION_LAYERS
    debug_messenger.setup(instance);
#endif
}


void Rendering::mainLoop() {
    //having the main loop run units the (x) is pressed
    while (!glfwWindowShouldClose(window.get_window())) {
        glfwPollEvents();   //checking for events (like (x) being pressed)
    }
}

void Rendering::cleanup() {
    //destroying the debug messenger
#ifdef VALDIATION_LAYERS
debug_messenger.cleanup();
#endif

    //closing the instance
    instance.cleanup();

    //closing the window
    window.cleanup();

}