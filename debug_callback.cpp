//
// Created by jacob on 1/11/21.
//

#ifdef VALDIATION_LAYERS

#include "debug_callback.hpp"
#include <iostream>

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


VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
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
    std::cerr << "validation layer: ";

    if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
        std::cerr << " verbose: ";
    } else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
        std::cerr << " info: ";
    } else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        std::cerr << " warning: ";
    } else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
        std::cerr << " error: ";
    } else {
        std::cerr << " unknown severity: ";
    }

    std::cerr << pCallbackData->pMessage << "\n";

    return VK_FALSE;
}

void getDebugCallbackSettings(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT; //sType must be VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT
    createInfo.messageSeverity =  VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT; //the severity events that will call the callback to trigger
    //currently all but VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT and VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;    //the type of events which will cause the callback to be triggered (all enabled)
    createInfo.pfnUserCallback = debugCallback; //the actuall callback function
    createInfo.pUserData = nullptr; //The pUserData field in the callback --- allows you to pass your own data to the callback
}

void DebugMessenger::setup(const Instance &i) {
    instance = &i;
    //filling a structure with details about the custom logger `debugCallback'
    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    getDebugCallbackSettings(createInfo);

    //a function to load and call vkCreateDebugUtilsMessengerEXT
    // because this function comes from an extension, it is not loaded automatically
    //https://khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateDebugUtilsMessengerEXT.html
    const auto can_create_messenger = CreateDebugUtilsMessengerEXT(instance->get_instance(), &createInfo, nullptr, &debugMessenger);
    if (can_create_messenger != VK_SUCCESS) {
        throw std::runtime_error("failed to set up debug messenger!");
    }
}
#endif
