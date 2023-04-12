#include "ModelFactory.h"

#include "VKModel.h"

// VARIABLES STATIQUES
std::unique_ptr<ModelFactory> ModelFactory::pInstance = nullptr;

// ********* //

// Méthode du patron de conception Singleton
ModelFactory& ModelFactory::GetInstance()
{
	if (!pInstance)
		pInstance = std::make_unique<ModelFactory>();
	return *pInstance;
}

// Destructeur réel pour contrôler la libération de mémoire
void ModelFactory::Clear()
{
	for (auto& entry : models)
		entry.second->Destroy();
	models.clear();
}

// Création/Récupération du modèle au chemin spécifié
VKModel& ModelFactory::GetModel(const std::string& path)
{
	// Si le modèle n'a pas été précédemment ouvert, on l'ouvre pour les futures utilisations
	if (!models.contains(path))
		models.emplace(path, std::make_shared<VKModel>(path));
	return *models.at(path);
}
