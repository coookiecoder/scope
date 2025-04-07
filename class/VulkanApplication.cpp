#include "../include/VulkanApplication.hpp"

void VulkanApplication::initVulkan() {
    if (this->verbose)
        std::cout << "Creating vulkan instance" << std::endl;
    createInstance();
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

void VulkanApplication::cleanUp() {
    if (this->verbose)
        std::cout << "Destroying vulkan instance" << std::endl;
    vkDestroyInstance(this->instance, nullptr);
}

VulkanApplication::VulkanApplication(bool verbose) :instance(nullptr), verbose(verbose) {
    this->initVulkan();
}

VulkanApplication::~VulkanApplication() {
    this->cleanUp();
}
