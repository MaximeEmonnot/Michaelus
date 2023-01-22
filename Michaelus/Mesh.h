#pragma once
#include "Vulkan.h"

#include <memory>
#include <string>

class Material;
class VKModel;

class Mesh
{
public:
	Mesh() = default;
	Mesh(const std::string& meshPath);
	Mesh(const std::string& meshPath, std::shared_ptr<Material> pMaterial);

	~Mesh();

	void SetMaterial(std::shared_ptr<Material> pNewMaterial);
	std::shared_ptr<Material> GetMaterial() const;

	void Draw(VkCommandBuffer commandBuffer, uint32_t currentFrame);

private:
	VKModel& rModel;
	std::shared_ptr<Material> pMaterial;
};

