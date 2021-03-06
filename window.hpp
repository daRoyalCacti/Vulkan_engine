//
// Created by jacob on 1/11/21.
//

#ifndef VULKAN_ENGINE_WINDOW_HPP
#define VULKAN_ENGINE_WINDOW_HPP

#include <GLFW/glfw3.h>

struct Window {
    GLFWwindow* window{}; //object to hold the window and a context
    const int window_width = 800;   //values are ints because thats what GLFW expects them as
    const int window_height = 600;
    const char* window_name = "Vulkan";

    void makeWindow();
    void cleanup() const;
    [[nodiscard]] inline GLFWwindow* const& get_window() const {
        return window;
    }

    bool window_resized = false;  //if the window was just resized (is needed for rendering)
};

void framebufferResizeCallback(GLFWwindow* window, int width, int height);


#endif //VULKAN_ENGINE_WINDOW_HPP
