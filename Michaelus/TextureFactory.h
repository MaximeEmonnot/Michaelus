#pragma once
#include <map>
#include <memory>
#include <string>

class VKTexture;

#define TEXTURE_CLEAR TextureFactory::GetInstance().Clear()
#define TEXTURE(path) TextureFactory::GetInstance().GetTexture(path)

/*
 * Classe TextureFactory
 * Permet la création des différentes Textures
 * Conserve une relation Clé-Valeur entre le chemin de la texture et la texture (Poids mouche)
 */
class TextureFactory
{
public:
	// Constructeur par défaut
	TextureFactory() = default;
	// Destructeur par défaut
	~TextureFactory() = default;

	// Ces différents constructeurs sont définis comme étant supprimés, pour correspondre au patron de conception Singleton
	// et également à la RO5 des normes du C++
	TextureFactory(const TextureFactory&) = delete;
	TextureFactory& operator=(const TextureFactory&) = delete;
	TextureFactory(TextureFactory&&) = delete;
	TextureFactory& operator=(TextureFactory&&) = delete;

	// Méthode du patron de conception Singleton
	static TextureFactory& GetInstance();

	// Destructeur réel pour contrôler la libération de mémoire
	void Clear();

	// Création/Récupération de la texture au chemin spécifié
	VKTexture& GetTexture(const std::string& path);

private:
	static std::unique_ptr<TextureFactory> pInstance;

	std::map<std::string, std::shared_ptr<VKTexture>> textures;
};

