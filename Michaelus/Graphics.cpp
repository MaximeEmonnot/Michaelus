#include "Graphics.h"

#include <set>
#include <chrono>

#include "MaterialFactory.h"
#include "VKDevice.h"
#include "VKSwapChain.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "ModelFactory.h"
#include "TextureFactory.h"

// VARIABLES STATIQUES
std::unique_ptr<Graphics> Graphics::pInstance = nullptr;

// ********* //

// Constructeur
Graphics::Graphics()
	:
	pSwapChain(std::make_unique<VKSwapChain>())
{
    CreateCommandBuffers();
}

// Méthode du patron de conception Singleton
Graphics& Graphics::GetInstance()
{
    if (!pInstance) pInstance = std::make_unique<Graphics>();
    return *pInstance;
}

// Destructeur réel pour contrôler la libération de mémoire
void Graphics::Destroy()
{
    pSwapChain->Destroy();
    meshes.clear();
    TEXTURE_CLEAR;
    MATERIAL_CLEAR;
    MODEL_CLEAR;
    VK_DEVICE.Destroy();
}

// Initialisation affichage
void Graphics::BeginDraw()
{
    pSwapChain->BeginDraw();
}

// Exécution affichage
void Graphics::EndDraw()
{
    std::vector<std::shared_ptr<Mesh>> meshList;
    for (auto mesh : meshes)
    	if (auto sharedMesh = mesh.lock())meshList.emplace_back(sharedMesh->GetMesh());

    pSwapChain->EndDraw(vkCommandBuffers.data(), meshList);
}

// Ajout d'un mesh à rendre dans la scène
void Graphics::AddMesh(std::weak_ptr<MeshComponent> mesh)
{
    meshes.emplace_back(mesh);
}

// Récupération du SwapChain
VKSwapChain& Graphics::GetSwapChain()
{
    return *pSwapChain;
}

// Création des CommandBuffers de Vulkan pour le rendu des différents objets
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