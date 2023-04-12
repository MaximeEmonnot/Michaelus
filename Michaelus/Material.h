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
 * Enum�rateur EShadingModel
 * D�finit les diff�rents mod�les de rendu
 * 3 mod�les : Unlit (pas de calcul de lumi�re), Lit (Calcul de lumi�re par Phong Shading), Cel (Calcul de lumi�re par Cel Shading)
 */
enum class EShadingModel
{
	Unlit,
	Lit,
	Cel
};

/*
 * Classe Material
 * D�fini par une texture et un mod�le de rendu
 * Ce mat�riau sera appliqu� aux Meshes de la sc�ne, gr�ce au Pipeline graphique de Vulkan (Vertex Shader - Fragment Shader)
 */
class Material
{
public:
	// Constructeur d�finissant la texture et le mod�le de rendu
	Material(const std::string& texturePath, EShadingModel shadingModel);
	// Destructeur par d�faut
	~Material() = default;
	// Diff�rents constructeurs pour satisfaire la RO5 des normes du C++
	// Pour le moment, on ne cherche pas � avoir de comportement particulier pour ces constructeurs
	// Ils sont donc marqu�s default, pour le moment
	Material(const Material&) = default;
	Material& operator=(const Material&) = default;
	Material(Material&&) = default;
	Material& operator=(Material&&) = default;

	// Destructeur r�el pour contr�ler la lib�ration de m�moire
	void Clear();

	// R�cup�ration de la Texture (Vulkan)
	VKTexture& GetTexture() const;
	// R�cup�ration de l'UniformBuffer (Vulkan)
	VKUniformBuffer& GetUniformBuffer() const;
	// R�cup�ration du Descriptor (Vulkan)
	VKDescriptor& GetDescriptor() const;
	// R�cup�ration du Pipeline Graphique (Vulkan)
	VKPipeLine& GetPipeline() const;

	// Mise � jour du UniformBuffer
	void UpdateUniformBuffer(const FTransform& transform);

	// Lien du Material avec le CommandBuffer courant du syst�me de rendu
	void Bind(VkCommandBuffer commandBuffer, uint32_t currentFrame);

private:
	// R�cup�ration des Shaders associ�s au mod�le
	std::string GetShaderName(EShadingModel shadingModel) const;

private:
	VKTexture& rTexture;
	std::unique_ptr<VKUniformBuffer> pUniformBuffer;
	std::unique_ptr<VKDescriptor> pDescriptor;
	std::unique_ptr<VKPipeLine> pPipeline;
};

