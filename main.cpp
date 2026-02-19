#if defined(__INTELLISENSE__) || !defined(USE_CPP20_MODULES)
#include <vulkan/vulkan_raii.hpp>
#else
import vulkan_hpp;
#endif
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>


constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;

const std::vector<char const*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
constexpr bool enableValidationLayers = false;
#else
constexpr bool enableValidationLayers = true;
#endif



class HelloTriangleApplication {
public:
    void run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private:
    GLFWwindow* window = nullptr;
    vk::raii::Context   context;
    vk::raii::Instance  instance = nullptr;

    void initWindow() {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(WIDTH, HEIGHT, "sug", nullptr, nullptr);
    }

    void createInstance() {
        constexpr vk::ApplicationInfo appinfo{ .pApplicationName = "huh",
                      .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
                      .pEngineName = "no engine",
                      .engineVersion = VK_MAKE_VERSION(1, 0, 0),
                      .apiVersion = vk::ApiVersion14 };

        std::vector<char const*> requiredLayers;
        if (enableValidationLayers) {
            requiredLayers.assign(validationLayers.begin(), validationLayers.end());
        }

        auto layerProperties = context.enumerateInstanceLayerProperties();

        if (std::ranges::any_of(requiredLayers, [&layerProperties](auto const& requiredLayer) {
            return std::ranges::none_of(layerProperties, [requiredLayer](auto const& layerProperty) {
                return strcmp(layerProperty.layerName, requiredLayer) == 0;
                });
            }))
        {
            throw std::runtime_error("atleast 1 req layers aint supported twin");
        }

        uint32_t glfwExtensionCount = 0;
        auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        auto extensionProperties = context.enumerateInstanceExtensionProperties();
        for (uint32_t i{}; i < glfwExtensionCount; ++i) {
            if (std::ranges::none_of(extensionProperties,
                [glfwExtension = glfwExtensions[i]](auto const& extensionProperty) {
                    return strcmp(extensionProperty.extensionName, glfwExtension) == 0;
                }))
            {
                throw std::runtime_error("required glfw extension not supported: " + std::string(glfwExtensions[i]));
            }
        }


        vk::InstanceCreateInfo createInfo{
            .pApplicationInfo = &appinfo,
            .enabledExtensionCount = glfwExtensionCount,
            .ppEnabledExtensionNames = glfwExtensions};

        instance = vk::raii::Instance(context, createInfo);
    }

    void initVulkan() {
        createInstance();
    }

    void mainLoop() {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }

    }

    void cleanup() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
};

int main() {
    HelloTriangleApplication app;

    try {
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}