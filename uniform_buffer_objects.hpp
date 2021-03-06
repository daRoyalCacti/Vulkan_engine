//
// Created by jacob on 28/11/21.
//

#ifndef VULKAN_ENGINE_UNIFORM_BUFFER_OBJECTS_HPP
#define VULKAN_ENGINE_UNIFORM_BUFFER_OBJECTS_HPP

#include <glm/glm.hpp>
#include <vector>
#include <vulkan/vulkan.h>

#include "logical_device.hpp"
#include "swap_chain.hpp"

namespace UBO {
    struct mvp {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    };
}

//the buffer that holds the data for the shaders
struct UniformBufferObject {
    //We should have multiple buffers, because multiple frames may be in flight at the same time,
    //and we don't want to update the buffer in preparation of the next frame while a previous one is still reading from it!
    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;

    [[nodiscard]] std::vector<VkBuffer>& get_buffers() {return uniformBuffers;}

    UniformBufferObject(LogicalDevice& d, SwapChain &s) : device(d), swap_chain(s){}

    void setup();
    void cleanup();
    virtual void update(unsigned image_index) {}


protected:
    LogicalDevice &device;
    SwapChain &swap_chain;
};

//rotating
struct UniformBufferObject1 : public UniformBufferObject{
    UniformBufferObject1(LogicalDevice& d, SwapChain &s) : UniformBufferObject(d,s) {}
    void update(unsigned image_index) override;
};

//rotating but about a different axis
struct UniformBufferObject2 : public UniformBufferObject{
    UniformBufferObject2(LogicalDevice& d, SwapChain &s) : UniformBufferObject(d,s) {}
    void update(unsigned image_index) override;
};

//rotating but the last axis
struct UniformBufferObject3 : public UniformBufferObject{
    UniformBufferObject3(LogicalDevice& d, SwapChain &s) : UniformBufferObject(d,s) {}
    void update(unsigned image_index) override;
};

#endif //VULKAN_ENGINE_UNIFORM_BUFFER_OBJECTS_HPP
