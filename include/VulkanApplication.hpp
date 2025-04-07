#pragma once

#include <stdexcept>
#include <iostream>

#include <vulkan/vulkan.h>

class VulkanApplication {
    private:
        VkInstance instance;
        bool verbose;

        void initVulkan();
        void createInstance();
        void cleanUp();

    public:
        explicit VulkanApplication(bool verbose);
        ~VulkanApplication();
};
