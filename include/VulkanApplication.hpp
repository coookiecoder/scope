#pragma once

#include <stdexcept>
#include <iostream>
#include <vector>

#include <vulkan/vulkan.h>

class VulkanApplication {
    private:
        VkInstance                  instance = VK_NULL_HANDLE;
        VkPhysicalDevice            physicalDevice = VK_NULL_HANDLE;
        VkPhysicalDeviceProperties  physicalDeviceProperties = {};

        bool verbose;

        void initVulkan();
        void createInstance();
        void pickPhysicalDevice();
        void cleanUp();

    public:
        explicit VulkanApplication(bool verbose);
        ~VulkanApplication();
};
