//
// Created by jacob on 3/11/21.
//

#include "physical_device.hpp"
#include <stdexcept>
#include <map>
#include <set>

#include "queue_family.hpp"
#include "swap_chain_details.hpp"

void PhysicalDevice::setup() {
    //finding the number of device that can be used with our instance
    unsigned device_count = 0;
    vkEnumeratePhysicalDevices(instance.get_instance(), &device_count, nullptr);    //must take an instance because the graphics card must support the features sepecified in the instance

    //if there are no cards with the required features, no point in continuing
    if (device_count== 0) {
        throw std::runtime_error("Failed to find GPUs that support the vulkan features");
    }

    //finding all the devices that can be used
    std::vector<VkPhysicalDevice> devices(device_count);
    vkEnumeratePhysicalDevices(instance.get_instance(), &device_count, devices.data());

    std::multimap<int, VkPhysicalDevice> device_scores;     //structure holding the score given to each device
                                                            //cannot be map because keys could be the same

    for (const auto& device : devices) {
        device_scores.insert( {rateDeviceSuitability(device), device}  );
    }

    if (device_scores.crbegin()->first != 0) {  //rateDeviceSuitability returns 0 if the device is not suitable
        physicalDevice = device_scores.crbegin()->second;
    } else {//if no suitable device was found
        throw std::runtime_error("Failed to find a suitable GPU");
    }


}

unsigned PhysicalDevice::rateDeviceSuitability(VkPhysicalDevice device) {
    if (!isDeviceSuitable(device)) {
        return 0;
    }

    //getting the device properties
    //these include:
    // - device type
    // - device name
    // - driver version
    //see https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkPhysicalDeviceProperties.html
    VkPhysicalDeviceProperties device_properties;   //struct holding the properties of a physical device
    vkGetPhysicalDeviceProperties(device, &device_properties);  //querying the device properties

    //getting the features of the device
    //these include:
    // - geometry shader
    // - tessellation shader
    // - multiViewport
    // - variable Multisample Rate
    // see https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkPhysicalDeviceFeatures.html
    VkPhysicalDeviceFeatures deviceFeatures;    //struct holding the features of a physical device
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);   //querying the features

    unsigned score = 0; //score given to device to determine its quality

    //prioritize choose discrete graphics
    //then integrated, then whatever else
    // https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkPhysicalDeviceType.html
    if (device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score += 1e3;
    } else if (device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) {
        score += 1e2;
    } else {
        score += 1e1;
    }




    return score;
}

bool PhysicalDevice::isDeviceSuitable(VkPhysicalDevice device) {
    QueueFamily queue_family(device, surface.get_surface());
    queue_family.setup();
    const auto queue_family_good = queue_family.has_graphics() && queue_family.has_present();

    const auto extensions_supported = checkDeviceExtensionSupport(device);

    //checking if there is an available swapchain to use
    bool swap_chain_good = false;
    if (extensions_supported) {
        SwapChainDetails swap_chain_details;
        swap_chain_details.query_swap_chain_support(device, surface.get_surface());
        swap_chain_good = true;
    }

    //checking if anisotopy is supported (for textures)
    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(device, &supportedFeatures);
    const auto anisotropy_good = supportedFeatures.samplerAnisotropy;


    return extensions_supported && queue_family_good && swap_chain_good && anisotropy_good;
}

bool PhysicalDevice::checkDeviceExtensionSupport(VkPhysicalDevice device) {
    //finding all the extensions supported
    uint32_t extensionCount;    //the number of extensions supported
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);    //querying the number of supported extensions

    std::vector<VkExtensionProperties> availableExtensions(extensionCount); //array to hold all the supported extensions. VkExtensionProperties holds the name of the extension as well as its version
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data()); //querying the supported extensions

    //checking to make sure add required extensions are supported
    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end()); //create a copy of the extensions to then removed elements from

    //removing elements from the required extensions until all supported extensions are checked
    for (const auto& ext : availableExtensions) {
        requiredExtensions.erase(ext.extensionName);    //removing the extension based on the extensions name (do not care about the extension version)
    }

    const bool all_supported = requiredExtensions.empty();

    return all_supported;
}
