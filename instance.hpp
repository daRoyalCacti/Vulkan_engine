//
// Created by jacob on 1/11/21.
//

#ifndef VULKAN_ENGINE_INSTANCE_HPP
#define VULKAN_ENGINE_INSTANCE_HPP

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <array>
#include <vector>

//An instance is created by describing your application and any API extensions you will be using
struct Instance {
#ifdef VALDIATION_LAYERS
    static constexpr std::array<const char*, 1> validation_layers = {
            "VK_LAYER_KHRONOS_validation"
    };
    //checking that all requested validation layers are available
    static bool checkValidationLayerSupport();
#endif

    //check that extensions requested are indeed supported
    static void checkExtensionSupport(const std::vector<const char*> &requried_extensions);

    //https://www.khronos.org/registry/vulkan/ has a list of all extensions
    static constexpr std::array<const char*, 0> instance_extensions = {};

    //getting the required extensions
    // currently those needed by GLFW and by validation layers
    static std::vector<const char*> getRequiredExtensions();

    void create();

    inline void cleanup() const {vkDestroyInstance(instance, nullptr);}   //nullptr is for custom memory allocation


    [[nodiscard]] inline VkInstance const& get_instance() const {
        return instance;
    }

private:
    VkInstance instance{};
    const char* application_name = "Vulkan";
    const unsigned application_version = VK_MAKE_VERSION(1, 0, 0);  //developer-supplied version number of the application
    const char* engine_name = "no_engine";  //the name of the engine used to create the application (its default is NULL)
    const unsigned engine_version = VK_MAKE_VERSION(1, 0, 0);   //the version number of the engine
    const unsigned vulkan_version = VK_API_VERSION_1_0; //the highest version of vulkan that the application is designed to use

};


#endif //VULKAN_ENGINE_INSTANCE_HPP
