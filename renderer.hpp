//
// Created by jacob on 31/10/21.
//

#ifndef VULKAN_ENGINE_RENDERER_HPP
#define VULKAN_ENGINE_RENDERER_HPP

#include <array>
#include <vector>
#include "window.hpp"
#include "debug_callback.hpp"
#include "instance.hpp"
#include "physical_device.hpp"
#include "queue_family.hpp"
#include "logical_device.hpp"
#include "surface.hpp"
#include "swap_chain.hpp"
#include "image_views.hpp"
#include "graphics_pipeline.hpp"
#include "framebuffers.hpp"
#include "command_pool.hpp"
#include "command_buffers.hpp"
#include "semaphores.hpp"
#include "fences.hpp"
#include "vertex.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"
#include "descriptor_set_layout.hpp"
#include "uniform_buffer_objects.hpp"
#include "descriptor_pool.hpp"
#include "descriptor_set.hpp"
#include "texture.hpp"

constexpr std::string_view vertex_shader_location1 = "../shader_bytecode/2D_vc_vert.spv";
constexpr std::string_view fragment_shader_location1 = "../shader_bytecode/2D_vc_frag.spv";

constexpr std::string_view vertex_shader_location2 = "../shader_bytecode/2D_vc_mvp_vert.spv";
constexpr std::string_view fragment_shader_location2 = "../shader_bytecode/2D_vc_mvp_frag.spv";

constexpr std::string_view texture_image = "../textures/statue.jpg";

struct Renderer {
    std::vector<Vertex::TWOD_VC> vertices_triangle = {
        {{0.0f, -1.0f}, {1.0f, 1.0f, 1.0f}},
        {{-1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}},
        {{-1.0f, -0.25f}, {0.0f, 1.0f, 0.0f}}
    };
    std::vector<Vertex::TWOD_VC> vertices_square = {
        {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
    };
    std::vector<uint16_t> indices_square = {
            0, 1, 2, 2, 3, 0
    };


#ifdef VALDIATION_LAYERS
    explicit Renderer(Window& w) : window(w), debug_messenger(instance), logical_device(physical_device, queue_family), queue_family(physical_device.physicalDevice, surface.surface),
            surface(window, instance), physical_device(instance, surface), swap_chain(window, logical_device, surface, queue_family),
            image_views(swap_chain, logical_device),
            graphics_pipeline1(logical_device, swap_chain, render_pass, nullptr, vertex_shader_location1,  fragment_shader_location1),
           graphics_pipeline2(logical_device, swap_chain, render_pass, &descriptor_set_layout, vertex_shader_location2,  fragment_shader_location2),
           render_pass(logical_device, swap_chain), framebuffers(logical_device, image_views, render_pass, swap_chain), command_pool(logical_device, queue_family),
           command_buffers(logical_device, command_pool, framebuffers, render_pass, swap_chain, graphics_pipeline1, graphics_pipeline2, vertex_buffer_triangle, vertex_buffer_square, index_buffer_square, descriptor_set),
                                   semaphores(logical_device), fences(logical_device), vertex_buffer_triangle(logical_device, command_pool, vertices_triangle),
            vertex_buffer_square(logical_device, command_pool, vertices_square), index_buffer_square(logical_device, command_pool, indices_square),
            descriptor_set_layout(logical_device), uniform_buffer_object(logical_device, swap_chain), descriptor_pool(logical_device, swap_chain),
                                   descriptor_set(logical_device, swap_chain, uniform_buffer_object, descriptor_pool, descriptor_set_layout), texture(logical_device, command_pool, texture_image){}
#else
    explicit Renderer(Window& w) : window(w), logical_device(physical_device, queue_family), queue_family(physical_device.physicalDevice, surface.surface),
        surface(window, instance), physical_device(instance, surface) , swap_chain(window, logical_device, surface, queue_family) ,
        image_views(swap_chain, logical_device),
       graphics_pipeline1(logical_device, swap_chain, render_pass, nullptr, vertex_shader_location1,  fragment_shader_location1),
       graphics_pipeline2(logical_device, swap_chain, render_pass, &descriptor_set_layout, vertex_shader_location2,  fragment_shader_location2),
        render_pass(logical_device, swap_chain),
        framebuffers(logical_device, image_views, render_pass, swap_chain), command_pool(logical_device, queue_family),
       command_buffers(logical_device, command_pool, framebuffers, render_pass, swap_chain, graphics_pipeline1, graphics_pipeline2, vertex_buffer_triangle, vertex_buffer_square, index_buffer_square, descriptor_set),
       semaphores(logical_device), fences(logical_device), vertex_buffer_triangle(logical_device, command_pool, vertices_triangle),
            vertex_buffer_square(logical_device, command_pool, vertices_square), index_buffer_square(logical_device, command_pool, indices_square),
            descriptor_set_layout(logical_device), uniform_buffer_object(logical_device, swap_chain), descriptor_pool(logical_device, swap_chain),
                                   descriptor_set(logical_device, swap_chain, uniform_buffer_object, descriptor_pool, descriptor_set_layout), texture(logical_device, command_pool, texture_image){}
#endif
    void initVulkan();
    void cleanup();
    void drawFrame();
    void endDrawFrame();

    //how many frames should be processed concurrently
    static constexpr unsigned max_frames_in_flight = 2;

private:
    size_t currentFrame = 0;    //used for rendering

    void recreateSwapChain();   //used whenever the window is resized

    //variables for the window
    Window& window;

    //An instance is created by describing your application and any API extensions you will be using
    Instance instance;

    //validation layers
#ifdef VALDIATION_LAYERS
    DebugMessenger debug_messenger;    //custom messenger for the validation layers
#endif

    //The graphics card to use
    PhysicalDevice physical_device;

    //the queue indices to use
    QueueFamily queue_family;

    //The interface to the graphics card
    LogicalDevice logical_device;

    //The surface to render to --- currently the GLFW window
    Surface surface;

    //The frame buffers
    SwapChain swap_chain;

    //The views into the swapchain images (needed for rendering)
    ImageViews image_views;

    //the graphics pipeline --- how all the rendering gets done
    GraphicsPipeline graphics_pipeline1;
    GraphicsPipeline graphics_pipeline2;

    //render pass -- how the framebuffer is written to
    RenderPass render_pass;

    //framebuffers -- stored the rendered images in the swap chain
    Framebuffers framebuffers;

    //UBO -- holds the data for the shader
    UniformBufferObject uniform_buffer_object;

    //descriptor set layouts -- the layout of the data being passed to the shader
    DescriptorSetLayout descriptor_set_layout;

    //descriptor pool --- holds the memory for the descriptor sets
    DescriptorPool descriptor_pool;

    //descriptor set -- like command buffers but for descriptors
    DescriptorSet descriptor_set;

    //memory to store command buffers
    CommandPool command_pool;

    //command buffers -- holds the rendering commands
    CommandBuffers command_buffers;

    //semaphores -- tell the GPU certain operations are done
    Semaphores<max_frames_in_flight> semaphores;

    //fences -- allow for CPU-GPU synchronisation
    Fences<max_frames_in_flight> fences;

    //making sure we don't render to an image that is already in flight
    std::vector<VkFence> imagesInFlight;

    //structure to hold the vertex data
    VertexBuffer<Vertex::TWOD_VC> vertex_buffer_triangle;
    VertexBuffer<Vertex::TWOD_VC> vertex_buffer_square;

    //structure to hold index data
    IndexBuffer<uint16_t> index_buffer_square;

    //structure to hold and image
    Texture texture;
};


#endif //VULKAN_ENGINE_RENDERER_HPP
