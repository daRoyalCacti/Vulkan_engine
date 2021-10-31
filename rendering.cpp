//
// Created by jacob on 31/10/21.
//

#include "rendering.hpp"
#include <stdexcept>
#include <iostream>
#include <cstring>

#ifndef NDEBUG
VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"); //loading the function
    if (func != nullptr) {  //if the function is loaded, run it
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {    //else error
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"); //loading the function
    if (func != nullptr) { //if the function is loaded, run it
        func(instance, debugMessenger, pAllocator);
    }
}
#endif


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


void rendering::checkExtensionSupport(const std::vector<const char*> &requried_extensions) {
    unsigned extension_count = 0;   //value to hold the number of supported extensions
    //querying the number of supported extensions
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);

    std::vector<VkExtensionProperties> extensions(extension_count); //array to store all the supported extensions
    //querying all the supported extensions
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());

    bool have_all_extensions = true;
    bool curr_extension_supported;
    //iterating throw all required extensions and seeing if they're in the list of supported extensions
    for (const auto & curr_extension : requried_extensions) {
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
}

#ifndef NDEBUG
bool rendering::checkValidationLayerSupport() {
    unsigned layer_count = 0;   //store to total number of validation layers available
    //querying the number of validation layers
    vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

    std::vector<VkLayerProperties> available_layers(layer_count);   //stores all validation layers
    //querying all validation layers
    vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());


    //iterating throw all required extensions and seeing if they're in the list of supported extensions
    bool layer_found;
    for (const char* layerName : validation_layers) {   //validation_layers defined in main struct
        layer_found = false;

        for (const auto& layerProperties : available_layers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {//if the layer is in the list of available layers
                layer_found = true;
                break;
            }
        }

        if (!layer_found) {
            return false;
        }
    }

    return true;
}
#endif

std::vector<const char*> rendering::getRequiredExtensions() {
    //first getting the required extensions for GLFW to work
    unsigned glfw_extension_count = 0;
    const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

    //putting the glw extensions into a vector to return
    std::vector<const char*> extensions(glfw_extensions, glfw_extensions + glfw_extension_count);

    if constexpr (enable_validaion_layers) {
        //extensions needed for validation layers

        //we need VK_EXT_debug_utils to use the custom debug messengers
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);    //VK_EXT_DEBUG_UTILS_EXTENSION_NAME is a macro for "VK_EXT_debug_utils". https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VK_EXT_debug_utils.html
    }

    return extensions;
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
    const auto extensions = getRequiredExtensions();


    //checking if all extensions requested are available
    checkExtensionSupport(extensions);

    //checking if requested validation layers are supported
    if constexpr (enable_validaion_layers) {
#ifndef NDEBUG
        const bool validation_layers_supported = checkValidationLayerSupport();
        if (!validation_layers_supported) {
            throw std::runtime_error("the validation layers requested are not supported");
        }
#endif
    }



    //telling the vulkan driver which global extensions and validation layers we want to use.
    //https://www.khronos.org/registry/vulkan/ has a list of all extensions
    VkInstanceCreateInfo createInfo{};  //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkInstanceCreateInfo.html
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;  //sType must be VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO
    createInfo.pApplicationInfo = &appInfo;
    //extensions
    createInfo.enabledExtensionCount = extensions.size();    //number of global extensions to enable
    createInfo.ppEnabledExtensionNames = extensions.data();       //pointer to an array of strings containing the names of extensions to enable
    //validation layers
    if constexpr (enable_validaion_layers) {
        #ifndef NDEBUG
        createInfo.enabledLayerCount = validation_layers.size();       //number of global layers to enable (validation layers)
        createInfo.ppEnabledLayerNames = validation_layers.data();      //the list of the validation layers to use

        //the regular validation debug caller requires an instance
        //as such it is created after the instance and destroyed before it
        //so now we pass the debug caller in here
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        getDebugCallbackSettings(debugCreateInfo);

        //actually setting the value in the struct
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
        #endif
    } else {
        createInfo.enabledLayerCount = 0;       //number of global layers to enable (validation layers)
        createInfo.pNext = nullptr;
    }




    //actually creating the instance
    //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateInstance.html
    const auto create_instance_err = vkCreateInstance(&createInfo, nullptr, &instance); //nullptr is for custom memory allocation
    if (create_instance_err != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance");
    }

}

#ifndef NDEBUG

VKAPI_ATTR VkBool32 VKAPI_CALL rendering::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
    //messageSeverity can take values:
    // - VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: Diagnostic message
    // - VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: Informational message like the creation of a resource
    // - VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: Message about behavior that is not necessarily an error, but very likely a bug in your application
    // - VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: Message about behavior that is invalid and may cause crashes
    //values are set up so comparisions work
    // e.g. messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT makes sense
    //
    //messageType takes values:
    //VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT: Some event has happened that is unrelated to the specification or performance
    //VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT: Something has happened that violates the specification or indicates a possible mistake
    //VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT: Potential non-optimal use of Vulkan
    //
    //pCallbackData takes values
    //pMessage: The debug message as a null-terminated string
    //pObjects: Array of Vulkan object handles related to the message
    //objectCount: Number of objects in array
    //
    //pUserData contains a pointer that was specified during the setup of the callback
    // - allows you to pass your own data to the callback
    //
    //
    //Function returns if the Vulkan call that triggered the validation layer should be aborted


    //just print the error to the error stream
    std::cerr << "validation layer: " << pCallbackData->pMessage << "\n";

    return VK_FALSE;
}

void rendering::getDebugCallbackSettings(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT; //sType must be VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT; //the severity events that will call the callback to trigger
    //currently all but VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;    //the type of events which will cause the callback to be triggered (all enabled)
    createInfo.pfnUserCallback = debugCallback; //the actuall callback function
    createInfo.pUserData = nullptr; //The pUserData field in the callback --- allows you to pass your own data to the callback
}

void rendering::setupDebugMessenger() {
    //filling a structure with details about the custom logger `debugCallback'
    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    getDebugCallbackSettings(createInfo);

    //a function to load and call vkCreateDebugUtilsMessengerEXT
    // because this function comes from an extension, it is not loaded automatically
    //https://khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateDebugUtilsMessengerEXT.html
    const auto can_create_messenger = CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger);
    if (can_create_messenger != VK_SUCCESS) {
        throw std::runtime_error("failed to set up debug messenger!");
    }
}
#endif

void rendering::initVulkan() {
    createInstance();
#ifndef NDEBUG
    setupDebugMessenger();
#endif
}







void rendering::mainLoop() {
    //having the main loop run units the (x) is pressed
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();   //checking for events (like (x) being pressed)
    }
}

void rendering::cleanup() {
    //destroying the debug messenger
#ifndef NDEBUG
    if constexpr (enable_validaion_layers) {
        DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
    }
#endif

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