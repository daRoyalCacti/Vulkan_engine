//
// Created by jacob on 6/11/21.
//

#ifndef VULKAN_ENGINE_VIEWPORT_HPP
#define VULKAN_ENGINE_VIEWPORT_HPP

#include <vulkan/vulkan.h>

//the region of the framebuffer the output will be rendered to
struct Viewport {
    //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkViewport.html
    VkViewport viewport{};

    Viewport(const float width, const float height, const float offset_x = 0.0f, const float offset_y = 0.0f, const float min_depth = 0.0f, const float max_depth = 1.0f) {
        //the viewports upper corner
        viewport.x = offset_x;
        viewport.y = offset_y;
        //the size of the viewport
        viewport.width = width;
        viewport.height = height;
        //the depth of the viewport
        // - the depth values in the framebuffer
        // - if not doing anything special they should just be 0 and 1
        viewport.minDepth = min_depth;
        viewport.maxDepth = max_depth;
    }

    [[nodiscard]] VkViewport& get_pipeline_stage() {return viewport;}
};


#endif //VULKAN_ENGINE_VIEWPORT_HPP
