#pragma once

#define NOMINMAX
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <array>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Vulkan.h"

#include "EngineException.h"
#include "Vec2D.h"
#include "Vec3D.h"

#define GFX Graphics::GetInstance()
#define GFX_EXCEPTION(note) ENGINE_EXCEPTION("Vulkan 3D Engine - Graphics Engine Exception", note)

#define MAX_FRAMES_IN_FLIGHT 2


struct Vertex
{
	bool operator==(const Vertex& other) const {
		return pos == other.pos && color == other.color && texCoord == other.texCoord;
	}

	static VkVertexInputBindingDescription GetBindingDescription()
	{
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescription()
	{
		std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

		return attributeDescriptions;
	}
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 texCoord;
};

struct UniformBufferObject
{
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
};

namespace std {
	template<> struct hash<Vertex> {
		size_t operator()(Vertex const& vertex) const {
			return ((hash<glm::vec3>()(vertex.pos) ^
				(hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertex.texCoord) << 1);
		}
	};
}

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

	void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	void CreateVertexBuffer();
	void CreateIndexBuffer();
	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	void CreateDescriptorSetLayout();
	void CreateUniformBuffers();
	void UpdateUniformBuffer(uint32_t currentImage);
	void CreateDescriptorPool();
	void CreateDescriptorSets();

	void CreateTextureImage();
	void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

	VkCommandBuffer BeginSingleTimeCommands();
	void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
	void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

	void CreateTextureImageView();
	VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
	void CreateTextureSampler();

	void CreateDepthResources();
	VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	VkFormat FindDepthFormat();
	bool HasStencilComponent(VkFormat format);

	void LoadModel();


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
	VkDescriptorSetLayout vkDescriptorSetLayout;
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

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	VkBuffer vkVertexBuffer;
	VkDeviceMemory vkVertexBufferMemory;
	VkBuffer vkIndexBuffer;
	VkDeviceMemory vkIndexBufferMemory;

	std::vector<VkBuffer> vkUniformBuffers;
	std::vector<VkDeviceMemory> vkUniformBuffersMemory;
	std::vector<void*> vkUniformBuffersMapped;

	VkDescriptorPool vkDescriptorPool;
	std::vector<VkDescriptorSet> vkDescriptorSets;

	VkBuffer vkStagingBuffer;
	VkDeviceMemory vkStagingBufferMemory;
	VkImage vkTextureImage;
	VkDeviceMemory vkTextureImageMemory;
	VkImageView vkTextureImageView;
	VkSampler vkTextureSampler;

	VkImage vkDepthImage;
	VkDeviceMemory vkDepthImageMemory;
	VkImageView vkDepthImageView;

	const std::string MODEL_PATH = "Meshes/viking_room.obj";
	const std::string TEXTURE_PATH = "Textures/viking_room.png";
};

