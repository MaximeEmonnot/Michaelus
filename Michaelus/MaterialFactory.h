#pragma once
#include <map>
#include <memory>
#include <string>

#include "Material.h"

// Macro pour le nettoyage de la liste des matériaux
#define MATERIAL_CLEAR MaterialFactory::GetInstance().Clear()
// Macro pour la création d'un matériau
#define MATERIAL(path, shading_model) MaterialFactory::GetInstance().GetMaterial(path, shading_model)

/*
 * Classe MaterialFactory
 * Permet la création des différents Materiaux
 */
class MaterialFactory
{
public:
	// Constructeur (publique car std::unique_ptr nécessite un constructeur publique) 
	MaterialFactory() = default;
	// Destructeur par défaut
	~MaterialFactory() = default;

	// Ces différents constructeurs sont définis comme étant supprimés, pour correspondre au patron de conception Singleton
	// et également à la RO5 des normes du C++
	MaterialFactory(const MaterialFactory&) = delete;
	MaterialFactory& operator=(const MaterialFactory&) = delete;
	MaterialFactory(MaterialFactory&&) = delete;
	MaterialFactory& operator=(MaterialFactory&&) = delete;

	// Méthode du patron de conception Singleton
	static MaterialFactory& GetInstance();

	// Destructeur réel pour contrôler la libération de mémoire
	void Clear();

	// Création du matériau selon un path et un modèle de rendu
	std::shared_ptr<Material> GetMaterial(const std::string& path, EShadingModel shadingModel);

private:
	static std::unique_ptr<MaterialFactory> pInstance;

	std::vector<std::shared_ptr<Material>> materials;
};

