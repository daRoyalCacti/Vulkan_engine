//
// Created by jacob on 20/6/22.
//

#ifndef VULKAN_ENGINE_TEXTURE_SAMPLER_HPP
#define VULKAN_ENGINE_TEXTURE_SAMPLER_HPP

#include "logical_device.hpp"

//structure specifying how the shader is to interface with images when there are more/less texels than fragments
// - e.g. nearest neighbour vs cubic interpolation
// - also turns on anisotropic filtering
struct TextureSampler {
    VkSampler textureSampler{};

    explicit TextureSampler(LogicalDevice &d) : device(d) {}

    //the specifics of the sampler are specified in the setup function
    // - first parameter states what happens when an image needs to be magnified
    // - second parameter states what happens when an image needs to be shrunk
    // - third paramter states what happens when the shader tries to access a texel outside the image
    void setup(VkFilter magFilter, VkFilter minFilter, VkSamplerAddressMode adressMode);
    void cleanup() {vkDestroySampler(device.get_device(), textureSampler, nullptr);}

private:
    LogicalDevice& device;
};


#endif //VULKAN_ENGINE_TEXTURE_SAMPLER_HPP
