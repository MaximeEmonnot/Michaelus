#pragma once
#include "Vulkan.h"

#include <memory>

#include <optional>
#include <vector>

#define VK_DEVICE VKDevice::GetInstance()

struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool IsComplete() const
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};


class VKDevice
{
public:
	VKDevice();
	~VKDevice();

	VKDevice(const VKDevice&) = delete;
	VKDevice& operator= (const VKDevice&) = delete;
	VKDevice(VKDevice&&) = delete;
	VKDevice& operator= (VKDevice&&) = delete;

	static VKDevice& GetInstance();

	void Destroy();

	VkDevice GetDevice() const;
	VkPhysicalDevice GetPhysicalDevice() const;
	VkSurfaceKHR GetSurface() const;
	VkQueue GetGraphicsQueue() const;
	VkQueue GetPresentQueue() const;
	VkCommandPool GetCommandPool() const;
	VkSampleCountFlagBits GetMSAASamples() const;

	SwapChainSupportDetails GetSwapChainSupport();
	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
	QueueFamilyIndices FindPhysicalQueueFamilies();
	VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const;

	void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	VkCommandBuffer BeginSingleTimeCommands();
	void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
	void CreateImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
	void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
	void GenerateMipMaps(VkImage image, VkFormat format, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);

private:
	void CreateInstance();
	void SetupDebugMessenger();
	void PickPhysicalDevice();
	void CreateLogicalDevice();
	void CreateSurface();
	void CreateCommandPool();

	bool IsDeviceSuitable(VkPhysicalDevice device);
	bool CheckValidationLayerSupport();
	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
	bool HasStencilComponent(VkFormat format);

	VkSampleCountFlagBits GetMaxUsableSampleCount();

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);

private:
	VkPhysicalDeviceProperties vkProperties;

private:
	static std::unique_ptr<VKDevice> pInstance;

#ifdef NDEBUG
		const bool bEnableValidationLayers = false;
#else
	const bool bEnableValidationLayers = true;
#endif

	VkInstance vkInstance;
	VkDebugUtilsMessengerEXT vkDebugMessenger;
	VkPhysicalDevice vkPhysicalDevice = VK_NULL_HANDLE;
	VkCommandPool vkCommandPool;

	VkDevice vkDevice;
	VkSurfaceKHR vkSurface;
	VkQueue vkGraphicsQueue;
	VkQueue vkPresentQueue;

	VkSampleCountFlagBits vkMSAASamples = VK_SAMPLE_COUNT_1_BIT;

	const std::vector<const char*> vkValidationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

	const std::vector<const char*> vkDeviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME,
		VK_KHR_VARIABLE_POINTERS_EXTENSION_NAME
	};
};