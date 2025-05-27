#pragma once

#include <stdexcept>
#include <iostream>
#include <fstream>
#include <optional>
#include <vector>
#include <set>
#include <algorithm>
#include <cstring>
#include <unordered_map>
#include <random>

#include <vulkan/vulkan.h>

#include <SFML/Window/Window.hpp>
#include <SFML/Window/Vulkan.hpp>

#include "../include/Obj.hpp"
#include "../include/stb_image.h"

#include "../template/Matrix.tpp"

struct Vertex {
    cookie::Vector3D<float> pos;
    cookie::Vector3D<float> color;
    cookie::Vector2D<float> texCoord;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

        return attributeDescriptions;
    }

    bool operator==(const Vertex& other) const {
        return pos == other.pos && color == other.color && texCoord == other.texCoord;
    }
};

inline void hashCombine(std::size_t& seed, std::size_t value) {
    seed ^= value + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

namespace std {
    template<>
    struct hash<Vertex> {
        size_t operator()(const Vertex& vertex) const noexcept {
            size_t seed = 0;
            // Manually hash each float component if cookie::Vector* lacks hashing
            hashCombine(seed, std::hash<float>()(vertex.pos.x));
            hashCombine(seed, std::hash<float>()(vertex.pos.y));
            hashCombine(seed, std::hash<float>()(vertex.pos.z));

            hashCombine(seed, std::hash<float>()(vertex.color.x));
            hashCombine(seed, std::hash<float>()(vertex.color.y));
            hashCombine(seed, std::hash<float>()(vertex.color.z));

            hashCombine(seed, std::hash<float>()(vertex.texCoord.x));
            hashCombine(seed, std::hash<float>()(vertex.texCoord.y));

            return seed;
        }
    };
}

struct UniformBufferObject {
    cookie::Matrix4D<float> model;
    cookie::Matrix4D<float> view;
    cookie::Matrix4D<float> proj;
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
        VkDescriptorSetLayout       descriptorSetLayout = VK_NULL_HANDLE;
        std::vector<VkBuffer>       uniformBuffers;
        std::vector<VkDeviceMemory> uniformBuffersMemory;
        std::vector<void*>          uniformBuffersMapped;
        VkDescriptorPool            descriptorPool = VK_NULL_HANDLE;
        std::vector<VkDescriptorSet>descriptorSets;
        VkImage                     textureImage = VK_NULL_HANDLE;
        VkDeviceMemory              textureImageMemory = VK_NULL_HANDLE;
        VkImageView                 textureImageView = VK_NULL_HANDLE;
        VkSampler                   textureSampler = VK_NULL_HANDLE;
        VkImage                     dummyTextureImage = VK_NULL_HANDLE;
        VkDeviceMemory              dummyTextureImageMemory = VK_NULL_HANDLE;
        VkImageView                 dummyTextureImageView = VK_NULL_HANDLE;
        VkSampler                   dummyTextureSampler = VK_NULL_HANDLE;
        VkImage                     depthImage = VK_NULL_HANDLE;
        VkDeviceMemory              depthImageMemory = VK_NULL_HANDLE;
        VkImageView                 depthImageView = VK_NULL_HANDLE;
        std::vector<Vertex>         vertices;
        std::vector<uint32_t>       indices;

        bool                        verbose;
        int                         currentFrame = 0;
        bool                        frameBufferResized = false;
        bool                        swapChainState = false;
        std::string                 texturePath;
        const Obj&                  obj;
        const float                 map_Kd[3] = {255.0, 255.0, 255.0};

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

        void                        createDescriptorSetLayout();

        void                        createGraphicsPipeline();
        VkShaderModule              createShaderModule(const std::vector<char>& code) const;

        void                        createFrameBuffers();

        void                        createCommandPool();

        void                        createDepthResources();
        VkFormat                    findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
        VkFormat                    findDepthFormat();
        bool                        hasStencilComponent(VkFormat format);

        void                        createTextureImage();
        void                        createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
        VkCommandBuffer             beginSingleTimeCommands();
        void                        endSingleTimeCommands(VkCommandBuffer commandBuffer);
        void                        transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
        void                        copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

        void                        createTextureImageView();
        VkImageView                 createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

        void                        createTextureSampler();
        void                        createDummyTexture();

        void                        loadModel();

        void                        createVertexBuffer();
        void                        createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        uint32_t                    findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        void                        copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

        void                        createIndexBuffer();

        void                        createUniformBuffers();

        void                        createDescriptorPool();

        void                        createDescriptorSets();

        void                        createCommandBuffer();
        void                        recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

        void                        createSyncObjects();

        void                        cleanUp();
        void                        cleanupSwapChain();

        void                        updateUniformBuffer(uint32_t currentImage);
    public:
        explicit                    VulkanApplication(bool verbose, sf::Window& window, std::string texturePath, const Obj& obj);
        explicit                    VulkanApplication(bool verbose, sf::Window& window, const cookie::Vector3D<float>& Kd, const Obj& obj);

        ~VulkanApplication();

        void                        drawFrame();
		void						wait();
        void                        triggerResize();

        float                       zoom = 2.0f;
        bool                        useTexture = false;
        bool                        updateTexture = false;
};
