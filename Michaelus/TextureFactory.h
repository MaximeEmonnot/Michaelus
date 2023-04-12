#pragma once
#include <map>
#include <memory>
#include <string>

class VKTexture;

#define TEXTURE_CLEAR TextureFactory::GetInstance().Clear()
#define TEXTURE(path) TextureFactory::GetInstance().GetTexture(path)

/*
 * Classe TextureFactory
 * Permet la cr�ation des diff�rentes Textures
 * Conserve une relation Cl�-Valeur entre le chemin de la texture et la texture (Poids mouche)
 */
class TextureFactory
{
public:
	// Constructeur par d�faut
	TextureFactory() = default;
	// Destructeur par d�faut
	~TextureFactory() = default;

	// Ces diff�rents constructeurs sont d�finis comme �tant supprim�s, pour correspondre au patron de conception Singleton
	// et �galement � la RO5 des normes du C++
	TextureFactory(const TextureFactory&) = delete;
	TextureFactory& operator=(const TextureFactory&) = delete;
	TextureFactory(TextureFactory&&) = delete;
	TextureFactory& operator=(TextureFactory&&) = delete;

	// M�thode du patron de conception Singleton
	static TextureFactory& GetInstance();

	// Destructeur r�el pour contr�ler la lib�ration de m�moire
	void Clear();

	// Cr�ation/R�cup�ration de la texture au chemin sp�cifi�
	VKTexture& GetTexture(const std::string& path);

private:
	static std::unique_ptr<TextureFactory> pInstance;

	std::map<std::string, std::shared_ptr<VKTexture>> textures;
};

