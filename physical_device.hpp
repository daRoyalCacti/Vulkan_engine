//
// Created by jacob on 3/11/21.
//

#ifndef VULKAN_ENGINE_PHYSICAL_DEVICE_HPP
#define VULKAN_ENGINE_PHYSICAL_DEVICE_HPP

#include "instance.hpp"


struct PhysicalDevice {
    explicit PhysicalDevice(Instance &i) : instance(i) {}
    void setup();

    //check the suitability of the graphics cards
    // - need to check that they meet the requirements for the program
    // - (there are more requirements than those just set in the instance)
    static unsigned rateDeviceSuitability(VkPhysicalDevice device);
    static bool isDeviceSuitable(VkPhysicalDevice device);
    VkPhysicalDevice& get_device() {return physicalDevice;}

private:
    Instance & instance;
    //the handle to graphics card
    // - setting to NULL to prevent undefined behaviour
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;    //this is implicitly destroyed when the instance is destroyed
};


#endif //VULKAN_ENGINE_PHYSICAL_DEVICE_HPP
