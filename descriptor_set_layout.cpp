//
// Created by jacob on 28/11/21.
//

#include "descriptor_set_layout.hpp"
#include <stdexcept>

void DescriptorSetLayout1::setup() {
    //the descriptor for the UBO
    // - specifying where in the shader the descriptor is
    VkDescriptorSetLayoutBinding uboLayoutBinding{};                        //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkDescriptorSetLayoutBinding.html
    uboLayoutBinding.binding = 0;                                           //the binding=___ in the shader
                                                                            // - in this case referring to binding=0
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;    //the descriptor type (e.g. uniform buffer or image sampler)
                                                                            // - see https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkDescriptorType.html
    uboLayoutBinding.descriptorCount = 1;                                   //the number of descriptions in the binding
                                                                            // - would need multiple for skeleton animations (rotations of the bones)
                                                                            //currently only have a single uniform buffer object so this is just 1
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;               //the shader stage this descriptor set is for (in this case the vertex buffer)
                                                                            // - see https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkShaderStageFlagBits.html
                                                                            // - can be any combination of the flags in the link above
    uboLayoutBinding.pImmutableSamplers = nullptr;                          //only relevant for image sampling related descriptors (get to this later)

    //descriptor bindings are combined into a single VkDescriptorSetLayout object
    VkDescriptorSetLayoutCreateInfo layoutInfo{};                           //https://khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkDescriptorSetLayoutCreateInfo.html
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO; //sType must be VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO
    layoutInfo.bindingCount = 1;                                            //the total number of bindings
    layoutInfo.pBindings = &uboLayoutBinding;                               //the array of bindings to use

    //actually creating the descriptor set layout
    if (vkCreateDescriptorSetLayout(device.get_device(), &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
}

void DescriptorSetLayout1::cleanup() {
    vkDestroyDescriptorSetLayout(device.get_device(), descriptorSetLayout, nullptr);
}




void DescriptorSetLayout2::setup() {
    //the descriptor for the UBO
    // - specifying where in the shader the descriptor is
    VkDescriptorSetLayoutBinding uboLayoutBinding{};                        //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkDescriptorSetLayoutBinding.html
    uboLayoutBinding.binding = 0;                                           //the binding=___ in the shader
    // - in this case referring to binding=0
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;    //the descriptor type (e.g. uniform buffer or image sampler)
    // - see https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkDescriptorType.html
    uboLayoutBinding.descriptorCount = 1;                                   //the number of descriptions in the binding
    // - would need multiple for skeleton animations (rotations of the bones)
    //currently only have a single uniform buffer object so this is just 1
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;               //the shader stage this descriptor set is for (in this case the vertex buffer)
    // - see https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkShaderStageFlagBits.html
    // - can be any combination of the flags in the link above
    uboLayoutBinding.pImmutableSamplers = nullptr;                          //only relevant for image sampling related descriptors (get to this later)

    //descriptor bindings are combined into a single VkDescriptorSetLayout object
    VkDescriptorSetLayoutCreateInfo layoutInfo{};                           //https://khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkDescriptorSetLayoutCreateInfo.html
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO; //sType must be VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO
    layoutInfo.bindingCount = 1;                                            //the total number of bindings
    layoutInfo.pBindings = &uboLayoutBinding;                               //the array of bindings to use

    //actually creating the descriptor set layout
    if (vkCreateDescriptorSetLayout(device.get_device(), &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
}

void DescriptorSetLayout2::cleanup() {
    vkDestroyDescriptorSetLayout(device.get_device(), descriptorSetLayout, nullptr);
}
