#pragma once

#include "Vulkan.h"

#include <memory>
#include <string>

#include "Transform.h"

// Forward declarations
class VKPipeLine;
class VKDescriptor;
class VKUniformBuffer;
class VKTexture;

/*
 * Enumérateur EShadingModel
 * Définit les différents modèles de rendu
 * 3 modèles : Unlit (pas de calcul de lumière), Lit (Calcul de lumière par Phong Shading), Cel (Calcul de lumière par Cel Shading)
 */
enum class EShadingModel
{
	Unlit,
	Lit,
	Cel
};

/*
 * Classe Material
 * Défini par une texture et un modèle de rendu
 * Ce matériau sera appliqué aux Meshes de la scène, grâce au Pipeline graphique de Vulkan (Vertex Shader - Fragment Shader)
 */
class Material
{
public:
	// Constructeur définissant la texture et le modèle de rendu
	Material(const std::string& texturePath, EShadingModel shadingModel);
	// Destructeur par défaut
	~Material() = default;
	// Différents constructeurs pour satisfaire la RO5 des normes du C++
	// Pour le moment, on ne cherche pas à avoir de comportement particulier pour ces constructeurs
	// Ils sont donc marqués default, pour le moment
	Material(const Material&) = default;
	Material& operator=(const Material&) = default;
	Material(Material&&) = default;
	Material& operator=(Material&&) = default;

	// Destructeur réel pour contrôler la libération de mémoire
	void Clear();

	// Récupération de la Texture (Vulkan)
	VKTexture& GetTexture() const;
	// Récupération de l'UniformBuffer (Vulkan)
	VKUniformBuffer& GetUniformBuffer() const;
	// Récupération du Descriptor (Vulkan)
	VKDescriptor& GetDescriptor() const;
	// Récupération du Pipeline Graphique (Vulkan)
	VKPipeLine& GetPipeline() const;

	// Mise à jour du UniformBuffer
	void UpdateUniformBuffer(const FTransform& transform);

	// Lien du Material avec le CommandBuffer courant du système de rendu
	void Bind(VkCommandBuffer commandBuffer, uint32_t currentFrame);

private:
	// Récupération des Shaders associés au modèle
	std::string GetShaderName(EShadingModel shadingModel) const;

private:
	VKTexture& rTexture;
	std::unique_ptr<VKUniformBuffer> pUniformBuffer;
	std::unique_ptr<VKDescriptor> pDescriptor;
	std::unique_ptr<VKPipeLine> pPipeline;
};

