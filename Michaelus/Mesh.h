#pragma once
#include "Vulkan.h"

#include <memory>
#include <string>

#include "Transform.h"

class Material;
class VKModel;

class Mesh
{
public:
	Mesh() = default;
	Mesh(const std::string& meshPath);
	Mesh(const std::string& meshPath, std::shared_ptr<Material> pMaterial);

	~Mesh();
	void Destroy();

	void SetMaterial(std::shared_ptr<Material> pNewMaterial);
	std::shared_ptr<Material> GetMaterial() const;

	void Update(const FTransform& transform);
	void Draw(VkCommandBuffer commandBuffer, uint32_t currentFrame);

private:
	VKModel& rModel;
	std::shared_ptr<Material> pMaterial;
};

