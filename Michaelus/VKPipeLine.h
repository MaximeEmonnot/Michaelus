#pragma once
#include "Vulkan.h"

#include <string>
#include <vector>

// Forward declarations
class VKDescriptor;
class VKSwapChain;

/*
 * Classe VKPipeLine
 * Définit le pipeline graphique pour le rendu des objets 3D
 * Element indispensable, sans qui l'affichage serait impossible
 */
class VKPipeLine
{
public:
	// Constructeur définissant le Vertex Shader, le Fragment Shader, le lien avec le SwapChain et le Descriptor associé aux Shaders
	VKPipeLine(const std::string& vertexFilePath, const std::string& fragmentFilePath, const VKSwapChain& swapChain, VKDescriptor& descriptor);
	// Destructeur par défaut
	~VKPipeLine() = default;

	// Différents constructeurs pour satisfaire la RO5 des normes du C++
	// Ces constructeurs sont définis comme étant supprimés, car ON NE VEUT PAS de copie du Pipeline
	// Cependant, le Pipeline n'est pas unique car on utilise au final un Pipeline par Modèle
	VKPipeLine(const VKPipeLine&) = delete;
	VKPipeLine& operator= (const VKPipeLine&) = delete;
	VKPipeLine(VKPipeLine&&) = delete;
	VKPipeLine& operator= (VKPipeLine&&) = delete;

	// Destructeur réel pour contrôler la libération de mémoire
	void Destroy();

	// Lien du Pipeline avec le CommandBuffer courant
	void Bind(VkCommandBuffer commandBuffer, uint32_t currentFrame, VKDescriptor& descriptor) const;

private:
	// Création du Pipeline Graphique
	void CreateGraphicsPipeline(const std::string& vertexFilePath, const std::string& fragmentFilePath, const VKSwapChain& swapChain, VKDescriptor& descriptor);

	// Lecture du Shader
	static std::vector<char> ReadShaderFile(const std::string& filename);
	// Création du Module de Shader, pour le rendu futur
	VkShaderModule CreateShaderModule(const std::vector<char>& code);

private:
	VkPipeline vkGraphicsPipeline;
	VkPipelineLayout vkPipelineLayout;
};

