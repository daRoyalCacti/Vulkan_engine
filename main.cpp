#include "renderer.hpp"

#include <iostream>
#include <stdexcept>

#define EXIT_FALURE 1
#define EXIT_SUCCESS 0

int main() {
    Window window;
    Renderer app(window);

    try {
        window.makeWindow();
    } catch (const std::exception& e) {
        std::cerr << "Initiating window failed\n";
        std::cerr << e.what() << "\n";
        return EXIT_FALURE;
    }

    try {
        app.initVulkan();
    } catch (const std::exception& e) {
        std::cerr << "Initiating vulkan failed\n";
        std::cerr << e.what() << "\n";
        return EXIT_FALURE;
    }

    try {

        //having the main loop run units the (x) is pressed
        while (!glfwWindowShouldClose(window.get_window())) {
            glfwPollEvents();   //checking for events (like (x) being pressed)
        }

    }catch (const std::exception& e) {
        std::cerr << "Vulkan mainloop failed\n";
        std::cerr << e.what() << "\n";
        return EXIT_FALURE;
    }

    try {
        app.cleanup();
        window.cleanup();
    }catch (const std::exception& e) {
        std::cerr << "Vulkan cleanup failed\n";
        std::cerr << e.what() << "\n";
        return EXIT_FALURE;
    }

    return EXIT_SUCCESS;
}