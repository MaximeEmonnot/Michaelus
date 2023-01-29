#include "Mesh.h"

#include "Material.h"
#include "ModelFactory.h"
#include "VKModel.h"


Mesh::Mesh(const std::string& meshPath, const Material& material)
	:
	rModel(MODEL(meshPath)),
	material(material)
{
}

void Mesh::SetMaterial(const Material& newMaterial)
{
	material = newMaterial;
}

Material Mesh::GetMaterial() const
{
	return material;
}

void Mesh::Update(const FTransform& transform)
{
	material.UpdateUniformBuffer(transform);
}

void Mesh::Draw(VkCommandBuffer commandBuffer, uint32_t currentFrame)
{
	rModel.Bind(commandBuffer);
	material.Bind(commandBuffer, currentFrame);
	rModel.Draw(commandBuffer);
}
