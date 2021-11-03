//
// Created by jacob on 3/11/21.
//

#ifndef VULKAN_ENGINE_QUEUE_FAIMILY_HPP
#define VULKAN_ENGINE_QUEUE_FAIMILY_HPP

#include <optional>
#include <vulkan/vulkan.h>

//Queues are used to issue commands to vulkan
//This struct holds the queues required by the program
struct QueueFaimily {
    //value to store the index of the queue_family that corresponds to graphics tasks
    //it is std::optional because we need some way to encode if the queue family cannot be found
    // - cannot just set the value to -1 (or something) because the index can be any unsigned value
    std::optional<unsigned> graphicsFamily;

    void findQueueFamilies(VkPhysicalDevice device);

    [[nodiscard]] bool has_graphics() const {return graphicsFamily.has_value();}

};


#endif //VULKAN_ENGINE_QUEUE_FAIMILY_HPP
