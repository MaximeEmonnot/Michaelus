#include "Graphics.h"

#include "Window.h"

std::unique_ptr<Graphics> Graphics::pInstance = nullptr;

Graphics::Graphics()
{
    VkResult result = VK_SUCCESS;

    // Vulkan Instance Creation
    CreateInstance();
}

Graphics::~Graphics()
{
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

VkBool32 Graphics::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                 VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                 void* pUserData)
{
    if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        throw GFX_EXCEPTION(pCallbackData->pMessage);

    return VK_FALSE;
}
