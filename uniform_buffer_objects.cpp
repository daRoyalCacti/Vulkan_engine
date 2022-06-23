//
// Created by jacob on 28/11/21.
//

#include "uniform_buffer_objects.hpp"
#include "buffer.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <chrono>
#include <cstring>  //for memcpy

void UniformBufferObject::setup() {
    //need a buffer for every image that could be in flight (so buffer for every image in the swapchain)
    uniformBuffers.resize(swap_chain.swapChainImages.size());
    uniformBuffersMemory.resize(swap_chain.swapChainImages.size());

    //creating the buffers
    // - buffers are the size of the object they represent (the UBO)
    //no need for a staging buffer here since the data is updated regularly it likely won't give any performance boost
    for (size_t i = 0; i < swap_chain.swapChainImages.size(); i++) {
        create_buffer(device, sizeof(UBO::mvp), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
    }
}

void UniformBufferObject::cleanup() {
    for (size_t i = 0; i < swap_chain.swapChainImages.size(); i++) {
        vkDestroyBuffer(device.get_device(), uniformBuffers[i], nullptr);
        vkFreeMemory(device.get_device(), uniformBuffersMemory[i], nullptr);
    }
}

//just rotating the mesh
void UniformBufferObject1::update(unsigned int image_index) {
    //setting the desired rotation
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    UBO::mvp ubo{};
    ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(glm::radians(45.0f), static_cast<float>(swap_chain.extent.width) / static_cast<float>(swap_chain.extent.height), 0.1f, 10.0f);
    ubo.proj[1][1] *= -1;   //need to invert because glm aws original designed for openGL -- y-axis is flipped
                            //the (1,1) element represents the scaling in the y-direction (clearly) and so this has the desired effect

    //copying the data into the buffer
    // - again don't need a staging buffer because the data is changing so frequently
    // - in this case it is likely better to use push-constants anyway
    void* data;
    vkMapMemory(device.get_device(), uniformBuffersMemory[image_index], 0, sizeof(ubo), 0, &data);
    memcpy(data, &ubo, sizeof(ubo));
    vkUnmapMemory(device.get_device(), uniformBuffersMemory[image_index]);
}

//just rotating the mesh
void UniformBufferObject2::update(unsigned int image_index) {
    //setting the desired rotation
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    UBO::mvp ubo{};
    ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(glm::radians(45.0f), static_cast<float>(swap_chain.extent.width) / static_cast<float>(swap_chain.extent.height), 0.1f, 10.0f);
    ubo.proj[1][1] *= -1;   //need to invert because glm aws original designed for openGL -- y-axis is flipped
    //the (1,1) element represents the scaling in the y-direction (clearly) and so this has the desired effect

    //copying the data into the buffer
    // - again don't need a staging buffer because the data is changing so frequently
    // - in this case it is likely better to use push-constants anyway
    void* data;
    vkMapMemory(device.get_device(), uniformBuffersMemory[image_index], 0, sizeof(ubo), 0, &data);
    memcpy(data, &ubo, sizeof(ubo));
    vkUnmapMemory(device.get_device(), uniformBuffersMemory[image_index]);
}



//just rotating the mesh
void UniformBufferObject3::update(unsigned int image_index) {
    //setting the desired rotation
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    UBO::mvp ubo{};
    ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(glm::radians(45.0f), static_cast<float>(swap_chain.extent.width) / static_cast<float>(swap_chain.extent.height), 0.1f, 10.0f);
    ubo.proj[1][1] *= -1;   //need to invert because glm aws original designed for openGL -- y-axis is flipped
    //the (1,1) element represents the scaling in the y-direction (clearly) and so this has the desired effect

    //copying the data into the buffer
    // - again don't need a staging buffer because the data is changing so frequently
    // - in this case it is likely better to use push-constants anyway
    void* data;
    vkMapMemory(device.get_device(), uniformBuffersMemory[image_index], 0, sizeof(ubo), 0, &data);
    memcpy(data, &ubo, sizeof(ubo));
    vkUnmapMemory(device.get_device(), uniformBuffersMemory[image_index]);
}