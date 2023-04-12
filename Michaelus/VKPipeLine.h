#pragma once
#include "Vulkan.h"

#include <string>
#include <vector>

// Forward declarations
class VKDescriptor;
class VKSwapChain;

/*
 * Classe VKPipeLine
 * D�finit le pipeline graphique pour le rendu des objets 3D
 * Element indispensable, sans qui l'affichage serait impossible
 */
class VKPipeLine
{
public:
	// Constructeur d�finissant le Vertex Shader, le Fragment Shader, le lien avec le SwapChain et le Descriptor associ� aux Shaders
	VKPipeLine(const std::string& vertexFilePath, const std::string& fragmentFilePath, const VKSwapChain& swapChain, VKDescriptor& descriptor);
	// Destructeur par d�faut
	~VKPipeLine() = default;

	// Diff�rents constructeurs pour satisfaire la RO5 des normes du C++
	// Ces constructeurs sont d�finis comme �tant supprim�s, car ON NE VEUT PAS de copie du Pipeline
	// Cependant, le Pipeline n'est pas unique car on utilise au final un Pipeline par Mod�le
	VKPipeLine(const VKPipeLine&) = delete;
	VKPipeLine& operator= (const VKPipeLine&) = delete;
	VKPipeLine(VKPipeLine&&) = delete;
	VKPipeLine& operator= (VKPipeLine&&) = delete;

	// Destructeur r�el pour contr�ler la lib�ration de m�moire
	void Destroy();

	// Lien du Pipeline avec le CommandBuffer courant
	void Bind(VkCommandBuffer commandBuffer, uint32_t currentFrame, VKDescriptor& descriptor) const;

private:
	// Cr�ation du Pipeline Graphique
	void CreateGraphicsPipeline(const std::string& vertexFilePath, const std::string& fragmentFilePath, const VKSwapChain& swapChain, VKDescriptor& descriptor);

	// Lecture du Shader
	static std::vector<char> ReadShaderFile(const std::string& filename);
	// Cr�ation du Module de Shader, pour le rendu futur
	VkShaderModule CreateShaderModule(const std::vector<char>& code);

private:
	VkPipeline vkGraphicsPipeline;
	VkPipelineLayout vkPipelineLayout;
};

