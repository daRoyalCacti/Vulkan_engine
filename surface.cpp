//
// Created by jacob on 3/11/21.
//

#include "surface.hpp"
#include <GLFW/glfw3.h>
#include <stdexcept>

void Surface::setup() {
    const auto did_create_surface = glfwCreateWindowSurface(instance.get_instance(), window.get_window(), nullptr, &surface);
    if (did_create_surface != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface");
    }
}

void Surface::cleanup() const {
    vkDestroySurfaceKHR(instance.get_instance(), surface, nullptr);    //simply destroying the surface
}
