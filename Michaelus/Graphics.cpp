#include "Graphics.h"

#include "Window.h"

std::unique_ptr<Graphics> Graphics::pInstance = nullptr;

Graphics::Graphics()
{
    // Vulkan Instance Creation
    CreateInstance();
    SetupDebugMessenger();
    PickPhysicalDevice();
    CreateLogicalDevice();
}

Graphics::~Graphics()
{
    vkDestroyDevice(vkDevice, nullptr);

    if (bEnableValidationLayers)
       DestroyDebugUtilsMessengerEXT(vkInstance, vkDebugMessenger, nullptr);

    vkDestroyInstance(vkInstance, nullptr);
}

Graphics& Graphics::GetInstance()
{
    if (!pInstance) pInstance = std::make_unique<Graphics>();
    return *pInstance;
}

void Graphics::CreateInstance()
{
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Michaelus";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Vulkan 3D Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    const char* extensions[] = {
    VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
    VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
    VK_KHR_SURFACE_EXTENSION_NAME
    };

    VkInstanceCreateInfo instanceInfos{};
    instanceInfos.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfos.pApplicationInfo = &appInfo;
    instanceInfos.ppEnabledExtensionNames = extensions;
    instanceInfos.enabledExtensionCount = std::size(extensions);
    if (bEnableValidationLayers && CheckValidationLayerSupport()) {
        instanceInfos.ppEnabledLayerNames = validationLayers.data();
        instanceInfos.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    }
    else instanceInfos.enabledLayerCount = 0;

    if (VK_FAILED(vkCreateInstance(&instanceInfos, nullptr, &vkInstance)))
        throw GFX_EXCEPTION("An exception has been caught during the Vulkan Instance Creation");
}

bool Graphics::CheckValidationLayerSupport()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	bool layerFound = false;
    for (const char* layerName : validationLayers)
    {
        for (const auto& layerProperties : availableLayers)
        {
	        if (layerName == layerProperties.layerName)
	        {
                layerFound = true;
                break;
	        }
        }
    }
    return layerFound;
}

void Graphics::SetupDebugMessenger()
{
    if (!bEnableValidationLayers) return;

    VkDebugUtilsMessengerCreateInfoEXT debugInfos{};
    debugInfos.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debugInfos.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debugInfos.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debugInfos.pfnUserCallback = DebugCallback;
    debugInfos.pUserData = nullptr;

    if (FAILED(CreateDebugUtilsMessengerEXT(vkInstance, &debugInfos, nullptr, &vkDebugMessenger)))
        throw GFX_EXCEPTION("An exception has been caught during the Debug Messenger Creation");
}

void Graphics::PickPhysicalDevice()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(vkInstance, &deviceCount, nullptr);
    if (deviceCount == 0)
        throw GFX_EXCEPTION("Failed to find GPUs with Vulkan support!");

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(vkInstance, &deviceCount, devices.data());

    for (const auto& device : devices)
    {
        if (IsDeviceSuitable(device)) {
            vkPhysicalDevice = device;
            break;
        }
    }

    if (vkPhysicalDevice == VK_NULL_HANDLE)
        throw GFX_EXCEPTION("Failed to find a suitable GPU!");
}

bool Graphics::IsDeviceSuitable(VkPhysicalDevice device)
{
    QueueFamilyIndices indices = FindQueueFamilies(device);

    return indices.IsComplete();
}

void Graphics::CreateLogicalDevice()
{
    QueueFamilyIndices indices = FindQueueFamilies(vkPhysicalDevice);

    float queuePriority = 1.f;

    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = 0;
    if (bEnableValidationLayers)
    {
        createInfo.enabledExtensionCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }
    else createInfo.enabledLayerCount = 0;

    if (FAILED(vkCreateDevice(vkPhysicalDevice, &createInfo, nullptr, &vkDevice)))
        throw GFX_EXCEPTION("Failed to create logical device!");

    vkGetDeviceQueue(vkDevice, indices.graphicsFamily.value(), 0, &vkGraphicsQueue);
}

VkBool32 Graphics::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                 VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                 void* pUserData)
{
    if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        throw GFX_EXCEPTION(pCallbackData->pMessage);

    return VK_FALSE;
}
