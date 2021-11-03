//
// Created by jacob on 3/11/21.
//

#ifndef VULKAN_ENGINE_LOGICAL_DEVICE_HPP
#define VULKAN_ENGINE_LOGICAL_DEVICE_HPP

#include <vulkan/vulkan.h>
#include "physical_device.hpp"

//logical device are used to interface with physical devices
//this struct also holds the queues that interface with the physical device
struct LogicalDevice {
    VkDevice device{};
    PhysicalDevice& physical_device;

    VkQueue graphics_queue{};   //handle to the graphics queue --- used to request graphics based commands

    explicit LogicalDevice(PhysicalDevice & pd) : physical_device(pd) {}

    void setup();
    void cleanup() const;
};


#endif //VULKAN_ENGINE_LOGICAL_DEVICE_HPP
