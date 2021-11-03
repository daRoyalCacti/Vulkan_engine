//
// Created by jacob on 3/11/21.
//

#include "queue_faimily.hpp"

#include <vector>

void QueueFaimily::findQueueFamilies(VkPhysicalDevice device) {
    //finding the queues
    uint32_t queue_family_count = 0;    //the number of queues available
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr); //querying the number of queues

    std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);    //array of all queues
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());   //querying all the queues

    //recording the queue that can be used for graphics
    for (unsigned i = 0; i < queue_family_count; i++) {
        //if the queue can be used for graphics
        // - see https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkQueueFlagBits.html for other possibilities
        if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            graphicsFamily = i;
            break;
        }
    }

}