//
// Created by jacob on 28/11/21.
//

#ifndef VULKAN_ENGINE_DESCRIPTOR_POOL_HPP
#define VULKAN_ENGINE_DESCRIPTOR_POOL_HPP

#include <vulkan/vulkan.h>
#include "logical_device.hpp"
#include "swap_chain.hpp"

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
struct DescriptorPool1 : public DescriptorPool {
    DescriptorPool1(LogicalDevice &d, SwapChain &s) : DescriptorPool(d,s) {}
    void setup() override;
};

//holds the memory for the descriptor sets
struct DescriptorPool2 : public DescriptorPool {
    DescriptorPool2(LogicalDevice &d, SwapChain &s) : DescriptorPool(d,s) {}
    void setup() override;
};


#endif //VULKAN_ENGINE_DESCRIPTOR_POOL_HPP
