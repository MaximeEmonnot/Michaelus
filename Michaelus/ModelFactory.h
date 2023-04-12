#pragma once
#include <map>
#include <memory>
#include <string>

// Forward declarations
class VKModel;

// Macro pour le nettoyage de la liste des modèles
#define MODEL_CLEAR ModelFactory::GetInstance().Clear()
// Macro pour la création d'un modèle
#define MODEL(path) ModelFactory::GetInstance().GetModel(path)

/*
 * Classe ModelFactory
 * Permet la création des différents Meshes
 * Conserve une relation Clé-Valeur entre le chemin du modèle et le modèle (Poids mouche)
 */
class ModelFactory
{
public:
	// Constructeur par défaut
	ModelFactory() = default;
	// Destructeur par défaut
	~ModelFactory() = default;

	// Ces différents constructeurs sont définis comme étant supprimés, pour correspondre au patron de conception Singleton
	// et également à la RO5 des normes du C++
	ModelFactory(const ModelFactory&) = delete;
	ModelFactory& operator=(const ModelFactory&) = delete;
	ModelFactory(ModelFactory&&) = delete;
	ModelFactory& operator=(ModelFactory&&) = delete;
 
	// Méthode du patron de conception Singleton
	static ModelFactory& GetInstance();

	// Destructeur réel pour contrôler la libération de mémoire
	void Clear();

	// Création/Récupération du modèle au chemin spécifié
	VKModel& GetModel(const std::string& path);

private:
	static std::unique_ptr<ModelFactory> pInstance;

	std::map<std::string, std::shared_ptr<VKModel>> models;
};

