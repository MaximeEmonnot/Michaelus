#pragma once

#include "Vulkan.h"

#include "VKPipeLine.h"
#include "VKSwapChain.h"

#include "VKTexture.h"
#include "VKModel.h"
#include "VKUniformBuffer.h"
#include "VKDescriptor.h"

#define GFX Graphics::GetInstance()

class MeshComponent;

class Graphics
{
public:
	Graphics();
	~Graphics();
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	Graphics(Graphics&&) = delete;
	Graphics& operator=(Graphics&&) = delete;

	static Graphics& GetInstance();

	void Destroy();

	void BeginDraw();
	void EndDraw();

	void AddMesh(std::weak_ptr<MeshComponent> mesh);

	VKSwapChain& GetSwapChain();
private:
	void CreateCommandBuffers();

private:
	static std::unique_ptr<Graphics> pInstance;

	std::vector<VkCommandBuffer> vkCommandBuffers;

	std::unique_ptr<VKSwapChain> pSwapChain;

	std::vector<std::weak_ptr<MeshComponent>> meshes;
};

