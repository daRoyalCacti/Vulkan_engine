cmake_minimum_required(VERSION 3.20)
project(Vulkan_engine)

find_package(Vulkan)
find_package(glfw3 3.3 REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release")


add_executable(Vulkan_engine main.cpp renderer.cpp renderer.hpp window.cpp window.hpp instance.cpp instance.hpp debug_callback.cpp debug_callback.hpp physical_device.cpp physical_device.hpp queue_family.cpp queue_family.hpp logical_device.cpp logical_device.hpp)

target_link_libraries(Vulkan_engine glfw)
target_link_libraries(Vulkan_engine Vulkan::Vulkan)

add_definitions(-DGLFW_INCLUDE_VULKAN)
IF(CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DVALDIATION_LAYERS>)
ENDIF(CMAKE_BUILD_TYPE MATCHES Debug)



set(warning_flags -Werror -Wpedantic -Wall -Wextra -Wfloat-equal -Wundef -Wcast-align -Wwrite-strings -Wlogical-op -Wmissing-declarations -Wredundant-decls -Wshadow -Woverloaded-virtual -Wmissing-include-dirs -Wunknown-pragmas -Wduplicated-cond -Wfloat-equal -Wshadow -Wunsafe-loop-optimizations -Wpacked -Wsuggest-attribute=pure -Wsuggest-attribute=const -Wsuggest-attribute=noreturn -Wmissing-noreturn -Wsuggest-attribute=malloc -Wsuggest-attribute=format -Wmissing-format-attribute -Wsuggest-attribute=cold>)
IF(CMAKE_BUILD_TYPE MATCHES Debug)
    add_compile_options(${warning_flags}>)
ENDIF(CMAKE_BUILD_TYPE MATCHES Debug)
IF(CMAKE_BUILD_TYPE MATCHES Release)
    add_compile_options(${warning_flags} -Ofast -fno-math-errno -funsafe-math-optimizations -ffinite-math-only -march=native -mfma)
ENDIF(CMAKE_BUILD_TYPE MATCHES Release)


