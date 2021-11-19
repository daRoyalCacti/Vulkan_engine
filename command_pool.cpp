//
// Created by jacob on 19/11/21.
//

#include "command_pool.hpp"
#include "queue_family.hpp"
#include <stdexcept>

void CommandPool::setup() {
    VkCommandPoolCreateInfo poolInfo{}; //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkCommandPoolCreateInfo.html
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;                //sType must be VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO
    poolInfo.queueFamilyIndex = queue_family.graphicsFamily.value();            //the queue that the command buffers submit to
                                                                                //doing graphics operations so allocating to the graphics queue
    poolInfo.flags = 0;             //possible flags:
                                    // - VK_COMMAND_POOL_CREATE_TRANSIENT_BIT: Hint that command buffers are rerecorded with new commands very often (may change memory allocation behavior)
                                    // - VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT: Allow command buffers to be rerecorded individually, without this flag they all have to be reset together
                                    //we are only recording 1 command buffer at the beginning of the program so none of these are relevant
    const auto create_res = vkCreateCommandPool(device.get_device(), &poolInfo, nullptr, &command_pool);
    if (create_res != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool!");
    }
}

void CommandPool::cleanup() {
    vkDestroyCommandPool(device.get_device(), command_pool, nullptr);
}
