//
// Created by jacob on 3/12/21.
//

#include "texture.hpp"


#define STB_IMAGE_IMPLEMENTATION
#include "dependencies/stb_image.h"
#include <stdexcept>
#include "buffer.hpp"



void create_image(LogicalDevice device, unsigned width, unsigned height, VkFormat, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) {
    //creating a texture object
    VkImageCreateInfo imageInfo{};                          //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkImageCreateInfo.html
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;  //sType must be VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO
    imageInfo.imageType = VK_IMAGE_TYPE_2D;                 //specifying if the image is 1D, 2D, or 3D
                                                            // - https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkImageType.html
    imageInfo.extent.width = width;                         //width of the image
    imageInfo.extent.height = height;                       //height of the image
    imageInfo.extent.depth = 1;                             //depth of the image (think 3D image)
    imageInfo.mipLevels = 1;                                //the number of levels of detail available
    imageInfo.arrayLayers = 1;                              //the number of layers in the image
    imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;             //the format of the pixels
                                                            // - use the same format for the texels as the pixels in the buffer
    imageInfo.tiling = tiling;                              //the arrangements of the texels in the image
                                                            // - VK_IMAGE_TILING_LINEAR: Texels are laid out in row-major order like our pixels array
                                                            // - VK_IMAGE_TILING_OPTIMAL: Texels are laid out in an implementation defined order for optimal access
                                                            //If you want to be able to directly access texels in the memory of the image, then you must use VK_IMAGE_TILING_LINEAR
                                                            // - using a staging buffer instead of a staging image so this is not necessary
                                                            //want VK_IMAGE_TILING_OPTIMAL for efficient access from the shader
                                                            // - https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkImageTiling.html
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;    //layout of the image and its subresources
                                                            // - https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkImageLayout.html
                                                            //There are only two possible values for the initialLayout of an image
                                                            // - VK_IMAGE_LAYOUT_UNDEFINED: Not usable by the GPU and the very first transition will discard the texels
                                                            // - VK_IMAGE_LAYOUT_PREINITIALIZED: Not usable by the GPU, but the first transition will preserve the texels.
                                                            //In our case, we're first going to transition the image to be a transfer destination and then copy texel data to it from a buffer object so we can safely use VK_IMAGE_LAYOUT_UNDEFINED
    imageInfo.usage = usage;                                //how the image is going to be used
                                                            // - https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkImageUsageFlagBits.html

    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;      //how the image should be shared among multiple queue families
                                                            // - https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkSharingMode.html
                                                            //the image is only used by 1 queue family so exclusive is fine
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;              //specifying sample counts supported for an image used for storage operations
                                                            // - only related to multisampling
                                                            // - https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkSampleCountFlagBits.html
                                                            //the maximum number of bits is set by a physical device limit
    imageInfo.flags = 0;                                    //additional parameters of the image (e.g. the image is spares)
                                                            // - https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkImageCreateFlagBits.html
                                                            //actually creating the image
                                                            // - this could fail if the VK_FORMAT_R8G8B8A8_SRGB is not supported by the graphics hardware
    if (vkCreateImage(device.get_device(), &imageInfo, nullptr, &image) != VK_SUCCESS) {
        throw std::runtime_error("failed to create image");
    }

    //Allocating memory to the texture
    // - we have only created the texture object, not actually given memory for it to reference
    //===============
    //first getting the requirements that the image requires
    VkMemoryRequirements memRequirements;   //Stores the memory requirements
    vkGetImageMemoryRequirements(device.get_device(), image, &memRequirements);  //https://www.khronos.org/registry/vulkan/specs/1.3-extensions/man/html/vkGetImageMemoryRequirements.html
    // - actually querying the memory requirements
    //setting the parameters required for allocation
    VkMemoryAllocateInfo allocInfo{};                               //https://www.khronos.org/registry/vulkan/specs/1.3-extensions/man/html/VkMemoryAllocateInfo.html
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;       //sType must be VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO
    allocInfo.allocationSize = memRequirements.size;                //the size of the memory to allocate in bytes
    allocInfo.memoryTypeIndex = findMemoryType(device.physical_device, memRequirements.memoryTypeBits, properties);    //finding the best memory type to use

    //actually allocating the memory
    if (vkAllocateMemory(device.get_device(), &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate image memory!");
    }

    //binding the image to the memory just created
    vkBindImageMemory(device.get_device(), image, imageMemory, 0);    //https://www.khronos.org/registry/vulkan/specs/1.3-extensions/man/html/vkBindImageMemory.html
}


void transition_image_layout(LogicalDevice device, CommandPool command_pool, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) {
    SingleTimeCommandBuffer command_buffer(device, command_pool);
    command_buffer.begin_recording();

    //here we transition the images using a memory barrier
    VkImageMemoryBarrier barrier{};                             //https://www.khronos.org/registry/vulkan/specs/1.3-extensions/man/html/VkImageMemoryBarrier.html
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;     //sType must be VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER
    barrier.oldLayout = oldLayout;                              //old layout in an image transition
    barrier.newLayout = newLayout;                              //new layout in an image transition
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;      //used when transferring queue ownership (not relevant here)
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;      //used when transferring queue ownership (not relevant here)
    barrier.image = image;                                      //the handle to the image which this barrier is to effect
    //barrier.subresourceRange refers to the subresource range of the image which is effected by this barrier
    // - https://www.khronos.org/registry/vulkan/specs/1.3-extensions/man/html/VkImageSubresourceRange.html
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;    //bitmask specifying which aspects of the image are effected by the barrier
                                                                        // - https://www.khronos.org/registry/vulkan/specs/1.3-extensions/man/html/VkImageAspectFlagBits.html
    barrier.subresourceRange.baseMipLevel = 0;                          //the first mipmap level to be effected
    barrier.subresourceRange.levelCount = 1;                            //the number of mipmap levels to be effected
    barrier.subresourceRange.baseArrayLayer = 0;                        //the first layer of the image to be effected
    barrier.subresourceRange.layerCount = 1;                            //the number of layers to be effected

    //need to handle a range of source and destinations
    // - want to handle undefined to transfer destination. These transfers don't need to wait on anything
    // - also need to handel transfer destination to shader reading. Shader reads should wait on transfer writes.
    //   -- This is especially the case in the fragment shader where were going to be using the texture
    //due to this need on waiting for things, the transition must occur at different points in the shader pipeline
    //creating some variables to help with this
    VkPipelineStageFlags sourceStage;           //https://www.khronos.org/registry/vulkan/specs/1.3-extensions/man/html/VkPipelineStageFlags.html
    VkPipelineStageFlags destinationStage;      //these are bitmasks of VkPipelineStageFlagBits that specify the pipeline stage to apply the memory barrier
                                                // -https://www.khronos.org/registry/vulkan/specs/1.3-extensions/man/html/VkPipelineStageFlagBits.html

    //below barrier.srcAccessMask and barrier.dstAccessMask are bitmasks of specifying the types of memory acesses that the image with experience
    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        //undefined to transfer destination
        barrier.srcAccessMask = 0;                              //initially undefined
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;   //new layout is the transfer of a write

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;        //no commands happen before the barrier --- no need to wait on anything
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;      //this is not a real stage in the pipeline. The image will be written in the same pipeline stage
    } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        //transfer destination to shader reading
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;   //initially layout is the destination of a write
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;      //new layout is for optimal shader reading

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;               //transition needs to happen after the buffer is written to. Hence the barrier is place after the transfer stage
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;   //transition needs to be finished before the fragment shader as this is where the image is read
    } else {
        throw std::invalid_argument("unsupported layout transition!");
    }

    vkCmdPipelineBarrier(                                   //https://www.khronos.org/registry/vulkan/specs/1.3-extensions/man/html/vkCmdPipelineBarrier.html
            command_buffer.get_buffer(),                    //the command buffer to record the commands to
            sourceStage,                                    //where the barrier was placed initially
            destinationStage,                               //where the barrier is to be placed now
            0,                               //bitmask specifying how memory dependencies are formed. Can be used to allow reading from parts of a resource that where writter so far
                                                            // - https://www.khronos.org/registry/vulkan/specs/1.3-extensions/man/html/VkDependencyFlagBits.html
            0,                            //used for memory barriers (not used here)
            nullptr,                         //used for memory barriers (not used here)
            0,                       //used for buffer memory barriers (not used here)
            nullptr,                    //used for buffer memory barriers (not used here)
            1,                       //the number of image memory barriers to create
            &barrier                                        //pointer to array of memory barriers to construct
    );


    command_buffer.execute_recording();
}


void copy_buffer_to_image(LogicalDevice device, CommandPool command_pool, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) {
    SingleTimeCommandBuffer command_buffer(device, command_pool);
    command_buffer.begin_recording();

    //specifying which part of the buffer is going to be copied into which part of the image
    VkBufferImageCopy region{};                                         //https://www.khronos.org/registry/vulkan/specs/1.3-extensions/man/html/VkBufferImageCopy.html
    region.bufferOffset = 0;                                            //offset in bytes from the start of the buffer
    region.bufferRowLength = 0;                                         // specify in texels a subregion of a 2 or 3 dimensional image in buffer memory
    region.bufferImageHeight = 0;                                       // if both height and width are 0, the buffer memory is considered tightly packed
                                                                        // this is used where there is some padding bytes between rows of the image
    //subresourceRange refers to the subresource range of the image which is effected by this copy
    // - https://www.khronos.org/registry/vulkan/specs/1.3-extensions/man/html/VkImageSubresourceLayers.html
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;     //bitmask specifying which aspects of the image are to be copied into
                                                                        // - https://www.khronos.org/registry/vulkan/specs/1.3-extensions/man/html/VkImageAspectFlagBits.html
    region.imageSubresource.mipLevel = 0;                               //the first mipmap level to be effected
    region.imageSubresource.baseArrayLayer = 0;                         //the number of mipmap levels to be effected
    region.imageSubresource.baseArrayLayer = 0;                         //the first layer of the image to be effected
    region.imageSubresource.layerCount = 1;                             //the number of layers to be effected

    region.imageOffset = {0, 0, 0};                             //the initial (x,y,z) offsets in texels
    region.imageExtent = {width, height, 1};                      //size of the region being copied into
                                                                         // - https://www.khronos.org/registry/vulkan/specs/1.3-extensions/man/html/VkExtent3D.html


    //actually specifying the copy
    vkCmdCopyBufferToImage(                                         //https://www.khronos.org/registry/vulkan/specs/1.3-extensions/man/html/vkCmdCopyBufferToImage.html
            command_buffer.get_buffer(),                            //the command buffer to record the command
            buffer,                                                 //the source of the data --- the buffer the image data is contained in
            image,                                                  //the image to copy into
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,     //the layout of the destination image subresource --- should just be layout the image is currently in
            1,                                           //the number of regions to copy --- it's possible to perform many different copies from a buffer to an image in one operation
            &region                                                 //pointer to an array of regions to copy
    );

    command_buffer.execute_recording();
}



void Texture::setup() {
    //loading the image using stb_image
    //=================================
    int texture_width, texture_height, texture_channels;    //channel holds the number of vales per pixel (i.e. 3 for rgb and 4 for rgba)
    stbi_uc* pixels = stbi_load(texture_path.data(), &texture_width, &texture_height, &texture_channels, STBI_rgb_alpha);   //cannot be const - stbi_image_free does not work then
    //STBI_rgb_alpha value forces the image to be loaded with an alpha channel, even if it doesn't have one
    VkDeviceSize imageSize = texture_width * texture_height * 4;    //could also do *texture_channels instead of 4, but we know the number of texture channels is 4

    //checking to make sure the image was indeed loaded
    if (!pixels) {
        std::string err_message("failed to load texture image : ");
        err_message.append(texture_path);
        throw std::runtime_error(err_message);
    }

    //create a buffer in host visible to memory to copy the pixels into
    //==================================================================
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    create_buffer(device, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
    //copying the data straight into the buffer
    void* data;
    vkMapMemory(device.get_device(), stagingBufferMemory, 0, imageSize, 0, &data);
    memcpy(data, pixels, imageSize);
    vkUnmapMemory(device.get_device(), stagingBufferMemory);
    //now that the data is copied into the staging buffer, the image data is no longer needed
    stbi_image_free(pixels);



    //creating the texture object
    // - The image is going to be used as destination for the buffer copy, so it should be set up as a transfer destination
    // - We also want to be able to access the image from the shader to color our mesh
    create_image(device, texture_width, texture_height, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage, textureImageMemory);


    //transitioning the image to an optimal format of copying into
    transition_image_layout(device, command_pool, textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    //actually copying the data into the image
    copy_buffer_to_image(device, command_pool, stagingBuffer, textureImage, static_cast<unsigned>(texture_width), static_cast<unsigned>(texture_height));

    //transitioning the image into a layout for optimal shader access
    transition_image_layout(device, command_pool, textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    //destroying the memory for the buffer --- the data has been copied into the image so this is no longer needed
    vkDestroyBuffer(device.get_device(), stagingBuffer, nullptr);
    vkFreeMemory(device.get_device(), stagingBufferMemory, nullptr);
}

void Texture::cleanup() {
    //Destroying the image
    vkDestroyImage(device.get_device(), textureImage, nullptr);
    vkFreeMemory(device.get_device(), textureImageMemory, nullptr);

}
