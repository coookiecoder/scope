#pragma once

#include <stdexcept>
#include <iostream>
#include <fstream>
#include <optional>
#include <vector>
#include <set>
#include <algorithm>

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

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
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
        VkSwapchainKHR              swapChain = VK_NULL_HANDLE;
        std::vector<VkImage>        swapChainImages;
        VkFormat                    swapChainImageFormat = VK_FORMAT_UNDEFINED;
        VkExtent2D                  swapChainExtent = {};
        std::vector<VkImageView>    swapChainImageViews;
        VkRenderPass                renderPass = VK_NULL_HANDLE;
        VkPipelineLayout            pipelineLayout = VK_NULL_HANDLE;
        VkPipeline                  graphicsPipeline = VK_NULL_HANDLE;
        std::vector<VkFramebuffer>  swapChainFrameBuffers;
        VkCommandPool               commandPool = VK_NULL_HANDLE;

        bool                        verbose;

        void                        initVulkan();

        void                        createInstance();

        void                        createSurface();

        void                        pickPhysicalDevice();

        //all the following are just here to check the gpu can do the bare minimum

        bool                        isDeviceUsable(const VkPhysicalDevice &device) const;
        static bool                 checkDeviceExtensionSupport(VkPhysicalDevice device);
        QueueFamilyIndices          findQueueFamilies(const VkPhysicalDevice& device) const;
        SwapChainSupportDetails     querySwapChainSupport(VkPhysicalDevice device) const;
        static VkSurfaceFormatKHR   chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        static VkPresentModeKHR     chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D                  chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;

        void                        createLogicalDevice();

        void                        createSwapChain();

        void                        createImageViews();

        void                        createRenderPass();

        void                        createGraphicsPipeline();
        VkShaderModule              createShaderModule(const std::vector<char>& code) const;

        void                        createFrameBuffers();

        void                        createCommandPool();

        void                        cleanUp();

    public:
        explicit                    VulkanApplication(bool verbose, sf::Window& window);

        ~VulkanApplication();
};
