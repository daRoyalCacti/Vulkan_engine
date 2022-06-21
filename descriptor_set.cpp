//
// Created by jacob on 28/11/21.
//

#include "descriptor_set.hpp"
#include <stdexcept>

void DescriptorSet1::setup() {
    //specifying the descriptor pool to allocate the set from
    std::vector<VkDescriptorSetLayout> layouts(swap_chain.swapChainImages.size(), descriptor_set_layout.get_layout());  //array of the descriptor set layout repeated swapChainImages.size() number of times
    VkDescriptorSetAllocateInfo allocInfo{};                            //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkDescriptorSetAllocateInfo.html
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;   //sType must be VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO
    allocInfo.descriptorPool = descriptor_pool.get_pool();              //the pool to allocate the descriptor sets from
    allocInfo.descriptorSetCount = swap_chain.swapChainImages.size();   //the number of sets to allocate
                                                                        // - there is a descriptor set for each image in the swapchain
    allocInfo.pSetLayouts = layouts.data();                             //array of descriptor set layouts
                                                                        // - want the same descriptor set layout for each image in the swapchin

    //allocating all the descriptor sets needed
    descriptorSets.resize(swap_chain.swapChainImages.size());
    if (vkAllocateDescriptorSets(device.get_device(), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

    //configuring the descriptor sets
    // - specifying which uniform buffer they correspond to
    // - general config
    for (size_t i = 0; i < swap_chain.swapChainImages.size(); i++) {
        VkDescriptorBufferInfo bufferInfo{};        //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkDescriptorBufferInfo.html
        bufferInfo.buffer = UBO.get_buffers()[i];   //the buffer to attach to this descriptor set
        bufferInfo.offset = 0;                      //the offset in bytes into the buffer
                                                    // - not doing anything fancy so this is just 0
        bufferInfo.range = sizeof(UBO::mvp);        //the size in bytes used for a descriptor updated
                                                    // - this is the size of the object in question

        VkWriteDescriptorSet descriptorWrite{};                             //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkWriteDescriptorSet.html
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;     //sType must be VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET
        descriptorWrite.dstSet = descriptorSets[i];                         //the descriptor ste to update
        descriptorWrite.dstBinding = 0;                                     //the descriptor binding within the set
                                                                            // - i.e. attaching this buffer to the "location=___" in the shader
        descriptorWrite.dstArrayElement = 0;                                //the index in the array of descriptor layouts
                                                                            // - not using an array here so this is just 0
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; //specifying the type of the descriptor
                                                                            // - clearly want it for a uniform buffer
        descriptorWrite.descriptorCount = 1;                                //the number of descriptors to update
        descriptorWrite.pBufferInfo = &bufferInfo;                          //specifying the ubo to bind to the descriptor set
        descriptorWrite.pImageInfo = nullptr;                               //not using images here
        descriptorWrite.pTexelBufferView = nullptr;                         //not using buffer views here

        vkUpdateDescriptorSets(device.get_device(), 1, &descriptorWrite, 0, nullptr);

    }

}




void DescriptorSet2::setup() {
    //specifying the descriptor pool to allocate the set from
    std::vector<VkDescriptorSetLayout> layouts(swap_chain.swapChainImages.size(), descriptor_set_layout.get_layout());  //array of the descriptor set layout repeated swapChainImages.size() number of times
    VkDescriptorSetAllocateInfo allocInfo{};                            //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkDescriptorSetAllocateInfo.html
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;   //sType must be VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO
    allocInfo.descriptorPool = descriptor_pool.get_pool();              //the pool to allocate the descriptor sets from
    allocInfo.descriptorSetCount = swap_chain.swapChainImages.size();   //the number of sets to allocate
    // - there is a descriptor set for each image in the swapchain
    allocInfo.pSetLayouts = layouts.data();                             //array of descriptor set layouts
    // - want the same descriptor set layout for each image in the swapchin

    //allocating all the descriptor sets needed
    descriptorSets.resize(swap_chain.swapChainImages.size());
    if (vkAllocateDescriptorSets(device.get_device(), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

    //configuring the descriptor sets
    // - specifying which uniform buffer they correspond to
    // - general config
    for (size_t i = 0; i < swap_chain.swapChainImages.size(); i++) {
        //info for the UBO
        VkDescriptorBufferInfo bufferInfo{};        //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkDescriptorBufferInfo.html
        bufferInfo.buffer = UBO.get_buffers()[i];   //the buffer to attach to this descriptor set
        bufferInfo.offset = 0;                      //the offset in bytes into the buffer
                                                    // - not doing anything fancy so this is just 0
        bufferInfo.range = sizeof(UBO::mvp);        //the size in bytes used for a descriptor updated
                                                    // - this is the size of the object in question

        //info for the texture sampler
        VkDescriptorImageInfo imageInfo{};                                  //https://www.khronos.org/registry/vulkan/specs/1.3-extensions/man/html/VkDescriptorImageInfo.html
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;   //the layout that the image subresources will be in at the time this descriptor is accessed
        imageInfo.imageView = texture_view.get_view();                      //the view that the descriptor will use
        imageInfo.sampler = texture_sampler.get_sample();                   //the sample the descriptor will use


        std::array<VkWriteDescriptorSet, 2> descriptorWrites{};                 //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkWriteDescriptorSet.html
        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;     //sType must be VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET
        descriptorWrites[0].dstSet = descriptorSets[i];                         //the descriptor ste to update
        descriptorWrites[0].dstBinding = 0;                                     //the descriptor binding within the set
                                                                                // - i.e. attaching this buffer to the "location=___" in the shader
        descriptorWrites[0].dstArrayElement = 0;                                //the index in the array of descriptor layouts
                                                                                // - not using an array here so this is just 0
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; //specifying the type of the descriptor
                                                                                // - clearly want it for a uniform buffer
        descriptorWrites[0].descriptorCount = 1;                                //the number of descriptors to update
        descriptorWrites[0].pBufferInfo = &bufferInfo;                          //specifying the ubo to bind to the descriptor set
        descriptorWrites[0].pImageInfo = nullptr;                               //not using images here
        descriptorWrites[0].pTexelBufferView = nullptr;                         //not using buffer views here

        descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;             //sType must be VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET
        descriptorWrites[1].dstSet = descriptorSets[i];                                 //the descriptor ste to update
        descriptorWrites[1].dstBinding = 1;                                             //the descriptor binding within the set
                                                                                        // - i.e. attaching this buffer to the "location=___" in the shader
        descriptorWrites[1].dstArrayElement = 0;                                        //the index in the array of descriptor layouts
                                                                                        // - not using an array here so this is just 0
        descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; //specifying the type of the descriptor
                                                                                        // - clearly want it for a uniform buffer
        descriptorWrites[1].descriptorCount = 1;                                        //the number of descriptors to update
        descriptorWrites[1].pBufferInfo = nullptr;                                      //not using buffers here
        descriptorWrites[1].pImageInfo = &imageInfo;                                    //specifying the details of the image that will be accessed
        descriptorWrites[1].pTexelBufferView = nullptr;                                 //not using buffer views here

        vkUpdateDescriptorSets(device.get_device(), descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);

    }

}
