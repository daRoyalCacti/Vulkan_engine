//
// Created by jacob on 19/11/21.
//

#ifndef VULKAN_ENGINE_COMMAND_POOL_HPP
#define VULKAN_ENGINE_COMMAND_POOL_HPP

#include <vulkan/vulkan.h>
#include "logical_device.hpp"
#include "queue_family.hpp"

//command pools manage the memory that is used to store command buffers
struct CommandPool {
    VkCommandPool command_pool;

    CommandPool(LogicalDevice &d, QueueFamily &q) : device(d), queue_family(q) {}

    void setup();
    void cleanup();

private:
    LogicalDevice &device;
    QueueFamily &queue_family;
};


#endif //VULKAN_ENGINE_COMMAND_POOL_HPP
