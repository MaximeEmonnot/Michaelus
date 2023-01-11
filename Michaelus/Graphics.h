#pragma once

#include <vector>

#include "Vulkan.h"

#include "EngineException.h"

#define GFX Graphics::GetInstance();
#define GFX_EXCEPTION(note) ENGINE_EXCEPTION("Vulkan 3D Engine - Graphics Engine Exception", note)

class Graphics
{
public:
	Graphics();
	~Graphics();

	static Graphics& GetInstance();

private:
	void CreateInstance();
	bool CheckValidationLayerSupport();
	void SetupDebugMessenger();
	void PickPhysicalDevice();
	bool IsDeviceSuitable(VkPhysicalDevice device);

	void CreateLogicalDevice();

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);

private:
	static std::unique_ptr<Graphics> pInstance;

	const std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

#ifdef NDEBUG
	const bool bEnableValidationLayers = false;
#else
	const bool bEnableValidationLayers = true;
#endif

	VkInstance vkInstance;
	VkDebugUtilsMessengerEXT vkDebugMessenger;
	VkPhysicalDevice vkPhysicalDevice = VK_NULL_HANDLE;

	VkSurfaceKHR vkSurface;
	VkSurfaceFormatKHR vkSurfaceFormat;
	VkDevice vkDevice;
	VkQueue vkGraphicsQueue;
	VkSwapchainKHR vkSwapchain;
	VkCommandPool vkCommandPool;

	VkSemaphore vkAquireSemaphore;
	VkSemaphore vkSubmitSemaphore;
};

