#pragma once
#include "Vulkan.h"

#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "Transform.h"

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

	void Update(const FTransform& modelTransform);

	std::vector<VkBuffer> GetUniformBuffers() const;
	std::vector<void*> GetUniformBuffersMapped() const;
private:
	void CreateUniformBuffers();

private:
	std::vector<VkBuffer> vkUniformBuffers;
	std::vector<VkDeviceMemory> vkUniformBuffersMemory;
	std::vector<void*> vkUniformBuffersMapped;
};

