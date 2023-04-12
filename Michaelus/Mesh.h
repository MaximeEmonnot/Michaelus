#pragma once
#include "Vulkan.h"

#include <memory>
#include <string>

#include "Transform.h"

class Material;
class VKModel;

/*
 * Classe Mesh
 * D�fini par un Mod�le (Vulkan) et une liste de Material 
 */
class Mesh
{
public:
	// Constructeur par d�faut
	Mesh() = default;
	// Constructeur d�finissant le chemin du mod�le, et les diff�rents mat�riaux composant ce mod�le
	Mesh(const std::string& meshPath, std::vector<std::shared_ptr<Material>> materials);
	// Destructeur par d�faut
	~Mesh() = default;
	// Diff�rents constructeurs pour satisfaire la RO5 des normes du C++
	// Pour le moment, on ne cherche pas � avoir de comportement particulier pour ces constructeurs
	// Ils sont donc marqu�s default, pour le moment
	Mesh(const Mesh&) = default;
	Mesh& operator=(const Mesh&) = default;
	Mesh(Mesh&&) = default;
	Mesh& operator=(Mesh&&) = default;

	// D�finition du mat�riau d'un certain index
	void SetMaterial(int index, std::shared_ptr<Material> pNewMaterial);
	// R�cup�ration d'un mat�riau d'un certain index
	std::shared_ptr<Material> GetMaterial(int index) const;

	// Mise � jour du Mesh
	void Update(const FTransform& transform);
	// Affichage du Mesh par le CommandBuffer courant de Vulkan
	void Draw(VkCommandBuffer commandBuffer, uint32_t currentFrame);

private:
	VKModel& rModel;

	std::vector<std::shared_ptr<Material>> pMaterials;
};

