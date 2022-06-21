//
// Created by jacob on 20/11/21.
//

#ifndef VULKAN_ENGINE_VERTEX_HPP
#define VULKAN_ENGINE_VERTEX_HPP

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <array>

namespace Vertex{

    struct TWOD_VC {
        glm::vec2 pos;
        glm::vec3 color;

        //getting the data about the rate to load data from memory
        // - see also vertex_input.hpp
        static VkVertexInputBindingDescription getBindingDescription() {
            VkVertexInputBindingDescription bindingDescription{};          //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkVertexInputBindingDescription.html
            bindingDescription.binding = 0;                                 //the binding number (the index in the array of bindings)
                                                                            // - you should have an array of binding descriptions if your vertex data has multiple forms
            bindingDescription.stride = sizeof(Vertex::TWOD_VC);                     //the stride between elements in the buffer   (number of bytes between data entries)
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;     //whether to move to the next data entry after each vertex or each instance.
                                                                            // - https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkVertexInputRate.html
                                                                            //not using instanced rendering RN so want the next vertex

            return bindingDescription;
        }

        //how to extract each vertex attribute (i.e. the "location =" in the vertex shader) per binding description
        // - see also vertex_input.hpp
        static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
            std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};   //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkVertexInputAttributeDescription.html

            //specifying the position
            attributeDescriptions[0].binding = 0;                           //binding (which binding description to use) -- i.e. the index of the binding returned by getBindingDescription
            attributeDescriptions[0].location = 0;                          //location (the "location =" number in the vertex shader)
                                                                            // - in this case position data is location 0 in the vertex shader
            attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;      //the format of the data
                                                                            // - see  https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkFormat.html
                                                                            //just want a vec2 for position data
            attributeDescriptions[0].offset = offsetof(Vertex::TWOD_VC, pos);        //the offset (where the data starts in the array of data)

            //specifying the colors
            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;                          //color data is location 1 in vertex shader
            attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;   //want a vec3 for color data
            attributeDescriptions[1].offset = offsetof(Vertex::TWOD_VC, color);

            return attributeDescriptions;
        }



    };


    struct TWOD_VT {
        glm::vec2 pos;
        glm::vec2 tex;

        //getting the data about the rate to load data from memory
        // - see also vertex_input.hpp
        static VkVertexInputBindingDescription getBindingDescription() {
            VkVertexInputBindingDescription bindingDescription{};          //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkVertexInputBindingDescription.html
            bindingDescription.binding = 0;                                 //the binding number (the index in the array of bindings)
            // - you should have an array of binding descriptions if your vertex data has multiple forms
            bindingDescription.stride = sizeof(Vertex::TWOD_VT);                     //the stride between elements in the buffer   (number of bytes between data entries)
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;     //whether to move to the next data entry after each vertex or each instance.
            // - https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkVertexInputRate.html
            //not using instanced rendering RN so want the next vertex

            return bindingDescription;
        }

        //how to extract each vertex attribute (i.e. the "location =" in the vertex shader) per binding description
        // - see also vertex_input.hpp
        static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
            std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};   //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkVertexInputAttributeDescription.html

            //specifying the position
            attributeDescriptions[0].binding = 0;                           //binding (which binding description to use) -- i.e. the index of the binding returned by getBindingDescription
            attributeDescriptions[0].location = 0;                          //location (the "location =" number in the vertex shader)
            // - in this case position data is location 0 in the vertex shader
            attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;      //the format of the data
            // - see  https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkFormat.html
            //just want a vec2 for position data
            attributeDescriptions[0].offset = offsetof(Vertex::TWOD_VT, pos);        //the offset (where the data starts in the array of data)

            //specifying the colors
            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;                          //color data is location 1 in vertex shader
            attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;      //want a vec2 for texture data
            attributeDescriptions[1].offset = offsetof(Vertex::TWOD_VT, tex);

            return attributeDescriptions;
        }



    };


}

#endif //VULKAN_ENGINE_VERTEX_HPP
