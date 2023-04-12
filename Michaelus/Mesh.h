#pragma once
#include "Vulkan.h"

#include <memory>
#include <string>

#include "Transform.h"

class Material;
class VKModel;

/*
 * Classe Mesh
 * Défini par un Modèle (Vulkan) et une liste de Material 
 */
class Mesh
{
public:
	// Constructeur par défaut
	Mesh() = default;
	// Constructeur définissant le chemin du modèle, et les différents matériaux composant ce modèle
	Mesh(const std::string& meshPath, std::vector<std::shared_ptr<Material>> materials);
	// Destructeur par défaut
	~Mesh() = default;
	// Différents constructeurs pour satisfaire la RO5 des normes du C++
	// Pour le moment, on ne cherche pas à avoir de comportement particulier pour ces constructeurs
	// Ils sont donc marqués default, pour le moment
	Mesh(const Mesh&) = default;
	Mesh& operator=(const Mesh&) = default;
	Mesh(Mesh&&) = default;
	Mesh& operator=(Mesh&&) = default;

	// Définition du matériau d'un certain index
	void SetMaterial(int index, std::shared_ptr<Material> pNewMaterial);
	// Récupération d'un matériau d'un certain index
	std::shared_ptr<Material> GetMaterial(int index) const;

	// Mise à jour du Mesh
	void Update(const FTransform& transform);
	// Affichage du Mesh par le CommandBuffer courant de Vulkan
	void Draw(VkCommandBuffer commandBuffer, uint32_t currentFrame);

private:
	VKModel& rModel;

	std::vector<std::shared_ptr<Material>> pMaterials;
};

