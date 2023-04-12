#pragma once
#include <map>
#include <memory>
#include <string>

// Forward declarations
class VKModel;

// Macro pour le nettoyage de la liste des mod�les
#define MODEL_CLEAR ModelFactory::GetInstance().Clear()
// Macro pour la cr�ation d'un mod�le
#define MODEL(path) ModelFactory::GetInstance().GetModel(path)

/*
 * Classe ModelFactory
 * Permet la cr�ation des diff�rents Meshes
 * Conserve une relation Cl�-Valeur entre le chemin du mod�le et le mod�le (Poids mouche)
 */
class ModelFactory
{
public:
	// Constructeur par d�faut
	ModelFactory() = default;
	// Destructeur par d�faut
	~ModelFactory() = default;

	// Ces diff�rents constructeurs sont d�finis comme �tant supprim�s, pour correspondre au patron de conception Singleton
	// et �galement � la RO5 des normes du C++
	ModelFactory(const ModelFactory&) = delete;
	ModelFactory& operator=(const ModelFactory&) = delete;
	ModelFactory(ModelFactory&&) = delete;
	ModelFactory& operator=(ModelFactory&&) = delete;
 
	// M�thode du patron de conception Singleton
	static ModelFactory& GetInstance();

	// Destructeur r�el pour contr�ler la lib�ration de m�moire
	void Clear();

	// Cr�ation/R�cup�ration du mod�le au chemin sp�cifi�
	VKModel& GetModel(const std::string& path);

private:
	static std::unique_ptr<ModelFactory> pInstance;

	std::map<std::string, std::shared_ptr<VKModel>> models;
};

