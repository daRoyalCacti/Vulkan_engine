//
// Created by jacob on 28/11/21.
//

#ifndef VULKAN_ENGINE_DESCRIPTOR_POOL_HPP
#define VULKAN_ENGINE_DESCRIPTOR_POOL_HPP

#include <vulkan/vulkan.h>
#include "logical_device.hpp"
#include "swap_chain.hpp"
#include <stdexcept>

//holds the memory for the descriptor sets
struct DescriptorPool {
    VkDescriptorPool descriptorPool{};

    [[nodiscard]] VkDescriptorPool& get_pool() {return descriptorPool;}

    DescriptorPool(LogicalDevice &d, SwapChain &s) : device(d), swap_chain(s) {}

    virtual void setup() {}
    void cleanup() {vkDestroyDescriptorPool(device.get_device(), descriptorPool, nullptr);}

protected:
    LogicalDevice& device;
    SwapChain& swap_chain;
};


//holds the memory for the descriptor sets
// - N indicates the number of descriptors that are going to be allocated from this pool
template<unsigned N>
struct DescriptorPool1 : public DescriptorPool {
    DescriptorPool1(LogicalDevice &d, SwapChain &s) : DescriptorPool(d,s) {}
    void setup() override;
};

//holds the memory for the descriptor sets
template<unsigned N>
struct DescriptorPool2 : public DescriptorPool {
    DescriptorPool2(LogicalDevice &d, SwapChain &s) : DescriptorPool(d,s) {}
    void setup() override;
};

template<unsigned N>
void DescriptorPool1<N>::setup() {
    //describe which descriptor types the descriptor sets are going to use
    VkDescriptorPoolSize poolSize{};                    //https://khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkDescriptorPoolSize.html
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;  //the type of the descriptor -- in this case it is for a uniform buffer
    poolSize.descriptorCount = N*swap_chain.swapChainImages.size();   //the number of descriptors to allocate (want one per image in the swap chain -- see descriptor_layout for why)

    //specifying information about the descriptor pool
    VkDescriptorPoolCreateInfo poolInfo{};                          //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkDescriptorPoolCreateInfo.html
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO; //sType must be VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO
    poolInfo.poolSizeCount = 1;                                     //the number of pools to create
    poolInfo.pPoolSizes = &poolSize;                                //array containing VkDescriptorPoolSizes
    poolInfo.maxSets = N*swap_chain.swapChainImages.size();           //the maximum number of descriptor sets that can be allocated from this pool
    // - we are only allocating 1 descriptor set for each swapchain image so this is just the number of images in the swapchain
    poolInfo.flags = 0;                                             //determines if individual descriptor sets can be freed or not
    // - no touching the descriptor set after creating it so this is just 0

    //creating the pool
    if (vkCreateDescriptorPool(device.get_device(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }

}


template<unsigned N>
void DescriptorPool2<N>::setup() {
    //describe which descriptor types the descriptor sets are going to use
    std::array<VkDescriptorPoolSize, 2> poolSizes{};                //https://khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkDescriptorPoolSize.html
    //the pool for the uniform buffers
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;               //the type of the descriptor -- in this case it is for a uniform buffer
    poolSizes[0].descriptorCount = N*swap_chain.swapChainImages.size();   //the number of descriptors to allocate (want one per image in the swap chain -- see descriptor_layout for why)
    //the pool for the image sampler
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = N*swap_chain.swapChainImages.size();


    //specifying information about the descriptor pool
    VkDescriptorPoolCreateInfo poolInfo{};                          //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkDescriptorPoolCreateInfo.html
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO; //sType must be VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO
    poolInfo.poolSizeCount = poolSizes.size();                      //the number of pools to create
    poolInfo.pPoolSizes = poolSizes.data();                         //array containing VkDescriptorPoolSizes
    poolInfo.maxSets = N*swap_chain.swapChainImages.size();           //the maximum number of descriptor sets that can be allocated from this pool (1 per each image in the swap chain)
    poolInfo.flags = 0;                                             //determines if individual descriptor sets can be freed or not
    // - no touching the descriptor set after creating it so this is just 0

    //creating the pool
    if (vkCreateDescriptorPool(device.get_device(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }

}


#endif //VULKAN_ENGINE_DESCRIPTOR_POOL_HPP
