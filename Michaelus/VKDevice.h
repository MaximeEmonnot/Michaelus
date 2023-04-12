#pragma once
#include "Vulkan.h"

#include <memory>

#include <optional>
#include <vector>

// Macro pour l'utilisation plus aisée de la classe VKDevice
#define VK_DEVICE VKDevice::GetInstance()

/*
 * Données indispensables pour les files graphique et de présentation
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
 * Données indispensables pour le SwapChain
 */
struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

/*
 * Classe VKDevice
 * Définition du Device Vulkan
 * Centralise une majorité du système Vulkan pour l'initialisation de beaucoup d'éléments Vulkan
 * Classe très technique, sans réelle utilisation de patron de conception si ce n'est un Singleton
 * La lecture de cette classe peut être intéressante, mais également très longue si l'on veut tout comprendre
 */
class VKDevice
{
public:
	// Constructeur (publique car std::unique_ptr nécessite un constructeur publique) 
	VKDevice();
	// Destructeur par défaut
	~VKDevice() = default;

	// Ces différents constructeurs sont définis comme étant supprimés, pour correspondre au patron de conception Singleton
	// et également à la RO5 des normes du C++
	VKDevice(const VKDevice&) = delete;
	VKDevice& operator= (const VKDevice&) = delete;
	VKDevice(VKDevice&&) = delete;
	VKDevice& operator= (VKDevice&&) = delete;

	// Méthode du patron de conception Singleton
	static VKDevice& GetInstance();

	// Destructeur réel pour contrôler la libération de mémoire
	void Destroy();

	// Récupération du Device Vulkan
	VkDevice GetDevice() const;
	// Récupération du Device Physique (Carte graphique)
	VkPhysicalDevice GetPhysicalDevice() const;
	// Récupération de la surface de rendu
	VkSurfaceKHR GetSurface() const;
	// Récupération de la file graphique
	VkQueue GetGraphicsQueue() const;
	// Récupération de la file de présentation
	VkQueue GetPresentQueue() const;
	// Récupération de la Pool de commandes
	VkCommandPool GetCommandPool() const;
	// Récupération des Samples MSAA (Multi sampling des textures)
	VkSampleCountFlagBits GetMSAASamples() const;

	// Récupération des détails pour le support du SwapChain
	SwapChainSupportDetails GetSwapChainSupport();
	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
	QueueFamilyIndices FindPhysicalQueueFamilies();
	VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const;

	// Création centralisée d'un Buffer
	void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	// Copie centralisée d'un Buffer
	void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	// Copie centralisée d'un Buffer vers une Image
	void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	// Démarrage d'un CommandBuffer pour le rendu
	VkCommandBuffer BeginSingleTimeCommands();
	// Fin d'un CommandBuffer pour le rendu
	void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
	// Création centralisée d'une Image
	void CreateImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	// Création centralisée d'une vue vers une Image
	VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
	// Transition centralisée d'un Layout d'Image
	void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
	// Génération centralisée des MipMaps pour les textures
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