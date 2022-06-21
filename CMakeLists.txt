cmake_minimum_required(VERSION 3.20)
project(Vulkan_engine)

find_package(Vulkan REQUIRED)
find_package(glfw3 3.3 REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release")



add_executable(Vulkan_engine main.cpp renderer.cpp renderer.hpp window.cpp window.hpp instance.cpp instance.hpp debug_callback.cpp debug_callback.hpp physical_device.cpp physical_device.hpp queue_family.cpp queue_family.hpp logical_device.cpp logical_device.hpp surface.cpp surface.hpp swap_chain_details.cpp swap_chain_details.hpp swap_chain.cpp swap_chain.hpp image_views.cpp image_views.hpp graphics_pipeline.hpp graphics_pipeline/shader.cpp graphics_pipeline/shader.hpp graphics_pipeline/vertex_input.hpp graphics_pipeline/input_assembly.hpp graphics_pipeline/viewport.hpp graphics_pipeline/scissor.hpp graphics_pipeline/rasterizer.hpp graphics_pipeline/multisampling.hpp graphics_pipeline/color_blend.hpp graphics_pipeline/pipeline_layout.hpp render_pass.cpp render_pass.hpp framebuffers.cpp framebuffers.hpp command_pool.cpp command_pool.hpp command_buffers.cpp command_buffers.hpp semaphores.hpp fences.hpp vertex.hpp vertex_buffer.hpp buffer.hpp buffer.cpp index_buffer.hpp uniform_buffer_objects.hpp descriptor_set_layout.cpp descriptor_set_layout.hpp uniform_buffer_objects.cpp descriptor_pool.cpp descriptor_pool.hpp descriptor_set.cpp descriptor_set.hpp texture.cpp texture.hpp texture_view.cpp texture_view.hpp texture_sampler.cpp texture_sampler.hpp)

target_link_libraries(Vulkan_engine glfw)
target_link_libraries(Vulkan_engine Vulkan::Vulkan)

IF(CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DVALDIATION_LAYERS>)
ENDIF(CMAKE_BUILD_TYPE MATCHES Debug)
add_definitions(-DGLM_FORCE_RADIANS)
add_definitions(-DGLM_FORCE_DEFAULT_ALIGNED_GENTYPES)



set(warning_flags -Werror -Wpedantic -Wall -Wextra -Wfloat-equal -Wundef -Wcast-align -Wwrite-strings -Wlogical-op -Wmissing-declarations -Wredundant-decls -Wshadow -Woverloaded-virtual -Wmissing-include-dirs -Wunknown-pragmas -Wduplicated-cond -Wfloat-equal -Wshadow -Wunsafe-loop-optimizations -Wpacked -Wsuggest-attribute=pure -Wsuggest-attribute=const -Wsuggest-attribute=noreturn -Wmissing-noreturn -Wsuggest-attribute=malloc -Wsuggest-attribute=format -Wmissing-format-attribute -Wsuggest-attribute=cold>)
IF(CMAKE_BUILD_TYPE MATCHES Debug)
    add_compile_options(${warning_flags}>)
ENDIF(CMAKE_BUILD_TYPE MATCHES Debug)
IF(CMAKE_BUILD_TYPE MATCHES Release)
    add_compile_options(${warning_flags} -Ofast -fno-math-errno -funsafe-math-optimizations -ffinite-math-only -march=native -mfma)
ENDIF(CMAKE_BUILD_TYPE MATCHES Release)


