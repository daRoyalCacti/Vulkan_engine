//
// Created by jacob on 4/11/21.
//

#define ALPHA_BLENDING

#include "graphics_pipeline.hpp"
#include "graphics_pipeline/vertex_input.hpp"
#include "graphics_pipeline/input_assembly.hpp"
#include "graphics_pipeline/viewport.hpp"
#include "graphics_pipeline/scissor.hpp"
#include "graphics_pipeline/rasterizer.hpp"
#include "graphics_pipeline/multisampling.hpp"
#include "graphics_pipeline/color_blend.hpp"
#include "graphics_pipeline/pipeline_layout.hpp"

#include <stdexcept>

void GraphicsPipeline::setup() {
    //setting the uniforms and push constants in the shader
    PipelineLayout pipeline_info(0, nullptr,0, nullptr);

    //creating the pipeline
    const auto pipeline_layout_create_res = vkCreatePipelineLayout(device.get_device(), &pipeline_info.get_pipeline_stage(), nullptr, &pipeline_layout);   //pipeline_layout decleared in main header
    if (pipeline_layout_create_res != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }

    Shader shader(device);
    shader.setup(vert_loc, frag_loc);

    const VkPipelineShaderStageCreateInfo shaderStages[] = {shader.get_vertex_pipeline_stage(), shader.get_frag_pipeline_stage()};


    //specifying the settings for all fixed functions of the graphics pipeline
    //========================================================================
    //where in the frame buffer to render to
    Viewport viewpot(static_cast<float>(swap_chain.extent.width), static_cast<float>(swap_chain.extent.height) );
    //the region rendered to to actually display
    Scissor scissor(swap_chain.extent);

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewpot.get_pipeline_stage();
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor.get_pipeline_stage();

    //how the vertex are layed out
    VertexInput vertex_input(0, nullptr, 0, nullptr);
    //the type of data being rendered (e.g. triangles or lines)
    InputAssembly input_assembly(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
    //the rasterizing settings
    Rasterizer rasterizer(VK_POLYGON_MODE_FILL, 1.0f, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_CLOCKWISE);
    //the multisampling settings (how much to multisample if at all)
    Multisample multisample(VK_SAMPLE_COUNT_1_BIT, VK_FALSE, 1.0f);

#ifdef ALPHA_BLENDING
    ColorBlendAttachmentState color_blend_attachment_state(VK_TRUE, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
                                       VK_BLEND_OP_ADD, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD);
#else
    ColorBlendAttachmentState color_blend_attachment_state(VK_FALSE, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ZERO,
                                                       VK_BLEND_OP_ADD, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ZERO,VK_BLEND_OP_ADD);
#endif
    ColorBlendState color_blend_state(color_blend_attachment_state);

    //can also set the Dynamic stateVkRenderPass
    //https://vulkan-tutorial.com/Drawing_a_triangle/Graphics_pipeline_basics/Fixed_functions

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;            //the number of shaders in the pStages array (i.e. how many shaders)
    pipelineInfo.pStages = shaderStages;    //pointer to an array of VkPipelineShaderStageCreateInfo structures (i.e. the array of shaders)
    //referencing all the fixed function settings
    pipelineInfo.pVertexInputState = &vertex_input.get_pipeline_stage();
    pipelineInfo.pInputAssemblyState = &input_assembly.get_pipeline_stage();
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer.get_pipeline_stage();
    pipelineInfo.pMultisampleState = &multisample.get_pipeline_stage();
    pipelineInfo.pDepthStencilState = nullptr; // not currently used
    pipelineInfo.pColorBlendState = &color_blend_state.get_pipeline_stage();
    pipelineInfo.pDynamicState = nullptr; // no currently used
    //description of the binding locations (i.e. how uniforms work)
    pipelineInfo.layout = pipeline_layout;
    //description of the render passes
    pipelineInfo.renderPass = render_pass.get_render_pass();
    pipelineInfo.subpass = 0;               //index of the subpass where this graphics pipeline will be used
    //for pipelines derived from other pipelines
    // - it is less expensive to set up pipelines that have similar functionality
    //to use these, need to set `.flag' to VK_PIPELINE_CREATE_DERIVATIVE_BIT
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;   //handle to an existing pipeline
    pipelineInfo.basePipelineIndex = -1;                //reference another pipeline that is about to be created

    //acutally creating the pipeline
    // - can create multiple pipelines all in a single call
    const auto pipeline_create_res = vkCreateGraphicsPipelines(device.get_device(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &grahics_pipeline);
    if (pipeline_create_res != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline!");
    }

    //destroying the shader modules
    shader.cleanup();
}

void GraphicsPipeline::cleanup() {
    vkDestroyPipeline(device.get_device(), grahics_pipeline, nullptr);
    vkDestroyPipelineLayout(device.get_device(), pipeline_layout, nullptr);
}
