//
// Created by jacob on 19/11/21.
//

#ifndef VULKAN_ENGINE_SEMAPHORES_HPP
#define VULKAN_ENGINE_SEMAPHORES_HPP

#include <vulkan/vulkan.h>
#include <array>
#include "logical_device.hpp"
#include <stdexcept>


//semaphores are used to synchronise swap chain events (e.g. drawing and rendering)
// - semaphores are used to synchronize operations within or across command queues
// - as opposed to fences which are mainly designed to synchronize your application itself with rendering operation
template <unsigned no_semahores>
struct Semaphores {
    std::array<VkSemaphore, no_semahores> imageAvailableSemaphore{};
    std::array<VkSemaphore, no_semahores> renderFinishedSemaphore{};

    explicit Semaphores(LogicalDevice &d) : device(d) {}

    void setup()  {
        //creating semaphores is trivial
        VkSemaphoreCreateInfo semaphoreInfo{};      //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkSemaphoreCreateInfo.html
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;  //sType must be VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO

        for (auto& img_semaphore : imageAvailableSemaphore) {
            const auto image_create_res = vkCreateSemaphore(device.get_device(), &semaphoreInfo, nullptr,
                                                            &img_semaphore);
            if (image_create_res != VK_SUCCESS) {
                throw std::runtime_error("Failed to create semaphores");
            }
        }

        for (auto& rend_semaphore : renderFinishedSemaphore) {
            const auto render_create_res = vkCreateSemaphore(device.get_device(), &semaphoreInfo, nullptr,
                                                             &rend_semaphore);
            if (render_create_res != VK_SUCCESS) {
                throw std::runtime_error("Failed to create semaphores");
            }
        }

    }

    void cleanup() {
        //can be cleanuped up when no more synchronisation is required
        for (auto& rend_semaphore : renderFinishedSemaphore) {
            vkDestroySemaphore(device.get_device(), rend_semaphore, nullptr);
        }
        for (auto& img_semaphore : imageAvailableSemaphore) {
            vkDestroySemaphore(device.get_device(), img_semaphore, nullptr);
        }

    }

private:
    LogicalDevice &device;
};


#endif //VULKAN_ENGINE_SEMAPHORES_HPP
