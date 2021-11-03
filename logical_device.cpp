//
// Created by jacob on 3/11/21.
//

#include "logical_device.hpp"
#include "queue_family.hpp"
#include <stdexcept>

void LogicalDevice::setup() {
    //first getting all the queues indices
    QueueFamily queue_family;
    queue_family.findQueueFamilies(physical_device.get_device());

    float queue_priority = 1.0f; //the priority of queues to influence scheduling (not really needed since we only have 1 queue)

    //generating the structure to hold all the queues we want
    VkDeviceQueueCreateInfo queue_create_info{};
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO; //sType must be VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO
    queue_create_info.queueFamilyIndex = queue_family.graphicsFamily.value(); //the index of the queue family to create the queue on
    queue_create_info.queueCount = 1; //the number of queses to create
    queue_create_info.pQueuePriorities = &queue_priority;   //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/html/vkspec.html#devsandqueues-priority


    //setting the device features that we'll be needing (e.g. geometry shader)
    // - don't required anything special right now so just leaving this blank
    VkPhysicalDeviceFeatures required_device_features{};

    //actually creating the logical device
    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;    //sType must be VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO
    createInfo.pQueueCreateInfos = &queue_create_info;  //array describing the queues that are to be created
    createInfo.queueCreateInfoCount = 1;    //the size of the pQueueCreateInfos array
    createInfo.pEnabledFeatures = &required_device_features;    //contains all of the features to be enabled
    createInfo.enabledExtensionCount = 0;   //the number of device extensions to enable. not enabling any device extensions
    createInfo.pEnabledFeatures = nullptr; //structure containing the device features to enable -- https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkPhysicalDeviceFeatures.html


    //actually create the logical device
    const auto did_create_device = vkCreateDevice(physical_device.get_device(), &createInfo, nullptr, &device);
    if (did_create_device != VK_SUCCESS) {
        throw std::runtime_error("Failed to create logical device");
    }

    //create the handle to the queues requested
    vkGetDeviceQueue(device, queue_family.graphicsFamily.value(), 0, &graphics_queue);
}

void LogicalDevice::cleanup() const {
    vkDestroyDevice(device, nullptr);
}
