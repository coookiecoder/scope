#include "../include/VulkanApplication.hpp"

void VulkanApplication::initVulkan() {
    if (this->verbose)
        std::cout << "Creating vulkan instance" << std::endl;
    this->createInstance();
    if (this->verbose)
        std::cout << "Creating surface instance" << std::endl;
    this->createSurface();
    if (this->verbose)
        std::cout << "Picking a physical device for vulkan" << std::endl;
    this->pickPhysicalDevice();
    if (this->verbose)
        std::cout << "Creating a logical device and queue" << std::endl;
    this->createLogicalDevice();
}

void VulkanApplication::createInstance() {
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Scope";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    if (vkCreateInstance(&createInfo, nullptr, &this->instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance");
    }
}

void VulkanApplication::createSurface() {
    if (this->window.createVulkanSurface(this->instance, this->surface) != VK_SUCCESS)
        std::cout << "Failed to create surface" << std::endl;
}


void VulkanApplication::pickPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(this->instance, &deviceCount, nullptr);

    if (deviceCount == 0)
        throw std::runtime_error("failed to find GPUs with Vulkan support!");

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(this->instance, &deviceCount, devices.data());

    for (const auto& device : devices) {
        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        if (isDeviceUsable(device)) {
            this->physicalDevice = device;
            break;
        }
    }

    if (this->physicalDevice == VK_NULL_HANDLE)
        throw std::runtime_error("failed to find a suitable GPU!");

    vkGetPhysicalDeviceProperties(this->physicalDevice, &this->physicalDeviceProperties);

    if (this->verbose) {
        std::cout << "using : " << this->physicalDeviceProperties.deviceName << std::endl;
        std::cout << "api version : " << this->physicalDeviceProperties.apiVersion << std::endl;
        std::cout << "device type : " << this->physicalDeviceProperties.deviceType << std::endl;
        std::cout << "driver version : " << this->physicalDeviceProperties.driverVersion << std::endl;
        std::cout << "device ID : " << this->physicalDeviceProperties.deviceID << std::endl;
        std::cout << "vendor ID : " << this->physicalDeviceProperties.vendorID << std::endl;
    }
}

bool VulkanApplication::isDeviceUsable(const VkPhysicalDevice &device) {
    const QueueFamilyIndices indices = this->findQueueFamilies(device);

    return indices.graphicsFamily.has_value();
}

QueueFamilyIndices VulkanApplication::findQueueFamilies(const VkPhysicalDevice& device) {
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        if (indices.graphicsFamily.has_value()) {
            break;
        }

        i++;
    }

    return indices;
}

void VulkanApplication::createLogicalDevice() {
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
    queueCreateInfo.queueCount = 1;
    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = 0;
    createInfo.enabledLayerCount = 0;

    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &this->logicalDevice) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }

    vkGetDeviceQueue(this->logicalDevice, indices.graphicsFamily.value(), 0, &this->graphicsQueue);
}


void VulkanApplication::cleanUp() {
    if (this->verbose)
        std::cout << "Destroying logical device" << std::endl;
    vkDestroyDevice(this->logicalDevice, nullptr);
    if (this->verbose)
        std::cout << "Destroying surface instance" << std::endl;
    vkDestroySurfaceKHR(this->instance, this->surface, nullptr);
    if (this->verbose)
        std::cout << "Destroying vulkan instance" << std::endl;
    vkDestroyInstance(this->instance, nullptr);
}

VulkanApplication::VulkanApplication(bool verbose, sf::Window &window) : window(window), verbose(verbose) {
    this->initVulkan();
}

VulkanApplication::~VulkanApplication() {
    this->cleanUp();
}
