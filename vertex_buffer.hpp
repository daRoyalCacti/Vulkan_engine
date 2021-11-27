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
#include "buffer.hpp"



template <typename T>
struct VertexBuffer {
    VkBuffer vertexBuffer{};
    VkDeviceMemory vertexBufferMemory{};    //a handle to the memory


    VertexBuffer(LogicalDevice &d, std::vector<T>& v) : device(d), vertices(v) {}

    [[nodiscard]] VkBuffer& get_buffer() {return vertexBuffer;}

    void setup() {
        //creating the vertex buffer
        //==========================
        const VkDeviceSize buffer_size = sizeof(T) * vertices.size();
        create_buffer(device, buffer_size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vertexBuffer, vertexBufferMemory);


        //filling the vertex buffer
        //=========================
        //need to map the buffer memory into something that is CPU accessible
        void* data;
        vkMapMemory(device.get_device(), vertexBufferMemory, 0, buffer_size, 0, &data);
        //This function allows us to access a region of the specified memory resource defined by an offset and size
        //The last parameter specifies the output for the pointer to the mapped memory.

        //simply memcpying the memory
        memcpy(data, vertices.data(), buffer_size);
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






    std::vector<T>& vertices;
private:
    LogicalDevice &device;

};






#endif //VULKAN_ENGINE_VERTEX_BUFFER_HPP
