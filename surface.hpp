//
// Created by jacob on 3/11/21.
//

#ifndef VULKAN_ENGINE_SURFACE_HPP
#define VULKAN_ENGINE_SURFACE_HPP

#include <vulkan/vulkan.h>
#include "instance.hpp"
#include "window.hpp"

struct Surface {
    Instance &instance;
    Window &window;

    Surface(Window &w, Instance &i) : window(w), instance(i) {};
    void setup();
    void cleanup() const;
    VkSurfaceKHR& get_surface() {return surface;}

    VkSurfaceKHR surface{};
};


#endif //VULKAN_ENGINE_SURFACE_HPP
