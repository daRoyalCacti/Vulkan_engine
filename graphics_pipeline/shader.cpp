//
// Created by jacob on 5/11/21.
//

#include "shader.hpp"
#include <fstream>
#include <stdexcept>

std::vector<char> Shader::readShader(const std::string_view &filename) {
    std::ifstream file(filename.data(), std::ios::ate | std::ios::binary);  //::ate so get the file size for free

    if (!file.is_open()) {
        throw std::runtime_error("failed to read shader");
    }

    //creating a vector that is the same size as the file
    const auto file_size = static_cast<long>(file.tellg());
    std::vector<char> ret_vec(file_size);

    //returning to the beginning of the file and start reading
    file.seekg(0);
    file.read(ret_vec.data(), file_size);

    file.close();
    return ret_vec;
}

//taking the VkShaderModule by reference, rather than returning a new VkShaderModule is because
// VkShaderModules contain the byte code for the shader. This means that VkShaderModule could be a very large object
void Shader::createShaderModule(const std::vector<char> &code, VkShaderModule &shader_module) {
    //specifying the shader bytecode to create the module from
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO; //sType must be VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO
    createInfo.codeSize = code.size();  //number of bytes in the shader
    createInfo.pCode = reinterpret_cast<const unsigned*>(code.data()); //the bytecode. The default allocator for std::vector ensures that this reinterpret_cast will work

    const auto create_res = vkCreateShaderModule(device.get_device(), &createInfo, nullptr, &shader_module);
    if (create_res != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module");
    }
}


void Shader::setup(const std::string_view &vert_file, const std::string_view &frag_file) {
    const auto vert_shader_code = readShader(vert_file);
    const auto frag_shader_code = readShader(frag_file);

    createShaderModule(vert_shader_code, vertShaderModule);
    createShaderModule(frag_shader_code, fragShaderModule);
}

void Shader::cleanup() {
    vkDestroyShaderModule(device.get_device(), fragShaderModule, nullptr);
    vkDestroyShaderModule(device.get_device(), vertShaderModule, nullptr);
}

VkPipelineShaderStageCreateInfo Shader::get_vertex_pipeline_stage() const {
    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;    //sType must be VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT; //which pipeline stage the shader is going to be used
    vertShaderStageInfo.module = vertShaderModule;  //the module containing the shader code
    vertShaderStageInfo.pName = "main";     //the function to invoke in the code --- the entrypoint
                                            //it is possible to have one shader with multiple entrypoints
    vertShaderStageInfo.pSpecializationInfo = nullptr;  //allows you to specify constants in the shader at compile time --- see https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkSpecializationInfo.html

    return vertShaderStageInfo;
}

VkPipelineShaderStageCreateInfo Shader::get_frag_pipeline_stage() const {
    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;    //sType must be VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT; //which pipeline stage the shader is going to be used
    fragShaderStageInfo.module = fragShaderModule;  //the module containing the shader code
    fragShaderStageInfo.pName = "main";     //the function to invoke in the code --- the entrypoint
    //it is possible to have one shader with multiple entrypoints
    fragShaderStageInfo.pSpecializationInfo = nullptr;  //allows you to specify constants in the shader at compile time --- see https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkSpecializationInfo.html

    return fragShaderStageInfo;
}
