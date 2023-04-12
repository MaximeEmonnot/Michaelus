#pragma once
#include "Vulkan.h"

#include <string>

/*
 * Classe VKTexture
 * Définition d'une Texture qui sera lue par Vulkan pour l'affichage des modèles avec texture
 */
class VKTexture
{
public:
	// Constructeur définissant la Texture
	VKTexture(const std::string& texturePath);

	// Destructeur réel pour contrôler la libération de mémoire
	void Destroy();

	// Récupération de l'Image (Texture)
	VkImage GetTextureImage() const;
	// Récupération de la vue vers l'Image
	VkImageView GetTextureImageView() const;
	// Récupération du Sampler de la Texture
	VkSampler GetTextureSampler() const;

private:
	// Création de l'Image (Texture)
	void CreateTextureImage(const std::string& texturePath);
	// Création de la vue vers l'Image
	void CreateTextureImageView();
	// Création du Sampler de la Texture
	void CreateTextureSampler();

private:
	VkImage vkTextureImage;
	VkDeviceMemory vkTextureImageMemory;
	VkImageView vkTextureImageView;
	VkSampler vkTextureSampler;
	uint32_t vkMipLevels;
};

