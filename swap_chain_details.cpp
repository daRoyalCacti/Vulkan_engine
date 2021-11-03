//
// Created by jacob on 3/11/21.
//

#include "swap_chain_details.hpp"

#include <cstdint>  //needed for UINT32_MAX
#include <algorithm>    //needed for std::clamp

//need device and surface because these are the core components of the swapchain
void SwapChainDetails::query_swap_chain_support(VkPhysicalDevice device, VkSurfaceKHR surface) {
    //querying the capabilities of the swapchain
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &capabilities);

    //filling the structures with the available formats and present modes
    //first formats
    unsigned format_count;  //holds the number of formats
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, nullptr);  //querying the number of formats
    formats.resize(format_count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, formats.data());   //querying the formats

    //second present mode
    unsigned present_count; //holds the number of present modes
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_count, nullptr);    //querying the number of present modes
    presentModes.resize(present_count);
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_count, presentModes.data());    //querying the present modes


}

VkSurfaceFormatKHR SwapChainDetails::chooseSwapSurfaceFormat() {
    //finding the format that is best
    for (const auto& availableFormat : formats) {
        if (availableFormat.format == best_format && availableFormat.colorSpace == best_colour_space) {
            return availableFormat;
        }
    }

    //if cannot find best format, just return any old format
    // - should really rank the formats to choose the next best one

    return formats[0];
}

VkPresentModeKHR SwapChainDetails::chooseSwapPresentMode() {
    //finding the present mode that is best
    for (const auto& availablePresentMode : presentModes) {
        if (availablePresentMode == best_present_mode) {
            return availablePresentMode;
        }
    }

    //VK_PRESENT_MODE_FIFO_KHR is the only format that is guaranteed to be available
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D SwapChainDetails::chooseSwapExtent(GLFWwindow* window) const {
    //some window managers do not allow the resolution of the swap chain to differ from the resolution of the window
    //those that can differ have capabilities.currentExtent.width and capabilities.currentExtent.height = UINT32_MAX
    //when they can't differ, just return the extent (match the swapchain resolution to the window resolution)
    if (capabilities.currentExtent.width != UINT32_MAX) {
        return capabilities.currentExtent;
    }
    //if they can differ then resolution that best matches the screen
    // - cannot always get a perfect match because the swap chain might not be able to go small/big enough
    else {
        //finding the resolution of the window in pixels
        // - note this is not the same as the window width and height defined when creating the window.
        //   these are screen coordinates. Most of the time they match up with pixels but not always.
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        //making sure the size is between the min and max allowed
        VkExtent2D actualExtent{ static_cast<unsigned>(width), static_cast<unsigned>(height) };
        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}
