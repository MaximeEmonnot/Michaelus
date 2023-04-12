#include "Mesh.h"

#include "Material.h"
#include "ModelFactory.h"
#include "VKModel.h"

// Constructeur d�finissant le chemin du mod�le, et les diff�rents mat�riaux composant ce mod�le
Mesh::Mesh(const std::string& meshPath, std::vector<std::shared_ptr<Material>> materials)
	:
	rModel(MODEL(meshPath)),
	pMaterials(materials)
{
}

// D�finition du mat�riau d'un certain index
void Mesh::SetMaterial(int index, std::shared_ptr<Material> pNewMaterial)
{
	pMaterials.at(index) = pNewMaterial;
}

// R�cup�ration d'un mat�riau d'un certain index
std::shared_ptr<Material> Mesh::GetMaterial(int index) const
{
	return pMaterials.at(index);
}

// Mise � jour du Mesh : On met � jour les diff�rents mat�riaux composant le Mesh
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
