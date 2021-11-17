//
// Created by jacob on 6/11/21.
//

#ifndef VULKAN_ENGINE_SCISSOR_HPP
#define VULKAN_ENGINE_SCISSOR_HPP

#include <vulkan/vulkan.h>

//defines a rectangle in which the pixels will actually be stored
// - are like a filter
struct Scissor {
    //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkRect2D.html
    VkRect2D scissor{};

    explicit Scissor(VkExtent2D& extent, VkOffset2D offset = {0,0}) {
        //the upper corner
        // - see https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkOffset2D.html
        scissor.offset = offset;
        //the extent -- just the width and the height
        // - see https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkExtent2D.html
        scissor.extent = extent;
    }

    [[nodiscard]] VkRect2D& get_pipeline_stage() {return scissor;}
};


#endif //VULKAN_ENGINE_SCISSOR_HPP
