#pragma once
#include "Vulkan.h"

#include <string>

/*
 * Classe VKTexture
 * D�finition d'une Texture qui sera lue par Vulkan pour l'affichage des mod�les avec texture
 */
class VKTexture
{
public:
	// Constructeur d�finissant la Texture
	VKTexture(const std::string& texturePath);

	// Destructeur r�el pour contr�ler la lib�ration de m�moire
	void Destroy();

	// R�cup�ration de l'Image (Texture)
	VkImage GetTextureImage() const;
	// R�cup�ration de la vue vers l'Image
	VkImageView GetTextureImageView() const;
	// R�cup�ration du Sampler de la Texture
	VkSampler GetTextureSampler() const;

private:
	// Cr�ation de l'Image (Texture)
	void CreateTextureImage(const std::string& texturePath);
	// Cr�ation de la vue vers l'Image
	void CreateTextureImageView();
	// Cr�ation du Sampler de la Texture
	void CreateTextureSampler();

private:
	VkImage vkTextureImage;
	VkDeviceMemory vkTextureImageMemory;
	VkImageView vkTextureImageView;
	VkSampler vkTextureSampler;
	uint32_t vkMipLevels;
};

