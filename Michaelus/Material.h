#pragma once

#include "Vulkan.h"

#include <memory>
#include <string>

#include "Transform.h"

class VKPipeLine;
class VKDescriptor;
class VKUniformBuffer;
class VKTexture;

enum class EShadingModel
{
	Unlit,
	Lit
};

struct SMaterialInformation
{
	struct SMaterialInformationCompare
	{
		bool operator() (const SMaterialInformation& lhs, const SMaterialInformation& rhs) const
		{
			return lhs.path < rhs.path&& lhs.shadingModel < rhs.shadingModel;
		}
	};

	std::string path;
	EShadingModel shadingModel;
};

class Material
{
public:
	Material(const std::string& texturePath, EShadingModel shadingModel);
	Material(const Material& copy);
	Material& operator= (const Material& rhs);
	~Material();

	void Clear();

	VKTexture& GetTexture() const;
	VKUniformBuffer& GetUniformBuffer() const;
	VKDescriptor& GetDescriptor() const;
	VKPipeLine& GetPipeline() const;

	void UpdateUniformBuffer(const FTransform& transform);

	void Bind(VkCommandBuffer commandBuffer, uint32_t currentFrame);

private:
	std::string GetShaderName(EShadingModel shadingModel) const;

private:
	VKTexture& rTexture;
	std::unique_ptr<VKUniformBuffer> pUniformBuffer;
	std::unique_ptr<VKDescriptor> pDescriptor;
	std::unique_ptr<VKPipeLine> pPipeline;
};

