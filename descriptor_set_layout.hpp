//
// Created by jacob on 28/11/21.
//

#ifndef VULKAN_ENGINE_DESCRIPTOR_SET_LAYOUT_HPP
#define VULKAN_ENGINE_DESCRIPTOR_SET_LAYOUT_HPP

#include "logical_device.hpp"

struct DescriptorSetLayout {
    VkDescriptorSetLayout descriptorSetLayout{};    //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkDescriptorSetLayout.html

    [[nodiscard]] VkDescriptorSetLayout& get_layout() {return descriptorSetLayout;}

    explicit DescriptorSetLayout(LogicalDevice &d) : device(d) {}

    virtual void setup() {}
    virtual void cleanup() {}

protected:
    LogicalDevice& device;
};


//The descriptor layout specifies the types of resources that are going to be accessed by the pipeline
// - it also includes the binding=x in the shaders
struct DescriptorSetLayout1 : public DescriptorSetLayout {
    explicit DescriptorSetLayout1(LogicalDevice &d) : DescriptorSetLayout(d) {}

    void setup() override;
    void cleanup() override;
};


struct DescriptorSetLayout2 : public DescriptorSetLayout  {
    explicit DescriptorSetLayout2(LogicalDevice &d) : DescriptorSetLayout(d) {}

    void setup() override;
    void cleanup() override;
};


#endif //VULKAN_ENGINE_DESCRIPTOR_SET_LAYOUT_HPP
