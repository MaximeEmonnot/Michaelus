#pragma once

#include "Vulkan.h"

#include <vector>


#define MAX_FRAMES_IN_FLIGHT 2

class Mesh;
class VKDescriptor;
class VKModel;
class VKPipeLine;

class VKSwapChain
{
public:
	VKSwapChain();

	~VKSwapChain();

	VKSwapChain(const VKSwapChain&) = delete;
	VKSwapChain& operator= (const VKSwapChain&) = delete;
	VKSwapChain(VKSwapChain&&) = delete;
	VKSwapChain& operator= (VKSwapChain&&) = delete;

	void Destroy();

	VkFramebuffer GetFrameBuffer(int index) const;
	VkRenderPass GetRenderPass() const;
	VkImageView GetImageView(int index) const;
	size_t GetImageCount() const;
	VkFormat GetSwapChainImageFormat() const;
	VkExtent2D GetSwapChainExtent() const;
	uint32_t GetWidth() const;
	uint32_t GetHeight() const;

	float GetExtentAspectRatio() const;

	VkFormat FindDepthFormat();

	void BeginDraw();
	void EndDraw(VkCommandBuffer* commandBuffers, std::vector<std::shared_ptr<Mesh>> meshes);
	void RecordCommandBuffer(VkCommandBuffer commandBuffer, std::vector<std::shared_ptr<Mesh>> meshes);
	void SubmitCommandBuffers(const VkCommandBuffer* pCommandBuffers);
	bool operator== (const VKSwapChain& rhs) const;

private:
	void CreateSwapChain();
	void CreateImageViews();
	void CreateColorResources();
	void CreateDepthResources();
	void CreateRenderPass();
	void CreateFrameBuffers();
	void CreateSynchronizationObjects();

	void CleanUpSwapChain();
	void RecreateSwapChain();

	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

private:
	VkSwapchainKHR vkSwapChain;
	VkFormat vkSwapChainImageFormat;
	VkFormat vkSwapChainDepthFormat;
	VkExtent2D vkSwapChainExtent;

	std::vector<VkImage> vkColorImages;
	std::vector<VkDeviceMemory> vkColorImageMemories;
	std::vector<VkImageView> vkColorImageViews;

	std::vector<VkImage> vkDepthImages;
	std::vector<VkDeviceMemory> vkDepthImageMemories;
	std::vector<VkImageView> vkDepthImageViews;

	std::vector<VkImage> vkSwapChainImages;
	std::vector<VkImageView> vkSwapChainImageViews;

	VkRenderPass vkRenderPass;

	std::vector<VkFramebuffer> vkSwapChainFrameBuffers;

	std::vector<VkSemaphore> vkImageAvailableSemaphores;
	std::vector<VkSemaphore> vkRenderFinishedSemaphores;
	std::vector<VkFence> vkInFlightFences;

	uint32_t currentFrame = 0;
	uint32_t imageIndex = 0;
};

