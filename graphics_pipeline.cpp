//
// Created by jacob on 4/11/21.
//

#include "graphics_pipeline.hpp"

void GraphicsPipeline::setup() {
    Shader shader(device);
    shader.setup(vert_loc, frag_loc);

    const VkPipelineShaderStageCreateInfo shaderStages[] = {shader.get_vertex_pipeline_stage(), shader.get_frag_pipeline_stage()};

    shader.cleanup();
}

void GraphicsPipeline::cleanup() {

}
