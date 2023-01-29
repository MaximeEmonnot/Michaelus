#pragma once
#include "Vulkan.h"

#include <vector>

class VKTexture;
class VKUniformBuffer;

class VKDescriptor
{
public:
	VKDescriptor(const VKTexture& texture, const VKUniformBuffer& uniformBuffer);
	VKDescriptor(const VKDescriptor& copy);
	~VKDescriptor();

	void Destroy();

	VkDescriptorSetLayout* GetDescriptorSetLayout();
	std::vector<VkDescriptorSet> GetDescriptorSets() const;

private:
	void CreateDescriptorSetLayout();
	void CreateDescriptorPool();
	void CreateDescriptorSets(const VKTexture& texture, const VKUniformBuffer& uniformBuffer);

private:
	VkDescriptorSetLayout vkDescriptorSetLayout;
	VkDescriptorPool vkDescriptorPool;
	std::vector<VkDescriptorSet> vkDescriptorSets;
};

