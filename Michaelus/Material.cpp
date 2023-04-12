#include "Material.h"

#include "Graphics.h"
#include "TextureFactory.h"
#include "VKDescriptor.h"
#include "VKPipeLine.h"
#include "VKTexture.h"
#include "VKUniformBuffer.h"

// Constructeur définissant la texture et le modèle de rendu
Material::Material(const std::string& texturePath, EShadingModel shadingModel)
	:
	rTexture(TEXTURE(texturePath)),
	pUniformBuffer(std::make_unique<VKUniformBuffer>()),
	pDescriptor(std::make_unique<VKDescriptor>(rTexture, *pUniformBuffer)),
	pPipeline(std::make_unique<VKPipeLine>(GetShaderName(shadingModel) + "Vert.spv", GetShaderName(shadingModel) + "Frag.spv", GFX.GetSwapChain(), *pDescriptor))
{
}

// Destructeur réel pour contrôler la libération de mémoire (IMPORTANT : La libération de mémoire doit se faire dans un ordre précis)
void Material::Clear()
{
	pUniformBuffer->Destroy();
	pDescriptor->Destroy();
	pPipeline->Destroy();
}

// Récupération de la Texture (Vulkan)
VKTexture& Material::GetTexture() const
{
	return rTexture;
}

// Récupération de l'UniformBuffer (Vulkan)
VKUniformBuffer& Material::GetUniformBuffer() const
{
	return *pUniformBuffer;
}

// Récupération du Descriptor (Vulkan)
VKDescriptor& Material::GetDescriptor() const
{
	return *pDescriptor;
}

// Récupération du Pipeline Graphique (Vulkan)
VKPipeLine& Material::GetPipeline() const
{
	return *pPipeline;
}

// Mise à jour du UniformBuffer
void Material::UpdateUniformBuffer(const FTransform& transform)
{
	pUniformBuffer->Update(transform);
}

// Lien du Material avec le CommandBuffer courant du système de rendu
void Material::Bind(VkCommandBuffer commandBuffer, uint32_t currentFrame)
{
	pPipeline->Bind(commandBuffer, currentFrame, *pDescriptor);
}

// Récupération des Shaders associés au modèle
std::string Material::GetShaderName(EShadingModel shadingModel) const
{
	switch(shadingModel)
	{
	case EShadingModel::Unlit: 
		return "unlit";
	case EShadingModel::Lit: 
		return "lit";
	case EShadingModel::Cel:
		return "cel";
	default: 
		return "";
	}
}
