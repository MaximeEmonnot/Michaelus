#include "Mesh.h"

#include "Material.h"
#include "ModelFactory.h"
#include "VKModel.h"


Mesh::Mesh(const std::string& meshPath, std::vector<std::shared_ptr<Material>> materials)
	:
	rModel(MODEL(meshPath)),
	pMaterials(materials)
{
}

void Mesh::SetMaterial(int index, std::shared_ptr<Material> pNewMaterial)
{
	pMaterials.at(index) = pNewMaterial;
}

std::shared_ptr<Material> Mesh::GetMaterial(int index) const
{
	return pMaterials.at(index);
}

void Mesh::Update(const FTransform& transform)
{
	for (auto& material : pMaterials) material->UpdateUniformBuffer(transform);
}

void Mesh::Draw(VkCommandBuffer commandBuffer, uint32_t currentFrame)
{
	rModel.Bind(commandBuffer);
	for (auto& material : pMaterials) material->Bind(commandBuffer, currentFrame);
	rModel.Draw(commandBuffer);
}
