#pragma once
#include "Vulkan.h"

#include <string>

class VKTexture
{
public:
	VKTexture(const std::string& texturePath);
	~VKTexture();

	VkImage GetTextureImage() const;
	VkImageView GetTextureImageView() const;
	VkSampler GetTextureSampler() const;

private:
	void CreateTextureImage(const std::string& texturePath);
	void CreateTextureImageView();
	void CreateTextureSampler();

private:
	VkImage vkTextureImage;
	VkDeviceMemory vkTextureImageMemory;
	VkImageView vkTextureImageView;
	VkSampler vkTextureSampler;
	uint32_t vkMipLevels;
};

