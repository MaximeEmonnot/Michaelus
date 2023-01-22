#include "Graphics.h"

#include <set>
#include <chrono>

#include "VKDevice.h"
#include "VKSwapChain.h"

std::unique_ptr<Graphics> Graphics::pInstance = nullptr;

Graphics::Graphics()
	:
	pSwapChain(std::make_unique<VKSwapChain>())
{
    CreateCommandBuffers();
}

Graphics::~Graphics()
{
}

Graphics& Graphics::GetInstance()
{
    if (!pInstance) pInstance = std::make_unique<Graphics>();
    return *pInstance;
}

void Graphics::Destroy()
{
    pSwapChain->Destroy();
    //for (std::shared_ptr<Mesh> mesh : meshes) mesh->Destroy();
    VK_DEVICE.Destroy();
}

void Graphics::BeginDraw()
{
    pSwapChain->BeginDraw();
}

void Graphics::EndDraw()
{
    pSwapChain->EndDraw(vkCommandBuffers.data(), meshes);
}

VKSwapChain& Graphics::GetSwapChain()
{
    return *pSwapChain;
}

void Graphics::CreateCommandBuffers()
{
    vkCommandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = VK_DEVICE.GetCommandPool();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = static_cast<uint32_t>(vkCommandBuffers.size());

    if (VK_FAILED(vkAllocateCommandBuffers(VK_DEVICE.GetDevice(), &allocInfo, vkCommandBuffers.data())))
        throw GFX_EXCEPTION("Failed to allocate command buffers!");
}