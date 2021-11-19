//
// Created by jacob on 19/11/21.
//

#ifndef VULKAN_ENGINE_FENCES_HPP
#define VULKAN_ENGINE_FENCES_HPP

#include <vulkan/vulkan.h>
#include <array>
#include "logical_device.hpp"

//fences perform CPU-GPU synchronisation
// - actually wait for them explictly in the code
template <unsigned no_fences>
struct Fences{
    explicit Fences(LogicalDevice &d) : device(d) {}

    std::array<VkFence, no_fences> in_flight_fences;

    [[nodiscard]] std::array<VkFence, no_fences>& get_fences() {return in_flight_fences;}

    void setup() {
        //creating fences is trivial
        VkFenceCreateInfo fenceInfo{};      //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkFenceCreateInfo.html
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;  //sType must be VK_STRUCTURE_TYPE_FENCE_CREATE_INFO
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;     //fences start signaled
                                                            //the way the draw loop is setup this is required for the program not to be infinitely waiting before rendering starts

        for (auto& fence : in_flight_fences) {
            const auto create_res = vkCreateFence(device.get_device(), &fenceInfo, nullptr, &fence);
            if (create_res != VK_SUCCESS) {
                throw std::runtime_error("failed to create fences");
            }
        }

    }

    void cleanup() {
        for (auto& fence : in_flight_fences) {
            vkDestroyFence(device.get_device(), fence, nullptr);
        }
    }

private:
    LogicalDevice &device;
};

#endif //VULKAN_ENGINE_FENCES_HPP
