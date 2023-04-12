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

/*
 * Classe Graphics
 * Permet le rendu des différents objets dans la scène
 * Conserve un lien (pointeur) vers les différents Meshes de la scène
 */
class Graphics
{
public:
	// Constructeur (publique car std::unique_ptr nécessite un constructeur publique) 
	Graphics();
	// Destructeur par défaut
	~Graphics() = default;
	// Ces différents constructeurs sont définis comme étant supprimés, pour correspondre au patron de conception Singleton
	// et également à la RO5 des normes du C++
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	Graphics(Graphics&&) = delete;
	Graphics& operator=(Graphics&&) = delete;

	// Méthode du patron de conception Singleton
	static Graphics& GetInstance();

	// Destructeur réel pour contrôler la libération de mémoire
	void Destroy();

	// Initialisation affichage
	void BeginDraw();
	// Exécution affichage
	void EndDraw();

	// Ajout d'un mesh à rendre dans la scène
	void AddMesh(std::weak_ptr<MeshComponent> mesh);

	// Récupération du SwapChain
	VKSwapChain& GetSwapChain();
private:
	// Création des CommandBuffers de Vulkan pour le rendu des différents objets
	void CreateCommandBuffers();

private:
	static std::unique_ptr<Graphics> pInstance;

	std::vector<VkCommandBuffer> vkCommandBuffers;

	std::unique_ptr<VKSwapChain> pSwapChain;

	std::vector<std::weak_ptr<MeshComponent>> meshes;
};

