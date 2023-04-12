#include "TextureFactory.h"

#include "VKTexture.h"

// VARIABLES STATIQUES
std::unique_ptr<TextureFactory> TextureFactory::pInstance = nullptr;

// ********* //

// Méthode du patron de conception Singleton
TextureFactory& TextureFactory::GetInstance()
{
	if (!pInstance)
		pInstance = std::make_unique<TextureFactory>();
	return *pInstance;
}

// Destructeur réel pour contrôler la libération de mémoire
void TextureFactory::Clear()
{
	for (auto& entry : textures)
		entry.second->Destroy();
	textures.clear();
}

// Création/Récupération de la texture au chemin spécifié
VKTexture& TextureFactory::GetTexture(const std::string& path)
{
	// Si la texture n'a pas été précédemment ouverte, on l'ouvre pour les futures utilisations
	if (!textures.contains(path))
		textures.emplace(path, std::make_shared<VKTexture>(path));
	return *textures.at(path);
}
