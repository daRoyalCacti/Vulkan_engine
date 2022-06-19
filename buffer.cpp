//
// Created by jacob on 27/11/21.
//

#include "buffer.hpp"
#include <stdexcept>


//need to match the memory requirements of the buffer with those offered by the graphics cards
//to find the right type of memory to use
//
//The typeFilter parameter will be used to specify the bit field of memory types that are suitable
unsigned findMemoryType(PhysicalDevice &physisical_device, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    //finding the memory offered by the graphics card
    VkPhysicalDeviceMemoryProperties memProperties;     //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkPhysicalDeviceMemoryProperties.html
    vkGetPhysicalDeviceMemoryProperties(physisical_device.get_device(), &memProperties);
    //The VkPhysicalDeviceMemoryProperties structure has two arrays memoryTypes and memoryHeaps.
    //- Memory heaps are distinct memory resources like dedicated VRAM and swap space in RAM for when VRAM runs out.
    // - The different types of memory exist within these heaps.
    //    > Right now we'll only concern ourselves with the type of memory and not the heap it comes from, but you can imagine that this can affect performance.

    //finding a suitable memory type for the buffer
    for (unsigned i = 0; i < memProperties.memoryTypeCount; i++) {
        //looping through all memory types supported and checking if the memory type is suitable
        // - because typeFilter is a bit field this means checking if the bit is 1
        const auto mem_type_suitable = typeFilter & (1 << i);
        //checking if we can write the vertex data into the memory
        // - the memoryTypes array specifies properties of each memory type
        // - these properties contain the ability to be written to from the CPU
        //because we can more than 1 required property, we should not just check if the bitwise and is not 0
        // - we need to compare it to properties to make sure we got both of them
        // - (not needed for mem_type_suitable because we only need one element in the bitmap, here we need all elements in the bitmap)
        const auto mem_has_prop = (memProperties.memoryTypes[i].propertyFlags & properties) == properties;
        //checking if the memory has the desired property
        if ( mem_type_suitable &&  mem_has_prop) {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}



void SingleTimeCommandBuffer::begin_recording() {
    //information for allocating the command buffer
    VkCommandBufferAllocateInfo allocInfo{};                            //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkCommandBufferAllocateInfo.html
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;   //sType must be VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;                  //should the command buffer be primary or secondary
    // - VK_COMMAND_BUFFER_LEVEL_PRIMARY: Can be submitted to a queue for execution, but cannot be called from other command buffers
    // - VK_COMMAND_BUFFER_LEVEL_SECONDARY: Cannot be submitted directly, but can be called from primary command buffers.
    //secondary command buffers are useful for reusing common operations
    allocInfo.commandPool = command_pool.get_command_pool();            //the command pool with which to allocate the command buffer
    allocInfo.commandBufferCount = 1;                                   //the number of command buffers to allocate (only need 1)

    vkAllocateCommandBuffers(device.get_device(), &allocInfo, &commandBuffer);

    //start recording the copying commands
    // -  all commands that are to be recorded have the vkCmd prefix
    VkCommandBufferBeginInfo beginInfo{};                           //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkCommandBufferBeginInfo.html
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;  //sType must be VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;  //specifies the buffers usage - https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkCommandBufferUsageFlagBits.html
    // - VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT: The command buffer will be rerecorded right after executing it once.
    // - VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT: This is a secondary command buffer that will be entirely within a single render pass.
    // - VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT: The command buffer can be resubmitted while it is also already pending execution.
    //it is good practice to tell the driver that we are only going to be using this command buffer once
    beginInfo.pInheritanceInfo = nullptr;                           //only relevant to secondary command buffers
    // - It specifies which state to inherit from the calling primary command buffers.
    //https://khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkCommandBufferInheritanceInfo.html

    //start recording the command buffers
    // - If the command buffer was already recorded once, then a call to vkBeginCommandBuffer will implicitly reset it.
    // - It's not possible to append commands to a buffer at a later time.
    // - commands can either be inline or secondary: https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkSubpassContents.html
    //    > VK_SUBPASS_CONTENTS_INLINE: The render pass commands will be embedded in the primary command buffer itself and no secondary command buffers will be executed
    //    > VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS: The render pass commands will be executed from secondary command buffers.
    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }
}

void SingleTimeCommandBuffer::execute_recording() {
    //end recording the command buffer
    vkEndCommandBuffer(commandBuffer);

    //submitting the command buffer
    // - want to perform the copy right now
    // - there are no semaphores or fences to wait on
    VkSubmitInfo submitInfo{};                          //https://khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkSubmitInfo.html
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;   //sType must be VK_STRUCTURE_TYPE_SUBMIT_INFO
    submitInfo.commandBufferCount = 1;                  //the number of command buffers to execute (only the 1 we just created)
    submitInfo.pCommandBuffers = &commandBuffer;        //the array of command buffers to execute (only the 1 we just created)

    //submitting the command buffer to the graphics queue
    // - this is slightly suboptimal and there should be a dedicated transfer queue
    // - see Transfer queue int the link below for how to do this
    //   > https://vulkan-tutorial.com/Vertex_buffers/Staging_buffer
    vkQueueSubmit(device.graphics_queue, 1, &submitInfo, VK_NULL_HANDLE);

    //waiting for this copy to complete
    // - We could use a fence and wait with vkWaitForFences
    // - A fence would allow you to schedule multiple transfers simultaneously and wait for all of them complete, instead of executing one at a time.
    // - That may give the driver more opportunities to optimize.
    vkQueueWaitIdle(device.graphics_queue);

    //want to cleanup up the command buffer right now
    // - this is normally done when the program finishes but it doesn't make sense to keep this buffer around for the lifetime of the program

    vkFreeCommandBuffers(device.get_device(), command_pool.get_command_pool(), 1, &commandBuffer); //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkFreeCommandBuffers.html
}



//size is size in bytes
//very few of these buffers should be created. On high end graphics cards there is a maximum of around 4000 possible.
//Should create one big buffers and use offsets to access the data inside it
void create_buffer(LogicalDevice &device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
    VkBufferCreateInfo bufferInfo{};                            //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkBufferCreateInfo.html
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;    //sType must be VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO
    bufferInfo.size = size;                                     //the size in bytes of the buffer to create
    bufferInfo.usage = usage;                                   //bitmask of the allowed usage of the buffer
    // - https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkBufferUsageFlagBits.html
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;         //how the buffer is shared between multiple queue families
    // - https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkSharingMode.html


    if (vkCreateBuffer(device.get_device(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }

    //allocating memory to the buffer
    //===============================
    //first querying the memory requirements
    // - Graphics cards can offer different types of memory to allocate from. Each type of memory varies in terms of allowed operations and performance characteristics
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device.get_device(), buffer, &memRequirements);
    //The VkMemoryRequirements struct has three fields:
    //    size: The size of the required amount of memory in bytes, may differ from bufferInfo.size.
    //    alignment: The offset in bytes where the buffer begins in the allocated region of memory, depends on bufferInfo.usage and bufferInfo.flags.
    //    memoryTypeBits: Bit field of the memory types that are suitable for the buffer.

    //actually allocating the memory
    VkMemoryAllocateInfo allocInfo{};                               //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkMemoryAllocateInfo.html
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;       //sType must be VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO
    allocInfo.allocationSize = memRequirements.size;                //the size of the allocation in bytes
    allocInfo.memoryTypeIndex = findMemoryType(device.physical_device, memRequirements.memoryTypeBits, properties); //index of the memory type
    //using the find memory type helper function to find the index of the best memory type

    if (vkAllocateMemory(device.get_device(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    //associating the memory with the buffer
    vkBindBufferMemory(device.get_device(), buffer, bufferMemory, 0);
    //the fourth parameter is the offset within the region of memory.
    //Since this memory is allocated specifically for this the vertex buffer, the offset is simply 0.
    // - If the offset is non-zero, then it is required to be divisible by memRequirements.alignment.
}


void copyBuffer(LogicalDevice &device, CommandPool& command_pool, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
    SingleTimeCommandBuffer command_buffer(device, command_pool);
    command_buffer.begin_recording();

    //specifying how to copy from one buffer to the other
    VkBufferCopy copyRegion{};  //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkBufferCopy.html
    copyRegion.srcOffset = 0;   //starting offset in bytes from the start of srcBuffer
                                // - want to copy the entire buffer so this is 0
    copyRegion.dstOffset = 0;   //starting offset in bytes from the start of dstBuffer
                                // - could maybe be taken as an argument to the function
    copyRegion.size = size;     //starting offset in bytes from the start of dstBuffer.
    //actually copying the data between the buffers
    // - https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCmdCopyBuffer.html
    vkCmdCopyBuffer(command_buffer.get_buffer(), srcBuffer, dstBuffer, 1, &copyRegion);
    //the last argument is technically an array of regions to copy
    //the second to last argument is how many regions there are

    command_buffer.execute_recording();
}

