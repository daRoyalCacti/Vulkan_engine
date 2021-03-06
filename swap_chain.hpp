//
// Created by jacob on 4/11/21.
//

#ifndef VULKAN_ENGINE_SWAP_CHAIN_HPP
#define VULKAN_ENGINE_SWAP_CHAIN_HPP

#include "window.hpp"
#include "logical_device.hpp"
#include "surface.hpp"
#include "queue_family.hpp"

#include <vector>

struct SwapChain {
    SwapChain(Window& w, LogicalDevice& d, Surface &s, QueueFamily &q) : window(w), device(d), surface(s), queue_family(q) {}

    void setup();
    void cleanup() const {vkDestroySwapchainKHR(device.get_device(), swapChain, nullptr);}

    //whether pixels that are obscured by other windows should be deleted
    // - better performance if set to true
    static constexpr VkBool32 clip_pixels = VK_TRUE;

    [[nodiscard]] VkSwapchainKHR& get_swap_chain() {return swapChain;}

    VkSwapchainKHR swapChain{}; //handle to the swap chain

    std::vector<VkImage> swapChainImages;   //reference to the images created by the swapchain

    //the settings of the swap chain
    VkSurfaceFormatKHR surface_format{};    //colour format (e.g. rgba)
    VkPresentModeKHR present_mode{};        //vsync and the like
    VkExtent2D extent{};                    //resolution for images in the swapchain

private:
    Window& window;
    LogicalDevice& device;
    Surface &surface;
    QueueFamily &queue_family;
};


#endif //VULKAN_ENGINE_SWAP_CHAIN_HPP
