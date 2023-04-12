#include "Mesh.h"

#include "Material.h"
#include "ModelFactory.h"
#include "VKModel.h"

// Constructeur définissant le chemin du modèle, et les différents matériaux composant ce modèle
Mesh::Mesh(const std::string& meshPath, std::vector<std::shared_ptr<Material>> materials)
	:
	rModel(MODEL(meshPath)),
	pMaterials(materials)
{
}

// Définition du matériau d'un certain index
void Mesh::SetMaterial(int index, std::shared_ptr<Material> pNewMaterial)
{
	pMaterials.at(index) = pNewMaterial;
}

// Récupération d'un matériau d'un certain index
std::shared_ptr<Material> Mesh::GetMaterial(int index) const
{
	return pMaterials.at(index);
}

// Mise à jour du Mesh : On met à jour les différents matériaux composant le Mesh
void Mesh::Update(const FTransform& transform)
{
	for (auto& material : pMaterials) material->UpdateUniformBuffer(transform);
}

// Affichage du Mesh par le CommandBuffer courant de Vulkan
void Mesh::Draw(VkCommandBuffer commandBuffer, uint32_t currentFrame)
{
	for (size_t i = 0; i < pMaterials.size(); i++)
	{
		rModel.Bind(commandBuffer, i);
		pMaterials.at(i)->Bind(commandBuffer, currentFrame);
		rModel.Draw(commandBuffer, i);
	}
}
