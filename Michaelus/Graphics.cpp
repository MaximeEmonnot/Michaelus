#include "Graphics.h"

#include <set>
#include <chrono>

#include "VKDevice.h"
#include "VKSwapChain.h"

std::unique_ptr<Graphics> Graphics::pInstance = nullptr;

Graphics::Graphics()
	:
	textureTEST("Textures/viking_room.png"),
	modelTEST("Meshes/viking_room.obj"),
	descriptorTEST(textureTEST, uniformBufferTEST),
	pipeline("vert.spv","frag.spv", swapChain, descriptorTEST)
{
    CreateCommandBuffers();
}

Graphics& Graphics::GetInstance()
{
    if (!pInstance) pInstance = std::make_unique<Graphics>();
    return *pInstance;
}

void Graphics::BeginDraw()
{
    swapChain.BeginDraw();
}

void Graphics::EndDraw()
{
    swapChain.EndDraw(vkCommandBuffers.data(), pipeline, modelTEST, descriptorTEST);
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