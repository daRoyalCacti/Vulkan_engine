//
// Created by jacob on 1/11/21.
//

#ifndef VULKAN_ENGINE_DEBUG_CALLBACK_HPP
#define VULKAN_ENGINE_DEBUG_CALLBACK_HPP
#ifdef VALDIATION_LAYERS


#include <vulkan/vulkan.h>
#include "instance.hpp"

//a function to load and call the vkCreateDebugUtilsMessengerEXT
// because this function comes from an extension, it is not loaded automatically
VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

//a function to load and call the vkDestroyDebugUtilsMessengerEXT
// because this function comes from an extension, it is not loaded automatically
void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

//custom callback to use when a validation layers wants to log sometihing
//VKAPI_ATTR and VKAPI_CALL ensure that the function has the right for Vulkan to call it
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

//the settings used in debugCallback
void getDebugCallbackSettings(VkDebugUtilsMessengerCreateInfoEXT& createInfo);



struct DebugMessenger {
    //validation layer custom logger
    VkDebugUtilsMessengerEXT debugMessenger{};    //object that handles passing along debug messages to a callback
    DebugMessenger(const Instance& i) : instance(i) {}
    void setup();
    inline void cleanup() const {DestroyDebugUtilsMessengerEXT(instance.get_instance(), debugMessenger, nullptr);}

private:
    const Instance& instance;
};


#endif
#endif //VULKAN_ENGINE_DEBUG_CALLBACK_HPP
