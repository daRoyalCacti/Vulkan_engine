//
// Created by jacob on 3/11/21.
//

#ifndef VULKAN_ENGINE_PHYSICAL_DEVICE_HPP
#define VULKAN_ENGINE_PHYSICAL_DEVICE_HPP

#include "instance.hpp"
#include "surface.hpp"

struct PhysicalDevice {
    explicit PhysicalDevice(Instance &i, Surface &s) : instance(i), surface(s) {}
    void setup();

    //device extensions required
    // - https://www.khronos.org/registry/vulkan/ has a list of extensions
    static constexpr std::array<const char*, 1> deviceExtensions = {"VK_KHR_swapchain"};

    //check the suitability of the graphics cards
    // - need to check that they meet the requirements for the program
    // - (there are more requirements than those just set in the instance)
    unsigned rateDeviceSuitability(VkPhysicalDevice device);
    bool isDeviceSuitable(VkPhysicalDevice device);
    static bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    VkPhysicalDevice& get_device() {return physicalDevice;}

    Surface & surface;
private:
    Instance & instance;

    //the handle to graphics card
    // - setting to NULL to prevent undefined behaviour
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;    //this is implicitly destroyed when the instance is destroyed
};


#endif //VULKAN_ENGINE_PHYSICAL_DEVICE_HPP
