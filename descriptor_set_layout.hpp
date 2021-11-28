//
// Created by jacob on 28/11/21.
//

#ifndef VULKAN_ENGINE_DESCRIPTOR_SET_LAYOUT_HPP
#define VULKAN_ENGINE_DESCRIPTOR_SET_LAYOUT_HPP

#include "logical_device.hpp"

//The descriptor layout specifies the types of resources that are going to be accessed by the pipeline
// - it also includes the binding=x in the shaders
struct DescriptorSetLayout {
    VkDescriptorSetLayout descriptorSetLayout{};    //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkDescriptorSetLayout.html

    [[nodiscard]] VkDescriptorSetLayout& get_layout() {return descriptorSetLayout;}

    explicit DescriptorSetLayout(LogicalDevice &d) : device(d) {}

    void setup();
    void cleanup();

private:
    LogicalDevice& device;
};


#endif //VULKAN_ENGINE_DESCRIPTOR_SET_LAYOUT_HPP
