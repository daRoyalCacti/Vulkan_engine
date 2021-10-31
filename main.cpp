#include "rendering.hpp"

#include <iostream>
#include <stdexcept>

#define EXIT_FALURE 1
#define EXIT_SUCCESS 0

int main() {
    rendering app;

    try {
        app.initWindow();
    } catch (const std::exception& e) {
        std::cerr << "Initiating windown failed\n";
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
        app.mainLoop();
    }catch (const std::exception& e) {
        std::cerr << "Vulkan mainloop failed\n";
        std::cerr << e.what() << "\n";
        return EXIT_FALURE;
    }

    try {
        app.cleanup();
    }catch (const std::exception& e) {
        std::cerr << "Vulkan cleanup failed\n";
        std::cerr << e.what() << "\n";
        return EXIT_FALURE;
    }

    return EXIT_SUCCESS;
}