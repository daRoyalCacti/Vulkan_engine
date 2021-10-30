cmake_minimum_required(VERSION 3.20)
project(Vulkan_engine)

find_package(Vulkan)
find_package(glfw3 3.3 REQUIRED)

set(CMAKE_CXX_STANDARD 20)

add_executable(Vulkan_engine main.cpp)



target_link_libraries(Vulkan_engine glfw)
target_link_libraries(Vulkan_engine Vulkan::Vulkan)