//
// Created by jacob on 5/11/21.
//

#ifndef VULKAN_ENGINE_SHADER_HPP
#define VULKAN_ENGINE_SHADER_HPP

#include <vector>
#include <string_view>
#include <vulkan/vulkan.h>
#include "logical_device.hpp"

struct Shader {
    explicit Shader(LogicalDevice & d) : device(d) {}

    //thin wrappers around the shader bytecode
    // - shaders must be wrapped in a VkShaderModule before it can be passed to the graphics pipeline
    VkShaderModule vertShaderModule{};
    VkShaderModule fragShaderModule{};

    static std::vector<char> readShader(const std::string_view& filename);  //reads the shader bytecode in from disk

    void setup(const std::string_view& vert_file, const std::string_view& frag_file);

    VkPipelineShaderStageCreateInfo get_vertex_pipeline_stage() const;
    VkPipelineShaderStageCreateInfo get_frag_pipeline_stage() const;

    void cleanup();

private:
    LogicalDevice &device;

    void createShaderModule(const std::vector<char> &code, VkShaderModule &shader_module);    //small helper function to create the VkShaderModules
};


#endif //VULKAN_ENGINE_SHADER_HPP
