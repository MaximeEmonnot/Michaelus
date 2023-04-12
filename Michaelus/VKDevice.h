#pragma once
#include "Vulkan.h"

#include <memory>

#include <optional>
#include <vector>

// Macro pour l'utilisation plus ais�e de la classe VKDevice
#define VK_DEVICE VKDevice::GetInstance()

/*
 * Donn�es indispensables pour les files graphique et de pr�sentation
 */
struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool IsComplete() const
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

/*
 * Donn�es indispensables pour le SwapChain
 */
struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

/*
 * Classe VKDevice
 * D�finition du Device Vulkan
 * Centralise une majorit� du syst�me Vulkan pour l'initialisation de beaucoup d'�l�ments Vulkan
 * Classe tr�s technique, sans r�elle utilisation de patron de conception si ce n'est un Singleton
 * La lecture de cette classe peut �tre int�ressante, mais �galement tr�s longue si l'on veut tout comprendre
 */
class VKDevice
{
public:
	// Constructeur (publique car std::unique_ptr n�cessite un constructeur publique) 
	VKDevice();
	// Destructeur par d�faut
	~VKDevice() = default;

	// Ces diff�rents constructeurs sont d�finis comme �tant supprim�s, pour correspondre au patron de conception Singleton
	// et �galement � la RO5 des normes du C++
	VKDevice(const VKDevice&) = delete;
	VKDevice& operator= (const VKDevice&) = delete;
	VKDevice(VKDevice&&) = delete;
	VKDevice& operator= (VKDevice&&) = delete;

	// M�thode du patron de conception Singleton
	static VKDevice& GetInstance();

	// Destructeur r�el pour contr�ler la lib�ration de m�moire
	void Destroy();

	// R�cup�ration du Device Vulkan
	VkDevice GetDevice() const;
	// R�cup�ration du Device Physique (Carte graphique)
	VkPhysicalDevice GetPhysicalDevice() const;
	// R�cup�ration de la surface de rendu
	VkSurfaceKHR GetSurface() const;
	// R�cup�ration de la file graphique
	VkQueue GetGraphicsQueue() const;
	// R�cup�ration de la file de pr�sentation
	VkQueue GetPresentQueue() const;
	// R�cup�ration de la Pool de commandes
	VkCommandPool GetCommandPool() const;
	// R�cup�ration des Samples MSAA (Multi sampling des textures)
	VkSampleCountFlagBits GetMSAASamples() const;

	// R�cup�ration des d�tails pour le support du SwapChain
	SwapChainSupportDetails GetSwapChainSupport();
	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
	QueueFamilyIndices FindPhysicalQueueFamilies();
	VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const;

	// Cr�ation centralis�e d'un Buffer
	void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	// Copie centralis�e d'un Buffer
	void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	// Copie centralis�e d'un Buffer vers une Image
	void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	// D�marrage d'un CommandBuffer pour le rendu
	VkCommandBuffer BeginSingleTimeCommands();
	// Fin d'un CommandBuffer pour le rendu
	void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
	// Cr�ation centralis�e d'une Image
	void CreateImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	// Cr�ation centralis�e d'une vue vers une Image
	VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
	// Transition centralis�e d'un Layout d'Image
	void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
	// G�n�ration centralis�e des MipMaps pour les textures
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