//
// Created by jacob on 3/11/21.
//

#ifndef VULKAN_ENGINE_LOGICAL_DEVICE_HPP
#define VULKAN_ENGINE_LOGICAL_DEVICE_HPP

#include <vulkan/vulkan.h>
#include "physical_device.hpp"
#include <array>
#include "queue_family.hpp"

//logical device are used to interface with physical devices
//this struct also holds the queues that interface with the physical device
struct LogicalDevice {
    VkDevice device{};
    PhysicalDevice& physical_device;

    VkQueue graphics_queue{};   //handle to the graphics queue --- used to request graphics based commands
    VkQueue present_queue{};   //handle to the present queue --- used to request presenting based commands

    //setting the device features that we'll be needing (e.g. geometry shader)
    // - https://www.khronos.org/registry/vulkan/specs/1.3-extensions/man/html/VkPhysicalDeviceFeatures.html
    // - This is not device extensions required. This is specified in physical_device.hpp
    VkPhysicalDeviceFeatures required_device_features{};

    explicit LogicalDevice(PhysicalDevice & pd, QueueFamily &q) : physical_device(pd), queue_family(q) {required_device_features.samplerAnisotropy = true;}
    [[nodiscard]] VkDevice get_device() const {return device;}

    void setup();
    void cleanup() const;

private:
    QueueFamily &queue_family;
};


#endif //VULKAN_ENGINE_LOGICAL_DEVICE_HPP
