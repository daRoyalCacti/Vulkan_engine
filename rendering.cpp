//
// Created by jacob on 31/10/21.
//

#include "rendering.hpp"
#include <stdexcept>
#include <iostream>
#include <vector>
#include <cstring>

void rendering::initWindow() {
    //initialize the GLFW library
    ////================================================================
    const auto glfw_init_err = glfwInit();
    //checking errors
    if (glfw_init_err == GLFW_FALSE) {
        throw std::runtime_error("could not initialze glfw");
    }

    //setting some properties of glfw
    //these will be set for the next call to glfwCreateWindow
    ////================================================================
    //need to call NO_API because GLFW was designed for openGL
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    //disabling the ability to resize the window for now
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);


    //creating the window
    ////================================================================
    //window, window_width, window_height, and window_name are all defined iin the main struct
    //that last nullptr is in reference to openGL
    //the second last nullptr refers to the a monitor. It is GLFWmonitor*.
    // - see https://www.glfw.org/docs/latest/monitor_guide.html#monitor_object
    window = glfwCreateWindow(window_width, window_height, window_name, nullptr, nullptr);
}





void rendering::mainLoop() {
    //having the main loop run units the (x) is pressed
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();   //checking for events (like (x) being pressed)
    }
}

void rendering::cleanup() {
    //closing the instance
    vkDestroyInstance(instance, nullptr);   //nullptr is for custom memory allocation

    //closing the window
    ////================================================================
    //destroying the window and its context.
    glfwDestroyWindow(window);
    //destroys all remaining windows and cursors, restores any modified gamma ramps,
    //frees any allocated resources created by GLFW
    glfwTerminate();
}


void rendering::createInstance() {
    //filling a struct with some information about out application
    VkApplicationInfo appInfo{};    //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkApplicationInfo.html
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO; //sType must be VK_STRUCTURE_TYPE_APPLICATION_INFO
    appInfo.pApplicationName = application_name;
    appInfo.applicationVersion = application_version;
    appInfo.pEngineName = engine_name;
    appInfo.engineVersion = engine_version;
    appInfo.apiVersion = vulkan_version;


    //getting the global extensions needed
    // (global means for the entire program)
    //first getting the required extensions for GLFW to work
    unsigned glfw_extension_count = 0;
    const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

    //checking if all extensions requested are available
    ////===================================================================
    unsigned extension_count = 0;   //value to hold the number of supported extensions
    //querying the number of supported extensions
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);

    std::vector<VkExtensionProperties> extensions(extension_count); //array to store all the supported extensions
    //querying all the supported extensions
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());

    bool have_all_extensions = true;
    bool curr_extension_supported;
    //iterating throw all required extensions and seeing if they're in the list of supported extensions
    for (unsigned i = 0; i < glfw_extension_count; i++) {
        const char* curr_extension = glfw_extensions[i];    //the extension to see if supported
        curr_extension_supported = false;
        for (const auto &ex : extensions) {
            if (std::strcmp(curr_extension, ex.extensionName) == 0) {   //if the extension is in the list of supported extensions
                curr_extension_supported = true;
                break;
            }
        }

        if (!curr_extension_supported) {
            have_all_extensions = false;
            std::cerr << "do not have extension " << curr_extension << "\n";
        }



    }

    if (!have_all_extensions) {
        throw std::runtime_error("requested extensions not supported");
    }



    ////===================================================================

    //telling the vulkan driver which global extensions and validation layers we want to use.
    //https://www.khronos.org/registry/vulkan/ has a list of all extensions
    VkInstanceCreateInfo createInfo{};  //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkInstanceCreateInfo.html
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;  //sType must be VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO
    createInfo.pApplicationInfo = &appInfo;
    //extensions
    createInfo.enabledExtensionCount = glfw_extension_count;    //number of global extensions to enable
    createInfo.ppEnabledExtensionNames = glfw_extensions;       //pointer to an array of strings containing the names of extensions to enable
    //validation layers
    createInfo.enabledLayerCount = 0;       //number of global layers to enable (validation layers)


    //actually creating the instance
    //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateInstance.html
    const auto create_instance_err = vkCreateInstance(&createInfo, nullptr, &instance); //nullptr is for custom memory allocation
    if (create_instance_err != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance");
    }

}


void rendering::initVulkan() {
    createInstance();
}