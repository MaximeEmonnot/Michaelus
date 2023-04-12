#pragma once
#include "Vulkan.h"

#include <vector>

class VKTexture;
class VKUniformBuffer;

/*
 * Classe VKDescriptor
 * Définition d'un Descriptor Vulkan pour le bon fonctionnement du Pipeline Graphique
 * Pour faire court, le Descriptor permet d'accéder aux différents Samplers pour les textures, ainsi que les Uniform Buffer Objects dans les shaders
 * Cette description est importante pour envoyer les différentes informations à la carte graphique, permettant ainsi l'affichage des différents objets
 */
class VKDescriptor
{
public:
	// Constructeur définissant la texture à sampler, ainsi que l'UniformBuffer qui lui est associé
	VKDescriptor(const VKTexture& texture, const VKUniformBuffer& uniformBuffer);
	// Destructeur par défaut
	~VKDescriptor() = default;
	// Différents constructeurs pour satisfaire la RO5 des normes du C++
	// Pour le moment, on ne cherche pas à avoir de comportement particulier pour ces constructeurs
	// Ils sont donc marqués default, pour le moment
	VKDescriptor(const VKDescriptor&) = default;
	VKDescriptor& operator=(const VKDescriptor&) = default;
	VKDescriptor(VKDescriptor&&) = default;
	VKDescriptor& operator=(VKDescriptor&&) = default;

	// Destructeur réel pour contrôler la libération de mémoire
	void Destroy();

	// Récupération du Layout du Descriptor
	VkDescriptorSetLayout* GetDescriptorSetLayout();
	// Récupération des Sets de Descriptor
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

