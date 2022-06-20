//
// Created by jacob on 3/12/21.
//

#ifndef VULKAN_ENGINE_TEXTURE_HPP
#define VULKAN_ENGINE_TEXTURE_HPP

#include <string_view>
#include "logical_device.hpp"
#include "command_pool.hpp"

struct Texture {
    //could set up the shader to access the pixel values in the shader
    //better to use image objects
    // - faster to retrieve colors because you can use 2D coordinates
    VkImage textureImage{};
    VkDeviceMemory textureImageMemory{};


    Texture(LogicalDevice &d, CommandPool &p, const std::string_view path) : device(d), command_pool(p), texture_path(path) {}

    void setup();
    void cleanup();

    const std::string_view texture_path;

    [[nodiscard]] VkImage& get_image() {return textureImage;}


private:
    LogicalDevice& device;
    CommandPool &command_pool;
};

//helper function to create images
void create_image(LogicalDevice device, unsigned width, unsigned height, VkFormat, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

//helper function to transfer the format of images
// - images don't start off with any specific format (I'm pretty sure)
// - this can be used to set that format
// - also, it is more efficient to have an image in a format optimized for loading data, then transitioning it to a format optimized for acess in the shader
void transition_image_layout(LogicalDevice device, CommandPool command_pool, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);


//helper function for moving data into the image object
void copy_buffer_to_image(LogicalDevice device, CommandPool command_pool, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);


#endif //VULKAN_ENGINE_TEXTURE_HPP
