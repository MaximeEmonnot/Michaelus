#pragma once
#include <map>
#include <memory>
#include <string>

#include "Material.h"

// Macro pour le nettoyage de la liste des mat�riaux
#define MATERIAL_CLEAR MaterialFactory::GetInstance().Clear()
// Macro pour la cr�ation d'un mat�riau
#define MATERIAL(path, shading_model) MaterialFactory::GetInstance().GetMaterial(path, shading_model)

/*
 * Classe MaterialFactory
 * Permet la cr�ation des diff�rents Materiaux
 */
class MaterialFactory
{
public:
	// Constructeur (publique car std::unique_ptr n�cessite un constructeur publique) 
	MaterialFactory() = default;
	// Destructeur par d�faut
	~MaterialFactory() = default;

	// Ces diff�rents constructeurs sont d�finis comme �tant supprim�s, pour correspondre au patron de conception Singleton
	// et �galement � la RO5 des normes du C++
	MaterialFactory(const MaterialFactory&) = delete;
	MaterialFactory& operator=(const MaterialFactory&) = delete;
	MaterialFactory(MaterialFactory&&) = delete;
	MaterialFactory& operator=(MaterialFactory&&) = delete;

	// M�thode du patron de conception Singleton
	static MaterialFactory& GetInstance();

	// Destructeur r�el pour contr�ler la lib�ration de m�moire
	void Clear();

	// Cr�ation du mat�riau selon un path et un mod�le de rendu
	std::shared_ptr<Material> GetMaterial(const std::string& path, EShadingModel shadingModel);

private:
	static std::unique_ptr<MaterialFactory> pInstance;

	std::vector<std::shared_ptr<Material>> materials;
};

