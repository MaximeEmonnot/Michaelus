#include "Material.h"

#include "Graphics.h"
#include "TextureFactory.h"
#include "VKDescriptor.h"
#include "VKPipeLine.h"
#include "VKTexture.h"
#include "VKUniformBuffer.h"

// Constructeur d�finissant la texture et le mod�le de rendu
Material::Material(const std::string& texturePath, EShadingModel shadingModel)
	:
	rTexture(TEXTURE(texturePath)),
	pUniformBuffer(std::make_unique<VKUniformBuffer>()),
	pDescriptor(std::make_unique<VKDescriptor>(rTexture, *pUniformBuffer)),
	pPipeline(std::make_unique<VKPipeLine>(GetShaderName(shadingModel) + "Vert.spv", GetShaderName(shadingModel) + "Frag.spv", GFX.GetSwapChain(), *pDescriptor))
{
}

// Destructeur r�el pour contr�ler la lib�ration de m�moire (IMPORTANT : La lib�ration de m�moire doit se faire dans un ordre pr�cis)
void Material::Clear()
{
	pUniformBuffer->Destroy();
	pDescriptor->Destroy();
	pPipeline->Destroy();
}

// R�cup�ration de la Texture (Vulkan)
VKTexture& Material::GetTexture() const
{
	return rTexture;
}

// R�cup�ration de l'UniformBuffer (Vulkan)
VKUniformBuffer& Material::GetUniformBuffer() const
{
	return *pUniformBuffer;
}

// R�cup�ration du Descriptor (Vulkan)
VKDescriptor& Material::GetDescriptor() const
{
	return *pDescriptor;
}

// R�cup�ration du Pipeline Graphique (Vulkan)
VKPipeLine& Material::GetPipeline() const
{
	return *pPipeline;
}

// Mise � jour du UniformBuffer
void Material::UpdateUniformBuffer(const FTransform& transform)
{
	pUniformBuffer->Update(transform);
}

// Lien du Material avec le CommandBuffer courant du syst�me de rendu
void Material::Bind(VkCommandBuffer commandBuffer, uint32_t currentFrame)
{
	pPipeline->Bind(commandBuffer, currentFrame, *pDescriptor);
}

// R�cup�ration des Shaders associ�s au mod�le
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
