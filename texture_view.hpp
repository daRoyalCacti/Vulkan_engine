//
// Created by jacob on 20/6/22.
//

#ifndef VULKAN_ENGINE_TEXTURE_VIEW_HPP
#define VULKAN_ENGINE_TEXTURE_VIEW_HPP

#include "texture.hpp"

struct TextureView {
    VkImageView textureImageView{};

    TextureView(LogicalDevice &d, Texture &t) : device(d), texture(t) {}

    void setup();
    void cleanup() { vkDestroyImageView(device.get_device(), textureImageView, nullptr); }

private:
    Texture& texture;
    LogicalDevice& device;

};


#endif //VULKAN_ENGINE_TEXTURE_VIEW_HPP
