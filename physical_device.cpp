//
// Created by jacob on 3/11/21.
//

#include "physical_device.hpp"
#include <stdexcept>
#include <map>

#include "queue_family.hpp"

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
    QueueFamily queue_family;
    queue_family.findQueueFamilies(device);

    return queue_family.has_graphics();
}
