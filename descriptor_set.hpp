//
// Created by jacob on 28/11/21.
//

#ifndef VULKAN_ENGINE_DESCRIPTOR_SET_HPP
#define VULKAN_ENGINE_DESCRIPTOR_SET_HPP

#include <vector>
#include <vulkan/vulkan.h>
#include "logical_device.hpp"
#include "swap_chain.hpp"
#include "uniform_buffer_objects.hpp"
#include "descriptor_pool.hpp"
#include "descriptor_set_layout.hpp"

/*It is actually possible to bind multiple descriptor sets simultaneously.
 * You need to specify a descriptor layout for each descriptor set when creating the pipeline layout.
 * Shaders can then reference specific descriptor sets like this:
        layout(set = 0, binding = 0) uniform UniformBufferObject1 { ... }
 * You can use this feature to put descriptors that vary per-object and descriptors that are shared into separate descriptor sets.
 * In that case you avoid rebinding most of the descriptors across draw calls which is potentially more efficient.
 */
//these are the equivalent of the command buffers for the command pools
//these are automatically cleanup up when the pool is destroyed
struct DescriptorSet {
    std::vector<VkDescriptorSet> descriptorSets;

    [[nodiscard]] std::vector<VkDescriptorSet>& get_sets() {return descriptorSets;}

    DescriptorSet(LogicalDevice& d, SwapChain &s, DescriptorPool& p, DescriptorSetLayout &l)
            : device(d), swap_chain(s), descriptor_pool(p), descriptor_set_layout(l) {}

    virtual void setup(){}

protected:
    LogicalDevice &device;
    SwapChain &swap_chain;
    DescriptorPool &descriptor_pool;
    DescriptorSetLayout &descriptor_set_layout;
};


struct DescriptorSet1 : public DescriptorSet{
    DescriptorSet1(LogicalDevice& d, SwapChain &s, UniformBufferObject &ubo, DescriptorPool& p, DescriptorSetLayout &l)
    : DescriptorSet(d, s, p, l), UBO(ubo) {}

    void setup() override;

private:
    UniformBufferObject &UBO;
};


struct DescriptorSet2 : public DescriptorSet{
    DescriptorSet2(LogicalDevice& d, SwapChain &s, UniformBufferObject &ubo, DescriptorPool& p, DescriptorSetLayout &l)
            : DescriptorSet(d, s, p, l), UBO(ubo) {}

    void setup() override;

private:
    UniformBufferObject &UBO;
};


#endif //VULKAN_ENGINE_DESCRIPTOR_SET_HPP
