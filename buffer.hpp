//
// Created by jacob on 27/11/21.
//

#ifndef VULKAN_ENGINE_BUFFER_HPP
#define VULKAN_ENGINE_BUFFER_HPP

#include <vulkan/vulkan.h>
#include "logical_device.hpp"
#include "command_pool.hpp"

//The typeFilter parameter will be used to specify the bit field of memory types that are suitable
unsigned findMemoryType(PhysicalDevice &physisical_device, uint32_t typeFilter, VkMemoryPropertyFlags properties) ;

//size is size in bytes
//last 2 parameters get written to
void create_buffer(LogicalDevice &device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

//helper function to copy data from one buffer to another
//command pool is to bool to allocate the copying commands from
void copyBuffer(LogicalDevice &device, CommandPool& command_pool, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);


#endif //VULKAN_ENGINE_BUFFER_HPP
