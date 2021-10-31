//
// Created by jacob on 31/10/21.
//

#ifndef VULKAN_ENGINE_RENDERING_HPP
#define VULKAN_ENGINE_RENDERING_HPP

#include <GLFW/glfw3.h>

struct rendering {
    void initWindow();
    void initVulkan();
    void mainLoop();
    void cleanup();

private:
    //variables for the window
    GLFWwindow* window; //object to hold the window and a context
    const int window_width = 800;   //values are ints because thats what GLFW expects them as
    const int window_height = 600;
    const char* window_name = "Vulkan";

    //createInstance
    VkInstance instance;    //holds the handle to an instance
    const char* application_name = window_name;
    const unsigned application_version = VK_MAKE_VERSION(1, 0, 0);  //developer-supplied version number of the application
    const char* engine_name = "no_engine";  //the name of the engine used to create the application (its default is NULL)
    const unsigned engine_version = VK_MAKE_VERSION(1, 0, 0);   //the version number of the engine
    const unsigned vulkan_version = VK_API_VERSION_1_0; //the highest version of vulkan that the application is designed to sue

    //An instance is created by describing your application and any API extensions you will be using
    void createInstance();
};


#endif //VULKAN_ENGINE_RENDERING_HPP
