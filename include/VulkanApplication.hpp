#pragma once

#include <stdexcept>
#include <iostream>
#include <fstream>
#include <optional>
#include <vector>
#include <set>
#include <algorithm>
#include <cstring>

#include <vulkan/vulkan.h>

#include <SFML/Window/Window.hpp>
#include <SFML/Window/Vulkan.hpp>

#include <glm/glm.hpp>

struct Vertex {
    glm::vec2 pos;
    glm::vec3 color;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        return attributeDescriptions;
    }
};

const std::vector<Vertex> vertices = {
    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
    {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
};

const std::vector<uint16_t> indices = {
    0, 1, 2, 2, 3, 0
};


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

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

constexpr int MAX_FRAMES_IN_FLIGHT = 2;

class VulkanApplication {
    private:
        sf::Window                  &window;
        VkInstance                  instance = VK_NULL_HANDLE;
        VkDebugUtilsMessengerEXT    debugMessenger = VK_NULL_HANDLE;
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
        std::vector<VkCommandBuffer>commandBuffer;
        std::vector<VkSemaphore>    imageAvailableSemaphore;
        std::vector<VkSemaphore>    renderFinishedSemaphore;
        std::vector<VkFence>        inFlightFence;
        VkBuffer                    vertexBuffer = VK_NULL_HANDLE;
        VkDeviceMemory              vertexBufferMemory = VK_NULL_HANDLE;
        VkBuffer                    indexBuffer = VK_NULL_HANDLE;
        VkDeviceMemory              indexBufferMemory = VK_NULL_HANDLE;

        bool                        verbose;
        int                         currentFrame = 0;
        bool                        frameBufferResized = false;
        bool                        swapChainState = false;

        void                        initVulkan();
        bool                        checkValidationLayerSupport();
        VkResult                    CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
        void                        DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
        void                        populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        void                        setupDebugMessenger();

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
        void                        recreateSwapChain();

        void                        createImageViews();

        void                        createRenderPass();

        void                        createGraphicsPipeline();
        VkShaderModule              createShaderModule(const std::vector<char>& code) const;

        void                        createFrameBuffers();

        void                        createCommandPool();

        void                        createVertexBuffer();
        void                        createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        uint32_t                    findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        void                        copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

        void                        createCommandBuffer();
        void                        recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

        void                        createSyncObjects();

        void                        cleanUp();
        void                        cleanupSwapChain();

    public:
        explicit                    VulkanApplication(bool verbose, sf::Window& window);

        ~VulkanApplication();

        void                        drawFrame();
		void						wait();
        void                        triggerResize();
};
