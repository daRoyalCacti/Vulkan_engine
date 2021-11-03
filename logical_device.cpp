//
// Created by jacob on 3/11/21.
//

#include "logical_device.hpp"
#include "queue_family.hpp"
#include <stdexcept>
#include <set>

void LogicalDevice::setup() {
    //first getting all the queues indices
    QueueFamily queue_family;
    queue_family.findQueueFamilies(physical_device.get_device(), physical_device.surface.get_surface());

    float queue_priority = 1.0f; //the priority of queues to influence scheduling (not really needed since we only have 1 queue)


    //the array to hold to creation info for all the queues we need
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfo;

    //the queue families to create queues for
    //can only create queues for unique queue indices (hence the std:set)
    std::set<unsigned> uniqueQueueFamilies = {queue_family.graphicsFamily.value(), queue_family.presentFamily.value()};

    //generating the structure to hold all the queues we want
    // - the queues we want is specified in uniqueQueueFamilies
    VkDeviceQueueCreateInfo qCreateInfo{};
    for (auto & queueFamily : uniqueQueueFamilies) {
        qCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO; //sType must be VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO
        qCreateInfo.queueFamilyIndex = queueFamily; //the index of the queue family to create the queue on
        qCreateInfo.queueCount = 1; //the number of queses to create
        qCreateInfo.pQueuePriorities = &queue_priority;   //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/html/vkspec.html#devsandqueues-priority
        queueCreateInfo.push_back(qCreateInfo);
    }


    //setting the device features that we'll be needing (e.g. geometry shader)
    // - don't required anything special right now so just leaving this blank
    VkPhysicalDeviceFeatures required_device_features{};


    //actually creating the logical device
    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;    //sType must be VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO
    createInfo.pQueueCreateInfos = queueCreateInfo.data();  //array describing the queues that are to be created
    createInfo.queueCreateInfoCount = queueCreateInfo.size();    //the size of the pQueueCreateInfos array
    createInfo.pEnabledFeatures = &required_device_features;    //contains all of the features to be enabled
    createInfo.enabledExtensionCount = PhysicalDevice::deviceExtensions.size();   //the number of device extensions to enable
    createInfo.ppEnabledExtensionNames = PhysicalDevice::deviceExtensions.data(); //the names of the extensions to enable -- https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkPhysicalDeviceFeatures.html


    //actually create the logical device
    const auto did_create_device = vkCreateDevice(physical_device.get_device(), &createInfo, nullptr, &device);
    if (did_create_device != VK_SUCCESS) {
        throw std::runtime_error("Failed to create logical device");
    }

    //create the handle to the queues requested
    vkGetDeviceQueue(device, queue_family.graphicsFamily.value(), 0, &graphics_queue);
    vkGetDeviceQueue(device, queue_family.presentFamily.value(), 0, &present_queue);
}

void LogicalDevice::cleanup() const {
    vkDestroyDevice(device, nullptr);
}
