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
	Mesh(const std::string& meshPath, std::vector<std::shared_ptr<Material>> materials);


	void SetMaterial(int index, std::shared_ptr<Material> pNewMaterial);
	std::shared_ptr<Material> GetMaterial(int index) const;

	void Update(const FTransform& transform);
	void Draw(VkCommandBuffer commandBuffer, uint32_t currentFrame);

private:
	VKModel& rModel;

	std::vector<std::shared_ptr<Material>> pMaterials;
};

