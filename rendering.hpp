//
// Created by jacob on 31/10/21.
//

#ifndef VULKAN_ENGINE_RENDERING_HPP
#define VULKAN_ENGINE_RENDERING_HPP

#include <array>
#include <vector>
#include "window.hpp"
#include "debug_callback.hpp"
#include "instance.hpp"


struct Rendering {
#ifdef VALDIATION_LAYERS
    Rendering() : debug_messenger(instance) {}
#else
    Rendering() = default;
#endif
    inline void initWindow() {window.makeWindow();}
    void initVulkan();
    void mainLoop();
    void cleanup();

private:
    //variables for the window
    Window window;

    //An instance is created by describing your application and any API extensions you will be using
    Instance instance;

    //validation layers
#ifdef VALDIATION_LAYERS
    DebugMessenger debug_messenger;    //custom messenger for the validation layers
#endif







};


#endif //VULKAN_ENGINE_RENDERING_HPP
