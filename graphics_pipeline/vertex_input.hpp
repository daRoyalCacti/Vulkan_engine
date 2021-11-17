//
// Created by jacob on 6/11/21.
//

#ifndef VULKAN_ENGINE_VERTEX_INPUT_HPP
#define VULKAN_ENGINE_VERTEX_INPUT_HPP

#include <vulkan/vulkan.h>

//describes the format of the vertex data that will be passed to the vertex shader
struct VertexInput {
    //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkPipelineVertexInputStateCreateInfo.html
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};

    VertexInput(const uint32_t no_bindings, const VkVertexInputBindingDescription* bindings, const uint32_t no_attributes, const VkVertexInputAttributeDescription* attributes) {
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;  //sType must be VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO
        vertexInputInfo.vertexBindingDescriptionCount = no_bindings;        //the number of bindings. i.e. the size of the pVertexBindingDescriptions array
        vertexInputInfo.pVertexBindingDescriptions = bindings;              //is an array of VkVertexInputBindingDescription
                                                                            // - see https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkVertexInputBindingDescription.html
                                                                            //Which rate to load data from memory
                                                                            // - array of structs that holds
                                                                            //    * the binding number (the index in the array of bindings)
                                                                            //    * the stride between elements in the buffer   (number of bytes between data entries)
                                                                            //    * whether to move to the next data entry after each vertex or each instance.
                                                                            //      >  https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkVertexInputRate.html
                                                                            //you should have an array of binding descriptions if your vertex data has multiple forms
                                                                            // - e.g. colour with alpha and without
                                                                            // - then when you draw the meshes you should bind the appropriate binding description
        vertexInputInfo.vertexAttributeDescriptionCount = no_attributes;    //the number of attributes. i.e. the size of the pVertexAttributeDescriptions array
        vertexInputInfo.pVertexAttributeDescriptions = attributes;          //is an array of VkVertexInputAttributeDescription
                                                                            // - see https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkVertexInputAttributeDescription.html
                                                                            //how to extract each vertex attribute (i.e. the "location =" in the vertex shader) per binding description
                                                                            // - how to pass the information into the shader for each layout of the data
                                                                            //    * location (the "location =" number in the vertex shader)
                                                                            //    * binding (which binding description to use)
                                                                            //    * the format of the data
                                                                            //       > see  https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkFormat.html
                                                                            //       > mainly just is it a float, or a vector of floats or an int, etc.
                                                                            //    * the offset (where the data starts in the array of data)
                                                                            //       > i.e. in (vertex, tex-coords, normals), what is the offset of the normals
    }


    [[nodiscard]] VkPipelineVertexInputStateCreateInfo& get_pipeline_stage() {return vertexInputInfo;}
};


#endif //VULKAN_ENGINE_VERTEX_INPUT_HPP
