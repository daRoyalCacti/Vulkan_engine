//
// Created by jacob on 3/11/21.
//

#ifndef VULKAN_ENGINE_QUEUE_FAMILY_HPP
#define VULKAN_ENGINE_QUEUE_FAMILY_HPP

#include <optional>
#include <vulkan/vulkan.h>

//Queues are used to issue commands to vulkan
//This struct holds the queues required by the program
// - This does not hold the actual queues
struct QueueFamily {
    //value to store the index of the queue_family that corresponds to graphics tasks
    //it is std::optional because we need some way to encode if the queue family cannot be found
    // - cannot just set the value to -1 (or something) because the index can be any unsigned value
    std::optional<unsigned> graphicsFamily;
    //storing the index of the queue used for presentation
    std::optional<unsigned> presentFamily;

    QueueFamily(VkPhysicalDevice &d, VkSurfaceKHR &s) : device(d), surface(s) {}

    void setup();

    [[nodiscard]] bool has_graphics() const {return graphicsFamily.has_value();}
    [[nodiscard]] bool has_present() const {return presentFamily.has_value();}

private:
    VkPhysicalDevice& device;
    VkSurfaceKHR& surface;
};


#endif //VULKAN_ENGINE_QUEUE_FAMILY_HPP
