#include "ModelFactory.h"

#include "VKModel.h"

// VARIABLES STATIQUES
std::unique_ptr<ModelFactory> ModelFactory::pInstance = nullptr;

// ********* //

// M�thode du patron de conception Singleton
ModelFactory& ModelFactory::GetInstance()
{
	if (!pInstance)
		pInstance = std::make_unique<ModelFactory>();
	return *pInstance;
}

// Destructeur r�el pour contr�ler la lib�ration de m�moire
void ModelFactory::Clear()
{
	for (auto& entry : models)
		entry.second->Destroy();
	models.clear();
}

// Cr�ation/R�cup�ration du mod�le au chemin sp�cifi�
VKModel& ModelFactory::GetModel(const std::string& path)
{
	// Si le mod�le n'a pas �t� pr�c�demment ouvert, on l'ouvre pour les futures utilisations
	if (!models.contains(path))
		models.emplace(path, std::make_shared<VKModel>(path));
	return *models.at(path);
}
