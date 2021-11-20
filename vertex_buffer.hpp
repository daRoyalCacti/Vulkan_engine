//
// Created by jacob on 20/11/21.
//

#ifndef VULKAN_ENGINE_VERTEX_BUFFER_HPP
#define VULKAN_ENGINE_VERTEX_BUFFER_HPP

#include <vulkan/vulkan.h>
#include <vector>
#include <stdexcept>
#include "logical_device.hpp"
#include "vertex.hpp"
#include <cstring>  //for memcpy




template <typename T>
struct VertexBuffer {
    VkBuffer vertexBuffer{};
    VkDeviceMemory vertexBufferMemory{};    //a handle to the memory


    VertexBuffer(LogicalDevice &d, std::vector<T>& v) : device(d), vertices(v) {}

    [[nodiscard]] VkBuffer& get_buffer() {return vertexBuffer;}

    void setup() {
        //creating the vertex buffer
        //==========================
        VkBufferCreateInfo bufferInfo{};                            //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkBufferCreateInfo.html
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;    //sType must be VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO
        bufferInfo.size = sizeof(T) * vertices.size();    //the size in bytes of the buffer to create
        bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;       //bitmask of the allowed usage of the buffer
                                                                    // - https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkBufferUsageFlagBits.html
                                                                    //only want it as a vertex buffer
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;         //how the buffer is shared between multiple queue families
                                                                    // - https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkSharingMode.html

        const auto buffer_create_result = vkCreateBuffer(device.get_device(), &bufferInfo, nullptr, &vertexBuffer);
        if (buffer_create_result != VK_SUCCESS) {
            throw std::runtime_error("failed to create vertex buffer");
        }

        //allocating memory to the buffer
        //===============================
        //first querying the memory requirements
        // - Graphics cards can offer different types of memory to allocate from. Each type of memory varies in terms of allowed operations and performance characteristics
        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(device.get_device(), vertexBuffer, &memRequirements);
        //The VkMemoryRequirements struct has three fields:
        //    size: The size of the required amount of memory in bytes, may differ from bufferInfo.size.
        //    alignment: The offset in bytes where the buffer begins in the allocated region of memory, depends on bufferInfo.usage and bufferInfo.flags.
        //    memoryTypeBits: Bit field of the memory types that are suitable for the buffer.

        //actally allocating the memory
        VkMemoryAllocateInfo allocInfo{};                   //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkMemoryAllocateInfo.html
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;   //sType must be VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO
        allocInfo.allocationSize = memRequirements.size;            //the size of the allocation in bytes
        allocInfo.memoryTypeIndex = findMemoryType(device.physical_device, memRequirements.memoryTypeBits,          //index of the memory type
                                                   VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT); //want the memory to be able to be written from the CPU
                                                                                                                                        //and to be able to be use coherently --- this ensures that the CPU always accesses the memory that is in the GPU

        const auto aloc_res = vkAllocateMemory(device.get_device(), &allocInfo, nullptr, &vertexBufferMemory);
        if (aloc_res != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate vertex buffer memory");
        }

        //associating the memory with the buffer
        vkBindBufferMemory(device.get_device(), vertexBuffer, vertexBufferMemory, 0);
        //the fourth parameter is the offset within the region of memory.
        //Since this memory is allocated specifically for this the vertex buffer, the offset is simply 0.
        // - If the offset is non-zero, then it is required to be divisible by memRequirements.alignment.


        //filling the vertex buffer
        //=========================
        //need to map the buffer memory into something that is CPU accessible
        void* data;
        vkMapMemory(device.get_device(), vertexBufferMemory, 0, bufferInfo.size, 0, &data);
        //This function allows us to access a region of the specified memory resource defined by an offset and size
        //The last parameter specifies the output for the pointer to the mapped memory.

        //simply memcpying the memory
        memcpy(data, vertices.data(), (size_t) bufferInfo.size);
        //then unmapping the memory
        vkUnmapMemory(device.get_device(), vertexBufferMemory);
        //Unfortunately the driver may not immediately copy the data into the buffer memory, for example because of caching.
        // It is also possible that writes to the buffer are not visible in the mapped memory yet. There are two ways to deal with that problem:
        //    Use a memory heap that is host coherent, indicated with VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        //    Call vkFlushMappedMemoryRanges after writing to the mapped memory, and call vkInvalidateMappedMemoryRanges before reading from the mapped memory
        //We went for the first approach
        //this may lead to slightly worse performance that explicit flushing (will be fixing this soon)
        //The transfer of data to the GPU is an operation that happens in the background and the specification simply tells us that it is guaranteed to be complete as of the next call to vkQueueSubmit.

    }

    void cleanup() {
        vkDestroyBuffer(device.get_device(), vertexBuffer, nullptr);
        vkFreeMemory(device.get_device(), vertexBufferMemory, nullptr);
    }


    //need to match the memory requirements of the buffer with those offered by the graphics cards
    //to find the right type of memory to use
    //
    //The typeFilter parameter will be used to specify the bit field of memory types that are suitable
    static unsigned findMemoryType(PhysicalDevice &physisical_device, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
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



    std::vector<T>& vertices;
private:
    LogicalDevice &device;

};






#endif //VULKAN_ENGINE_VERTEX_BUFFER_HPP
