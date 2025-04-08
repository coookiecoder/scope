#pragma once

#include <stdexcept>
#include <iostream>
#include <optional>
#include <vector>

#include <vulkan/vulkan.h>

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
};

class VulkanApplication {
    private:
        VkInstance                  instance = VK_NULL_HANDLE;
        VkPhysicalDevice            physicalDevice = VK_NULL_HANDLE;
        VkPhysicalDeviceProperties  physicalDeviceProperties = {};
        VkDevice                    logicalDevice = VK_NULL_HANDLE;
        VkQueue                     graphicsQueue = VK_NULL_HANDLE;

        bool    verbose;

        void                initVulkan();

        void                createInstance();

        void                pickPhysicalDevice();
        bool                isDeviceUsable(const VkPhysicalDevice &device);
        QueueFamilyIndices  findQueueFamilies(const VkPhysicalDevice& device);

        void                createLogicalDevice();

        void                cleanUp();

    public:
        explicit VulkanApplication(bool verbose);
        ~VulkanApplication();
};
