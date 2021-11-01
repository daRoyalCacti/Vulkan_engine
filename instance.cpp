//
// Created by jacob on 1/11/21.
//

#include "instance.hpp"

#include <stdexcept>
#include <cstring>
#include <iostream>
#include "debug_callback.hpp"


void Instance::create() {
    //filling a struct with some information about out application
    VkApplicationInfo appInfo{};    //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkApplicationInfo.html
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO; //sType must be VK_STRUCTURE_TYPE_APPLICATION_INFO
    appInfo.pApplicationName = application_name;
    appInfo.applicationVersion = application_version;
    appInfo.pEngineName = engine_name;
    appInfo.engineVersion = engine_version;
    appInfo.apiVersion = vulkan_version;


    //getting the global extensions needed
    // (global means for the entire program)
    const auto extensions = getRequiredExtensions();


    //checking if all extensions requested are available
    checkExtensionSupport(extensions);

    //checking if requested validation layers are supported

#ifdef VALDIATION_LAYERS
const bool validation_layers_supported = checkValidationLayerSupport();
if (!validation_layers_supported) {
    throw std::runtime_error("the validation layers requested are not supported");
}
#endif




//telling the vulkan driver which global extensions and validation layers we want to use.
//https://www.khronos.org/registry/vulkan/ has a list of all extensions
VkInstanceCreateInfo createInfo{};  //https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkInstanceCreateInfo.html
createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;  //sType must be VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO
createInfo.pApplicationInfo = &appInfo;
//extensions
createInfo.enabledExtensionCount = extensions.size();    //number of global extensions to enable
createInfo.ppEnabledExtensionNames = extensions.data();       //pointer to an array of strings containing the names of extensions to enable
//validation layers

#ifdef VALDIATION_LAYERS
createInfo.enabledLayerCount = validation_layers.size();       //number of global layers to enable (validation layers)
createInfo.ppEnabledLayerNames = validation_layers.data();      //the list of the validation layers to use

//the regular validation debug caller requires an instance
//as such it is created after the instance and destroyed before it
//so now we pass the debug caller in here
VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
getDebugCallbackSettings(debugCreateInfo);

//actually setting the value in the struct
createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
#else
createInfo.enabledLayerCount = 0;       //number of global layers to enable (validation layers)
createInfo.pNext = nullptr;
#endif




//actually creating the instance
//https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/vkCreateInstance.html
const auto create_instance_err = vkCreateInstance(&createInfo, nullptr, &instance); //nullptr is for custom memory allocation
if (create_instance_err != VK_SUCCESS) {
    throw std::runtime_error("failed to create instance");
}

}



void Instance::checkExtensionSupport(const std::vector<const char*> &requried_extensions) {
    unsigned extension_count = 0;   //value to hold the number of supported extensions
    //querying the number of supported extensions
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);

    std::vector<VkExtensionProperties> extensions(extension_count); //array to store all the supported extensions
    //querying all the supported extensions
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());

    bool have_all_extensions = true;
    bool curr_extension_supported;
    //iterating throw all required extensions and seeing if they're in the list of supported extensions
    for (const auto & curr_extension : requried_extensions) {
        curr_extension_supported = false;
        for (const auto &ex : extensions) {
            if (std::strcmp(curr_extension, ex.extensionName) == 0) {   //if the extension is in the list of supported extensions
                curr_extension_supported = true;
                break;
            }
        }

        if (!curr_extension_supported) {
            have_all_extensions = false;
            std::cerr << "do not have extension " << curr_extension << "\n";
        }



    }

    if (!have_all_extensions) {
        throw std::runtime_error("requested extensions not supported");
    }
}

#ifdef VALDIATION_LAYERS
bool Instance::checkValidationLayerSupport() {
    unsigned layer_count = 0;   //store to total number of validation layers available
    //querying the number of validation layers
    vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

    std::vector<VkLayerProperties> available_layers(layer_count);   //stores all validation layers
    //querying all validation layers
    vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());


    //iterating throw all required extensions and seeing if they're in the list of supported extensions
    bool layer_found;
    for (const char* layerName : validation_layers) {   //validation_layers defined in main struct
        layer_found = false;

        for (const auto& layerProperties : available_layers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {//if the layer is in the list of available layers
                layer_found = true;
                break;
            }
        }

        if (!layer_found) {
            return false;
        }
    }

    return true;
}
#endif

std::vector<const char*> Instance::getRequiredExtensions() {
    //first getting the required extensions for GLFW to work
    unsigned glfw_extension_count = 0;
    const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

    //putting the glw extensions into a vector to return
    std::vector<const char*> extensions(glfw_extensions, glfw_extensions + glfw_extension_count);


    //extensions needed for validation layers
    //we need VK_EXT_debug_utils to use the custom debug messengers
#ifdef VALDIATION_LAYERS
extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);    //VK_EXT_DEBUG_UTILS_EXTENSION_NAME is a macro for "VK_EXT_debug_utils". https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VK_EXT_debug_utils.html
#endif


return extensions;
}