#pragma once
#include "Vulkan.h"

#include <vector>

class VKTexture;
class VKUniformBuffer;

/*
 * Classe VKDescriptor
 * D�finition d'un Descriptor Vulkan pour le bon fonctionnement du Pipeline Graphique
 * Pour faire court, le Descriptor permet d'acc�der aux diff�rents Samplers pour les textures, ainsi que les Uniform Buffer Objects dans les shaders
 * Cette description est importante pour envoyer les diff�rentes informations � la carte graphique, permettant ainsi l'affichage des diff�rents objets
 */
class VKDescriptor
{
public:
	// Constructeur d�finissant la texture � sampler, ainsi que l'UniformBuffer qui lui est associ�
	VKDescriptor(const VKTexture& texture, const VKUniformBuffer& uniformBuffer);
	// Destructeur par d�faut
	~VKDescriptor() = default;
	// Diff�rents constructeurs pour satisfaire la RO5 des normes du C++
	// Pour le moment, on ne cherche pas � avoir de comportement particulier pour ces constructeurs
	// Ils sont donc marqu�s default, pour le moment
	VKDescriptor(const VKDescriptor&) = default;
	VKDescriptor& operator=(const VKDescriptor&) = default;
	VKDescriptor(VKDescriptor&&) = default;
	VKDescriptor& operator=(VKDescriptor&&) = default;

	// Destructeur r�el pour contr�ler la lib�ration de m�moire
	void Destroy();

	// R�cup�ration du Layout du Descriptor
	VkDescriptorSetLayout* GetDescriptorSetLayout();
	// R�cup�ration des Sets de Descriptor
	std::vector<VkDescriptorSet> GetDescriptorSets() const;

private:
	void CreateDescriptorSetLayout();
	void CreateDescriptorPool();
	void CreateDescriptorSets(const VKTexture& texture, const VKUniformBuffer& uniformBuffer);

private:
	VkDescriptorSetLayout vkDescriptorSetLayout;
	VkDescriptorPool vkDescriptorPool;
	std::vector<VkDescriptorSet> vkDescriptorSets;
};

