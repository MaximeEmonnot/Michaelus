#pragma once
#include "Vulkan.h"

#include <memory>
#include <string>

#include "Material.h"
#include "Transform.h"

class Material;
class VKModel;

class Mesh
{
public:
	Mesh() = default;
	Mesh(const std::string& meshPath, const Material& pMaterial);


	void SetMaterial(const Material& material);
	Material GetMaterial() const;

	void Update(const FTransform& transform);
	void Draw(VkCommandBuffer commandBuffer, uint32_t currentFrame);

private:
	VKModel& rModel;
	Material material;
};

