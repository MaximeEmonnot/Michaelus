#pragma once

#include "Vulkan.h"

#include <memory>
#include <string>

class VKPipeLine;
class VKDescriptor;
class VKUniformBuffer;
class VKTexture;

enum class EShadingModel
{
	Unlit,
	Lit
};

class Material
{
public:
	Material(const std::string& texturePath, EShadingModel shadingModel);
	~Material();

	VKTexture& GetTexture() const;
	VKUniformBuffer& GetUniformBuffer() const;
	VKDescriptor& GetDescriptor() const;
	VKPipeLine& GetPipeline() const;

	void Bind(VkCommandBuffer commandBuffer, uint32_t currentFrame);

private:
	std::string GetShaderName(EShadingModel shadingModel) const;

private:
	VKTexture& rTexture;
	std::unique_ptr<VKUniformBuffer> pUniformBuffer;
	std::unique_ptr<VKDescriptor> pDescriptor;
	std::unique_ptr<VKPipeLine> pPipeline;
};

