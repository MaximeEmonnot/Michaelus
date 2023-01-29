#include "Material.h"

#include "Graphics.h"
#include "TextureFactory.h"
#include "VKDescriptor.h"
#include "VKPipeLine.h"
#include "VKTexture.h"
#include "VKUniformBuffer.h"

Material::Material(const std::string& texturePath, EShadingModel shadingModel)
	:
	rTexture(TEXTURE(texturePath)),
	pUniformBuffer(std::make_unique<VKUniformBuffer>()),
	pDescriptor(std::make_unique<VKDescriptor>(rTexture, *pUniformBuffer)),
	pPipeline(std::make_unique<VKPipeLine>(GetShaderName(shadingModel) + "Vert.spv", GetShaderName(shadingModel) + "Frag.spv", GFX.GetSwapChain(), *pDescriptor))
{
}

Material::Material(const Material& copy)
	:
	rTexture(copy.rTexture)
{
	*this = copy;
}

Material& Material::operator=(const Material& rhs)
{
	rTexture = rhs.rTexture;
	pUniformBuffer = std::make_unique<VKUniformBuffer>(*rhs.pUniformBuffer);
	pDescriptor = std::make_unique<VKDescriptor>(*rhs.pDescriptor);
	pPipeline = std::make_unique<VKPipeLine>(*rhs.pPipeline);

	return *this;
}

Material::~Material()
{

}

void Material::Clear()
{
	pUniformBuffer->Destroy();
	pDescriptor->Destroy();
	pPipeline->Destroy();
}

VKTexture& Material::GetTexture() const
{
	return rTexture;
}

VKUniformBuffer& Material::GetUniformBuffer() const
{
	return *pUniformBuffer;
}

VKDescriptor& Material::GetDescriptor() const
{
	return *pDescriptor;
}

VKPipeLine& Material::GetPipeline() const
{
	return *pPipeline;
}

void Material::UpdateUniformBuffer(const FTransform& transform)
{
	pUniformBuffer->Update(transform);
}

void Material::Bind(VkCommandBuffer commandBuffer, uint32_t currentFrame)
{
	pPipeline->Bind(commandBuffer, currentFrame, *pDescriptor);
}

std::string Material::GetShaderName(EShadingModel shadingModel) const
{
	switch(shadingModel)
	{
	case EShadingModel::Unlit: 
		return "unlit";
	case EShadingModel::Lit: 
		return "lit";
	default: 
		return "";
	}
}
