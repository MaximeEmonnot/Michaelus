#include "TextureFactory.h"

#include "VKTexture.h"

// VARIABLES STATIQUES
std::unique_ptr<TextureFactory> TextureFactory::pInstance = nullptr;

// ********* //

// M�thode du patron de conception Singleton
TextureFactory& TextureFactory::GetInstance()
{
	if (!pInstance)
		pInstance = std::make_unique<TextureFactory>();
	return *pInstance;
}

// Destructeur r�el pour contr�ler la lib�ration de m�moire
void TextureFactory::Clear()
{
	for (auto& entry : textures)
		entry.second->Destroy();
	textures.clear();
}

// Cr�ation/R�cup�ration de la texture au chemin sp�cifi�
VKTexture& TextureFactory::GetTexture(const std::string& path)
{
	// Si la texture n'a pas �t� pr�c�demment ouverte, on l'ouvre pour les futures utilisations
	if (!textures.contains(path))
		textures.emplace(path, std::make_shared<VKTexture>(path));
	return *textures.at(path);
}
