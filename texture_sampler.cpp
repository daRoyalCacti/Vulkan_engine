//
// Created by jacob on 20/6/22.
//

#include "texture_sampler.hpp"
#include <stdexcept>

void TextureSampler::setup(const VkFilter magFilter, const VkFilter minFilter, const VkSamplerAddressMode addressMode) {
    VkPhysicalDeviceProperties properties{};
    vkGetPhysicalDeviceProperties(device.physical_device.get_device(), &properties);

    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable = VK_TRUE;
    samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;


    if (vkCreateSampler(device.get_device(), &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture sampler!");
    }

}

/*
void TextureSampler::setup(const VkFilter magFilter, const VkFilter minFilter, const VkSamplerAddressMode addressMode) {
    VkSamplerCreateInfo samplerInfo{};                              //https://www.khronos.org/registry/vulkan/specs/1.3-extensions/man/html/VkSamplerCreateInfo.html
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;      //sType must be VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO
    samplerInfo.magFilter = magFilter;                              //filter applied when image needs to be magnified
    samplerInfo.minFilter = minFilter;                              //filter applied when image needs to be minified
                                                                    // - there are only 2 options without using a device feature, nearest neighbour and linear
                                                                    // - https://www.khronos.org/registry/vulkan/specs/1.3-extensions/man/html/VkFilter.html
    samplerInfo.addressModeU = addressMode;                         //specifying how to address U coordinates outside [0,1]
    samplerInfo.addressModeV = addressMode;                         //specifying how to address V coordinates outside [0,1]
    samplerInfo.addressModeW = addressMode;                         //specifying how to address W coordinates outside [0,1]
                                                                    // - 4 settings without enabling device settings: repeat, mirrored repeat, clamp to edge, clamp to border
                                                                    // - https://www.khronos.org/registry/vulkan/specs/1.3-extensions/man/html/VkSamplerAddressMode.html
                                                                    // - nice image of these at https://vulkan-tutorial.com/images/texture_addressing.png
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;     //colour to use when clamping image to boarder --- only used for specific address modes
                                                                    // - other possibilities include white and transparent
    samplerInfo.unnormalizedCoordinates = VK_FALSE;                 //can address texels in range [0, tex_width) and [0, tex_height) instead of [0,1)
    samplerInfo.compareEnable = VK_FALSE;                           //If there should be a comparison operation that returns true or false
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;                   //The specific compare operation
                                                                    // - https://www.khronos.org/registry/vulkan/specs/1.3-extensions/man/html/VkCompareOp.html

    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;         //the mipmap mode used for texture lookups. Either nearest neighbour or linear
                                                                    // - https://www.khronos.org/registry/vulkan/specs/1.3-extensions/man/html/VkSamplerMipmapMode.html
    samplerInfo.mipLodBias = 0.0f;                                  //bias to add to the mipmap LOD calculation
    samplerInfo.minLod = 0.0f;                                      //minimum lod value to use
    samplerInfo.maxLod = 0.0f;                                      //maximum lod value to use


    samplerInfo.anisotropyEnable = VK_TRUE;                         //always enabling anisotropy --- could disable if performance is bad
    //want to enable to maxAnisotropy allowed by the device
    // - hence need to query the device
    VkPhysicalDeviceProperties properties{};
    vkGetPhysicalDeviceProperties(device.physical_device.get_device(), &properties);
    samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;

    //actually creating the sampler
    if (vkCreateSampler(device.get_device(), &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture sampler!");
    }
}
*/