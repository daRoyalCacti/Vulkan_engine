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
#include "logical_device.hpp"
#include "surface.hpp"
#include "swap_chain.hpp"
#include "image_views.hpp"
#include "graphics_pipeline.hpp"

struct Renderer {
#ifdef VALDIATION_LAYERS
    explicit Renderer(Window& w) : window(w), debug_messenger(instance), logical_device(physical_device),
            surface(window, instance), physical_device(instance, surface), swap_chain(window, logical_device, surface),
            image_views(swap_chain, logical_device), graphics_pipeline(logical_device, swap_chain, render_pass),
                                   render_pass(logical_device, swap_chain){}
#else
    explicit Renderer(Window& w) : window(w), logical_device(physical_device),
        surface(window, instance), physical_device(instance, surface) , swap_chain(window, logical_device, surface) ,
        image_views(swap_chain, logical_device), graphics_pipeline(logical_device, swap_chain, render_pass), render_pass(logical_device, swap_chain){}
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

    //The interface to the graphics card
    LogicalDevice logical_device;

    //The surface to render to --- currently the GLFW window
    Surface surface;

    //The frame buffers
    SwapChain swap_chain;

    //The views into the swapchain images (needed for rendering)
    ImageViews image_views;

    //the graphics pipeline --- how all the rendering gets done
    GraphicsPipeline graphics_pipeline;

    //render pass -- how the framebuffer is written to
    RenderPass render_pass;
};


#endif //VULKAN_ENGINE_RENDERER_HPP
