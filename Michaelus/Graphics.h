#pragma once

#include <vector>

#include "Vulkan.h"

#include "EngineException.h"

#define GFX Graphics::GetInstance()
#define GFX_EXCEPTION(note) ENGINE_EXCEPTION("Vulkan 3D Engine - Graphics Engine Exception", note)

#define MAX_FRAMES_IN_FLIGHT 2

class Graphics
{
public:
	Graphics();
	~Graphics();
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	Graphics(Graphics&&) = delete;
	Graphics& operator=(Graphics&&) = delete;

	static Graphics& GetInstance();

	void Draw();

	void SetFrameBufferResize();

private:
	void CreateInstance();
	bool CheckValidationLayerSupport();
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
	void SetupDebugMessenger();
	void PickPhysicalDevice();
	bool IsDeviceSuitable(VkPhysicalDevice device);
	void CreateLogicalDevice();
	void CreateSurface();
	void CreateSwapChain();
	void CreateImageViews();
	void CreateGraphicsPipeline();
	void CreateRenderPass();
	void CreateFrameBuffers();
	void CreateCommandPool();
	void CreateCommandBuffers();
	void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
	void CreateSynchronizationObjects();

	void RecreateSwapChain();
	void CleanUpSwapChain();

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

	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME,
		VK_KHR_VARIABLE_POINTERS_EXTENSION_NAME
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
	VkDevice vkDevice;
	VkQueue vkGraphicsQueue;
	VkQueue vkPresentQueue;
	VkSwapchainKHR vkSwapChain;
	std::vector<VkImage> vkSwapChainImages;
	VkFormat vkSwapChainImageFormat;
	VkExtent2D vkSwapChainExtent;
	std::vector<VkImageView> vkSwapChainImageViews;

	VkRenderPass vkRenderPass;
	VkPipelineLayout vkPipelineLayout;
	VkPipeline vkGraphicsPipeline;

	std::vector<VkFramebuffer> vkSwapChainFrameBuffers;

	VkCommandPool vkCommandPool;
	std::vector<VkCommandBuffer> vkCommandBuffers;

	std::vector<VkSemaphore> vkImageAvailableSemaphores;
	std::vector<VkSemaphore> vkRenderFinishedSemaphores;
	std::vector<VkFence> vkInFlightFences;

	bool bFrameBufferResized = false;

	uint32_t currentFrame = 0;
};

