//
// Created by jacob on 31/10/21.
//

#ifndef VULKAN_ENGINE_RENDERER_HPP
#define VULKAN_ENGINE_RENDERER_HPP

#include <array>
#include <vector>
#include "window.hpp"
#include "debug_callback.hpp"
#include "instance.hpp"
#include "physical_device.hpp"


struct Renderer {
#ifdef VALDIATION_LAYERS
    explicit Renderer(Window& w) : window(w), debug_messenger(instance), physical_device(instance) {}
#else
    Renderer(Window& w) : window(w) {};
#endif
    void initVulkan();
    void cleanup();

private:
    //variables for the window
    Window& window;

    //An instance is created by describing your application and any API extensions you will be using
    Instance instance;

    //validation layers
#ifdef VALDIATION_LAYERS
    DebugMessenger debug_messenger;    //custom messenger for the validation layers
#endif

    //The graphics card to use
    PhysicalDevice physical_device;

};


#endif //VULKAN_ENGINE_RENDERER_HPP
