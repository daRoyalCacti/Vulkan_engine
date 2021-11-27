//
// Created by jacob on 27/11/21.
//

#include "buffer.hpp"
#include <stdexcept>


//need to match the memory requirements of the buffer with those offered by the graphics cards
//to find the right type of memory to use
//
//The typeFilter parameter will be used to specify the bit field of memory types that are suitable
unsigned findMemoryType(PhysicalDevice &physisical_device, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    //finding the memory offered by the graphics card
    VkPhysicalDeviceMemoryProperties memProperties;     //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkPhysicalDeviceMemoryProperties.html
    vkGetPhysicalDeviceMemoryProperties(physisical_device.get_device(), &memProperties);
    //The VkPhysicalDeviceMemoryProperties structure has two arrays memoryTypes and memoryHeaps.
    //- Memory heaps are distinct memory resources like dedicated VRAM and swap space in RAM for when VRAM runs out.
    // - The different types of memory exist within these heaps.
    //    > Right now we'll only concern ourselves with the type of memory and not the heap it comes from, but you can imagine that this can affect performance.

    //finding a suitable memory type for the buffer
    for (unsigned i = 0; i < memProperties.memoryTypeCount; i++) {
        //looping through all memory types supported and checking if the memory type is suitable
        // - because typeFilter is a bit field this means checking if the bit is 1
        const auto mem_type_suitable = typeFilter & (1 << i);
        //checking if we can write the vertex data into the memory
        // - the memoryTypes array specifies properties of each memory type
        // - these properties contain the ability to be written to from the CPU
        //because we can more than 1 required property, we should not just check if the bitwise and is not 0
        // - we need to compare it to properties to make sure we got both of them
        // - (not needed for mem_type_suitable because we only need one element in the bitmap, here we need all elements in the bitmap)
        const auto mem_has_prop = (memProperties.memoryTypes[i].propertyFlags & properties) == properties;
        //checking if the memory has the desired property
        if ( mem_type_suitable &&  mem_has_prop) {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}


//size is size in bytes
void create_buffer(LogicalDevice &device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
    VkBufferCreateInfo bufferInfo{};                            //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkBufferCreateInfo.html
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;    //sType must be VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO
    bufferInfo.size = size;                                     //the size in bytes of the buffer to create
    bufferInfo.usage = usage;                                   //bitmask of the allowed usage of the buffer
    // - https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkBufferUsageFlagBits.html
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;         //how the buffer is shared between multiple queue families
    // - https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkSharingMode.html


    if (vkCreateBuffer(device.get_device(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }

    //allocating memory to the buffer
    //===============================
    //first querying the memory requirements
    // - Graphics cards can offer different types of memory to allocate from. Each type of memory varies in terms of allowed operations and performance characteristics
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device.get_device(), buffer, &memRequirements);
    //The VkMemoryRequirements struct has three fields:
    //    size: The size of the required amount of memory in bytes, may differ from bufferInfo.size.
    //    alignment: The offset in bytes where the buffer begins in the allocated region of memory, depends on bufferInfo.usage and bufferInfo.flags.
    //    memoryTypeBits: Bit field of the memory types that are suitable for the buffer.

    //actually allocating the memory
    VkMemoryAllocateInfo allocInfo{};                               //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkMemoryAllocateInfo.html
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;       //sType must be VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO
    allocInfo.allocationSize = memRequirements.size;                //the size of the allocation in bytes
    allocInfo.memoryTypeIndex = findMemoryType(device.physical_device, memRequirements.memoryTypeBits, properties); //index of the memory type
    //using the find memory type helper function to find the index of the best memory type

    if (vkAllocateMemory(device.get_device(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    //associating the memory with the buffer
    vkBindBufferMemory(device.get_device(), buffer, bufferMemory, 0);
    //the fourth parameter is the offset within the region of memory.
    //Since this memory is allocated specifically for this the vertex buffer, the offset is simply 0.
    // - If the offset is non-zero, then it is required to be divisible by memRequirements.alignment.
}

