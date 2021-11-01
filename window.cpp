//
// Created by jacob on 1/11/21.
//

#include "window.hpp"
#include <stdexcept>

void Window::makeWindow() {
    //initialize the GLFW library
    ////================================================================
    const auto glfw_init_err = glfwInit();
    //checking errors
    if (glfw_init_err == GLFW_FALSE) {
        throw std::runtime_error("could not initialze glfw");
    }

    //setting some properties of glfw
    //these will be set for the next call to glfwCreateWindow
    ////================================================================
    //need to call NO_API because GLFW was designed for openGL
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    //disabling the ability to resize the window for now
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);


    //creating the window
    ////================================================================
    //window, window_width, window_height, and window_name are all defined iin the main struct
    //that last nullptr is in reference to openGL
    //the second last nullptr refers to the a monitor. It is GLFWmonitor*.
    // - see https://www.glfw.org/docs/latest/monitor_guide.html#monitor_object
    window = glfwCreateWindow(window_width, window_height, window_name, nullptr, nullptr);
}

void Window::cleanup() {
    //destroying the window and its context.
    glfwDestroyWindow(window);
    //destroys all remaining windows and cursors, restores any modified gamma ramps,
    //frees any allocated resources created by GLFW
    glfwTerminate();
}