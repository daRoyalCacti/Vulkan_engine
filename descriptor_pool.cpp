//
// Created by jacob on 28/11/21.
//

#include "descriptor_pool.hpp"
#include <stdexcept>

void DescriptorPool1::setup() {
    //describe which descriptor types the descriptor sets are going to use
    VkDescriptorPoolSize poolSize{};                    //https://khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkDescriptorPoolSize.html
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;  //the type of the descriptor -- in this case it is for a uniform buffer
    poolSize.descriptorCount = swap_chain.swapChainImages.size();   //the number of descriptors to allocate (want one per image in the swap chain -- see descriptor_layout for why)

    //specifying information about the descriptor pool
    VkDescriptorPoolCreateInfo poolInfo{};                          //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkDescriptorPoolCreateInfo.html
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO; //sType must be VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO
    poolInfo.poolSizeCount = 1;                                     //the number of pools to create
    poolInfo.pPoolSizes = &poolSize;                                //array containing VkDescriptorPoolSizes
    poolInfo.maxSets = swap_chain.swapChainImages.size();           //the maximum number of descriptor sets that can be allocated from this pool
                                                                    // - we are only allocating 1 descriptor set for each swapchain image so this is just the number of images in the swapchain
    poolInfo.flags = 0;                                             //determines if individual descriptor sets can be freed or not
                                                                    // - no touching the descriptor set after creating it so this is just 0

    //creating the pool
    if (vkCreateDescriptorPool(device.get_device(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }

}



void DescriptorPool2::setup() {
    //describe which descriptor types the descriptor sets are going to use
    VkDescriptorPoolSize poolSize{};                    //https://khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkDescriptorPoolSize.html
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;  //the type of the descriptor -- in this case it is for a uniform buffer
    poolSize.descriptorCount = swap_chain.swapChainImages.size();   //the number of descriptors to allocate (want one per image in the swap chain -- see descriptor_layout for why)

    //specifying information about the descriptor pool
    VkDescriptorPoolCreateInfo poolInfo{};                          //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkDescriptorPoolCreateInfo.html
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO; //sType must be VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO
    poolInfo.poolSizeCount = 1;                                     //the number of pools to create
    poolInfo.pPoolSizes = &poolSize;                                //array containing VkDescriptorPoolSizes
    poolInfo.maxSets = swap_chain.swapChainImages.size();           //the maximum number of descriptor sets that can be allocated from this pool
    // - we are only allocating 1 descriptor set for each swapchain image so this is just the number of images in the swapchain
    poolInfo.flags = 0;                                             //determines if individual descriptor sets can be freed or not
    // - no touching the descriptor set after creating it so this is just 0

    //creating the pool
    if (vkCreateDescriptorPool(device.get_device(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }

}

