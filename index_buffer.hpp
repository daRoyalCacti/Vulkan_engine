//
// Created by jacob on 27/11/21.
//

#ifndef VULKAN_ENGINE_INDEX_BUFFER_HPP
#define VULKAN_ENGINE_INDEX_BUFFER_HPP


#include <vulkan/vulkan.h>
#include <vector>
#include <stdexcept>
#include "logical_device.hpp"
#include "vertex.hpp"
#include <cstring>  //for memcpy
#include "buffer.hpp"



template <typename T>
struct IndexBuffer {
    VkBuffer indexBuffer{};
    VkDeviceMemory indexBufferMemory{};    //a handle to the memory


    IndexBuffer(LogicalDevice &d, CommandPool& c, std::vector<T>& i) : device(d), command_pool(c), indices(i) {}

    [[nodiscard]] VkBuffer& get_buffer() {return indexBuffer;}

    void setup() {
        const VkDeviceSize buffer_size = sizeof(T) * indices.size();
        //Creating the staging buffer
        // - The most optimal memory has the VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT flag and is usually not accessible by the CPU on dedicated graphics cards.
        // - So we create two vertex buffers.
        // - One staging buffer in CPU accessible memory to upload the data from the vertex array to, and the final vertex buffer in device local memory.
        // - We'll then use a buffer copy command to move the data from the staging buffer to the actual vertex buffer.
        //=================================================================================================================================================
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        create_buffer(device, buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);


        //filling the staging buffer
        //=========================
        //need to map the buffer memory into something that is CPU accessible
        void* data;
        vkMapMemory(device.get_device(), stagingBufferMemory, 0, buffer_size, 0, &data);
        //This function allows us to access a region of the specified memory resource defined by an offset and size
        //The last parameter specifies the output for the pointer to the mapped memory.

        //simply memcpying the memory
        memcpy(data, indices.data(), buffer_size);
        //then unmapping the memory
        vkUnmapMemory(device.get_device(), stagingBufferMemory);
        //Unfortunately the driver may not immediately copy the data into the buffer memory, for example because of caching.
        // It is also possible that writes to the buffer are not visible in the mapped memory yet. There are two ways to deal with that problem:
        //    Use a memory heap that is host coherent, indicated with VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        //    Call vkFlushMappedMemoryRanges after writing to the mapped memory, and call vkInvalidateMappedMemoryRanges before reading from the mapped memory
        //We went for the first approach
        //this may lead to slightly worse performance that explicit flushing (will be fixing this soon)
        //The transfer of data to the GPU is an operation that happens in the background and the specification simply tells us that it is guaranteed to be complete as of the next call to vkQueueSubmit.


        //creating the index buffer
        //==========================
        //using create_buffer is slightly optimal as there is a maximum of simultaneous memory allocations
        // - this is given by thw maxMemoryAllocationCount physical device limit (on high-end graphics cards it can be 4096)
        // - should create bigger buffers and use offsets to specify the individual buffers
        // - could use https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator
        create_buffer(device, buffer_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);


        //copying data from the staging buffer to the vertex buffer
        // - note this is not the optimal way to do this if there are multiple copies that need to happen (see copy buffer comments)
        //==========================================================================================================================
        copyBuffer(device, command_pool, stagingBuffer, indexBuffer, buffer_size);

        //destroying the staging buffer (it is no longer of use -- the data has been copied from it)
        vkDestroyBuffer(device.get_device(), stagingBuffer, nullptr);
        vkFreeMemory(device.get_device(), stagingBufferMemory, nullptr);


    }

    void cleanup() {
        vkDestroyBuffer(device.get_device(), indexBuffer, nullptr);
        vkFreeMemory(device.get_device(), indexBufferMemory, nullptr);
    }






    std::vector<T>& indices;
private:
    LogicalDevice &device;
    CommandPool &command_pool;

};




#endif //VULKAN_ENGINE_INDEX_BUFFER_HPP
