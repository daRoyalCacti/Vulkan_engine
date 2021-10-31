//
// Created by jacob on 31/10/21.
//

#ifndef VULKAN_ENGINE_RENDERING_HPP
#define VULKAN_ENGINE_RENDERING_HPP

#include <GLFW/glfw3.h>

struct rendering {
    void initWindow();
    void initVulkan() {}
    void mainLoop();
    void cleanup();

private:
    //variables for the window
    GLFWwindow* window; //object to hold the window and a context
    const int window_width = 800;   //values are ints because thats what GLFW expects them as
    const int window_height = 600;
    const char* window_name = "Vulkan";
};


#endif //VULKAN_ENGINE_RENDERING_HPP
