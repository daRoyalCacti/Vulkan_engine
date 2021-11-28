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

    void setup();
    void cleanup();

private:
    LogicalDevice& device;
    SwapChain& swap_chain;
};


#endif //VULKAN_ENGINE_DESCRIPTOR_POOL_HPP
