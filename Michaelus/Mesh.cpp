#include "Mesh.h"

#include "Material.h"
#include "ModelFactory.h"
#include "VKModel.h"

Mesh::Mesh(const std::string& meshPath)
	:
	rModel(MODEL(meshPath)),
	pMaterial(nullptr)
{
}

Mesh::Mesh(const std::string& meshPath, std::shared_ptr<Material> pMaterial)
	:
	rModel(MODEL(meshPath)),
	pMaterial(pMaterial)
{
}

Mesh::~Mesh()
{
	//rModel.Destroy();
	//pMaterial->Destroy();
}

void Mesh::Destroy()
{
}

void Mesh::SetMaterial(std::shared_ptr<Material> pNewMaterial)
{
	pMaterial = pNewMaterial;
}

std::shared_ptr<Material> Mesh::GetMaterial() const
{
	return pMaterial;
}

void Mesh::Update(const FTransform& transform)
{
	pMaterial->UpdateUniformBuffer(transform);
}

void Mesh::Draw(VkCommandBuffer commandBuffer, uint32_t currentFrame)
{
	rModel.Bind(commandBuffer);
	pMaterial->Bind(commandBuffer, currentFrame);
	rModel.Draw(commandBuffer);
}
