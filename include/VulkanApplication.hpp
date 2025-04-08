#pragma once

#include <stdexcept>
#include <iostream>
#include <optional>
#include <vector>
#include <set>

#include <vulkan/vulkan.h>
#include <SFML/Window/Window.hpp>

#include <SFML/Window/Vulkan.hpp>

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

class VulkanApplication {
    private:
        sf::Window                  &window;
        VkInstance                  instance = VK_NULL_HANDLE;
        VkPhysicalDevice            physicalDevice = VK_NULL_HANDLE;
        VkPhysicalDeviceProperties  physicalDeviceProperties = {};
        VkDevice                    logicalDevice = VK_NULL_HANDLE;
        VkQueue                     graphicsQueue = VK_NULL_HANDLE;
        VkQueue                     presentQueue = VK_NULL_HANDLE;
        VkSurfaceKHR                surface = VK_NULL_HANDLE;

        bool    verbose;

        void                initVulkan();

        void                createInstance();

        void                createSurface();

        void                pickPhysicalDevice();
        bool                isDeviceUsable(const VkPhysicalDevice &device);
        bool                checkDeviceExtensionSupport(VkPhysicalDevice device);
        QueueFamilyIndices  findQueueFamilies(const VkPhysicalDevice& device);

        void                createLogicalDevice();

        void                cleanUp();

    public:
        explicit VulkanApplication(bool verbose, sf::Window& window);
        ~VulkanApplication();
};
