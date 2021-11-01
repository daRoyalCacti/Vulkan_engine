//
// Created by jacob on 31/10/21.
//

#ifndef VULKAN_ENGINE_RENDERING_HPP
#define VULKAN_ENGINE_RENDERING_HPP

#include <GLFW/glfw3.h>
#include <array>
#include <vector>

#ifndef NDEBUG
#define VALDIATION_LAYERS
#endif

#ifdef VALDIATION_LAYERS
//a function to load and call the vkCreateDebugUtilsMessengerEXT
// because this function comes from an extension, it is not loaded automatically
VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

//a function to load and call the vkDestroyDebugUtilsMessengerEXT
// because this function comes from an extension, it is not loaded automatically
void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
#endif

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
    const unsigned vulkan_version = VK_API_VERSION_1_0; //the highest version of vulkan that the application is designed to use

    //validation layers
#ifdef VALDIATION_LAYERS
    static constexpr std::array<const char*, 1> validation_layers = {
            "VK_LAYER_KHRONOS_validation"
    };

    //validation layer custom logger
    VkDebugUtilsMessengerEXT debugMessenger;    //object that handles passing along debug messages to a callback
    void setupDebugMessenger();

    //custom callback to use when a validation layers wants to log sometihing
    //VKAPI_ATTR and VKAPI_CALL ensure that the function has the right for Vulkan to call it
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

    //the settings used in debugCallback
    static void getDebugCallbackSettings(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

    //checking that all requested validation layers are available
    bool checkValidationLayerSupport();
#endif


    //An instance is created by describing your application and any API extensions you will be using
    void createInstance();

    //check that extensions requested are indeed supported
    void checkExtensionSupport(const std::vector<const char*> &requried_extensions);

    //getting the required extensions
    // currently those needed by GLFW and by validation layers
    static std::vector<const char*> getRequiredExtensions();


};


#endif //VULKAN_ENGINE_RENDERING_HPP
