//
// Created by jacob on 3/11/21.
//

#ifndef VULKAN_ENGINE_SWAP_CHAIN_DETAILS_HPP
#define VULKAN_ENGINE_SWAP_CHAIN_DETAILS_HPP

#include <vulkan/vulkan.h>
#include <vector>
#include <GLFW/glfw3.h>

//swapchain is like a framebuffer
//it is a queue of images that are waiting to be presented to the screen
//
//We need to check if the swapchain has the properties that we want (e.g. srgb)
struct SwapChainDetails {
    VkSurfaceCapabilitiesKHR capabilities;      //the characteristics of the swap chain. Like the maximum number of images.
                                                //see https://khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkSurfaceCapabilitiesKHR.html
    std::vector<VkSurfaceFormatKHR> formats;    //the available swapchain format-color (e.g. rgb, rgba, srgb)
                                                //see https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkSurfaceFormatKHR.html,
                                                //and https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkFormat.html
                                                //and https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkColorSpaceKHR.html
    std::vector<VkPresentModeKHR> presentModes; //supported present modes. These include things like vsync
                                                //see https://khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkPresentModeKHR.html
    /* The available present modes are
     * VK_PRESENT_MODE_IMMEDIATE_KHR:       Images submitted by your application are transferred to the screen right away, which may result in tearing.
     * VK_PRESENT_MODE_FIFO_KHR:            The swap chain is a queue where the display takes an image from the front of the queue when the display is refreshed and the program inserts rendered images at the back of the queue.
     *                                      If the queue is full then the program has to wait. This is most similar to vertical sync as found in modern games. The moment that the display is refreshed is known as "vertical blank".
     * VK_PRESENT_MODE_FIFO_RELAXED_KHR:    This mode only differs from the previous one if the application is late and the queue was empty at the last vertical blank.
     *                                      Instead of waiting for the next vertical blank, the image is transferred right away when it finally arrives. This may result in visible tearing.
     * VK_PRESENT_MODE_MAILBOX_KHR:         This is another variation of the second mode. Instead of blocking the application when the queue is full, the images that are already queued are simply replaced with the newer ones.
     *                                      This mode can be used to render frames as fast as possible while still avoiding tearing, resulting in fewer latency issues than standard vertical sync.
     *                                      This is commonly known as "triple buffering", although the existence of three buffers alone does not necessarily mean that the framerate is unlocked.
     */


    void query_swap_chain_support(VkPhysicalDevice device, VkSurfaceKHR surface);

    //function to return the best surface format out of those in `formats'
    VkSurfaceFormatKHR chooseSwapSurfaceFormat();
    //function to return the best present mode out of those in `presentModes'
    VkPresentModeKHR chooseSwapPresentMode();
    //choosing the resolution for the swapchain (based on the window resolution)
    VkExtent2D chooseSwapExtent(GLFWwindow* window) const;

    //specifying what the best format would be for the swap chain (i.e. framebuffer)
    // - in this case srgb would be best
    static constexpr VkFormat best_format = VK_FORMAT_B8G8R8A8_SRGB;    //the format that we want to the swap chain to have
    static constexpr VkColorSpaceKHR best_colour_space = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR; //the best colour space to have

    //specifying the best present mode
    static constexpr VkPresentModeKHR best_present_mode = VK_PRESENT_MODE_FIFO_KHR;
};


#endif //VULKAN_ENGINE_SWAP_CHAIN_DETAILS_HPP
