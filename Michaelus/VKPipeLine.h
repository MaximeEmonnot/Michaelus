#pragma once
#include "Vulkan.h"

#include <string>
#include <vector>

class VKDescriptor;
class VKSwapChain;

class VKPipeLine
{
public:
	VKPipeLine(const std::string& vertexFilePath, const std::string& fragmentFilePath, const VKSwapChain& swapChain, VKDescriptor& descriptor);
	~VKPipeLine();

	VKPipeLine(const VKPipeLine&) = delete;
	VKPipeLine& operator= (const VKPipeLine&) = delete;
	VKPipeLine(VKPipeLine&&) = delete;
	VKPipeLine& operator= (VKPipeLine&&) = delete;

	void Bind(VkCommandBuffer commandBuffer, uint32_t currentFrame, VKDescriptor& descriptor) const;

private:
	void CreateGraphicsPipeline(const std::string& vertexFilePath, const std::string& fragmentFilePath, const VKSwapChain& swapChain, VKDescriptor& descriptor);

	static std::vector<char> ReadShaderFile(const std::string& filename);
	VkShaderModule CreateShaderModule(const std::vector<char>& code);

private:
	VkPipeline vkGraphicsPipeline;
	VkPipelineLayout vkPipelineLayout;
};

