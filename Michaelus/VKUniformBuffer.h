#pragma once
#include "Vulkan.h"

#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

struct UniformBufferObject
{
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
};

class VKUniformBuffer
{
public:
	VKUniformBuffer();
	~VKUniformBuffer();

	void Destroy();

	void Update(UniformBufferObject ubo, uint32_t currentImage);

	std::vector<VkBuffer> GetUniformBuffers() const;
	std::vector<void*> GetUniformBuffersMapped() const;
private:
	void CreateUniformBuffers();

	void InitializeUniformlBuffers();

private:
	std::vector<VkBuffer> vkUniformBuffers;
	std::vector<VkDeviceMemory> vkUniformBuffersMemory;
	std::vector<void*> vkUniformBuffersMapped;
};

