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
 * Permet le rendu des diff�rents objets dans la sc�ne
 * Conserve un lien (pointeur) vers les diff�rents Meshes de la sc�ne
 */
class Graphics
{
public:
	// Constructeur (publique car std::unique_ptr n�cessite un constructeur publique) 
	Graphics();
	// Destructeur par d�faut
	~Graphics() = default;
	// Ces diff�rents constructeurs sont d�finis comme �tant supprim�s, pour correspondre au patron de conception Singleton
	// et �galement � la RO5 des normes du C++
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	Graphics(Graphics&&) = delete;
	Graphics& operator=(Graphics&&) = delete;

	// M�thode du patron de conception Singleton
	static Graphics& GetInstance();

	// Destructeur r�el pour contr�ler la lib�ration de m�moire
	void Destroy();

	// Initialisation affichage
	void BeginDraw();
	// Ex�cution affichage
	void EndDraw();

	// Ajout d'un mesh � rendre dans la sc�ne
	void AddMesh(std::weak_ptr<MeshComponent> mesh);

	// R�cup�ration du SwapChain
	VKSwapChain& GetSwapChain();
private:
	// Cr�ation des CommandBuffers de Vulkan pour le rendu des diff�rents objets
	void CreateCommandBuffers();

private:
	static std::unique_ptr<Graphics> pInstance;

	std::vector<VkCommandBuffer> vkCommandBuffers;

	std::unique_ptr<VKSwapChain> pSwapChain;

	std::vector<std::weak_ptr<MeshComponent>> meshes;
};

