#pragma once

#include "Vulkan.h"

#include <vector>


#define MAX_FRAMES_IN_FLIGHT 2

class Mesh;
class VKDescriptor;
class VKModel;
class VKPipeLine;

/*
 * Classe SwapChain
 * Element indispensable pour l'affichage régulier des objets
 * Fonctionne selon le principe d'un Double Buffered rendering :
 * Un Front Buffer est affiché à l'écran pendant que l'on prépare la prochaine image dans un Back Buffer
 * Les deux buffers sont inversés à chaque Frame
 */
class VKSwapChain
{
public:
	// Constructeur
	VKSwapChain();
	// Destructeur
	~VKSwapChain() = default;

	// Différents constructeurs pour satisfaire la RO5 des normes du C++
	// Ces constructeurs sont définis comme étant supprimés, car ON NE VEUT SURTOUT PAS de copie du SwapChain
	// La question du Singleton pourrait alors se poser, mais il est possible d'avoir plusieurs SwapChain
	VKSwapChain(const VKSwapChain&) = delete;
	VKSwapChain& operator= (const VKSwapChain&) = delete;
	VKSwapChain(VKSwapChain&&) = delete;
	VKSwapChain& operator= (VKSwapChain&&) = delete;

	// Destructeur réel pour contrôler la libération de mémoire
	void Destroy();

	// Récupération du Buffer de la frame en cours
	VkFramebuffer GetFrameBuffer(int index) const;
	// Récupération de la passe de rendu
	VkRenderPass GetRenderPass() const;
	// Récupération de la vue du Buffer en cours
	VkImageView GetImageView(int index) const;
	// Récupération du nombre d'images
	size_t GetImageCount() const;
	// Récupération du format d'image du SwapChain
	VkFormat GetSwapChainImageFormat() const;
	// Récupération du champ du SwapChain
	VkExtent2D GetSwapChainExtent() const;
	// Récupération de la largeur du champ du SwapChain
	uint32_t GetWidth() const;
	// Récupération de la hauteur du champ du SwapChain
	uint32_t GetHeight() const;

	// Récupération du ratio largeur/hauteur du champ du SwapChain
	float GetExtentAspectRatio() const;

	// Récupération du format de profondeur du SwapChain
	VkFormat FindDepthFormat();

	// Démarrage de la séquence d'affichage
	void BeginDraw();
	// Fin de la séquence d'affichage
	void EndDraw(VkCommandBuffer* commandBuffers, std::vector<std::shared_ptr<Mesh>> meshes);
	// Enregistrement du CommandBuffer courant
	void RecordCommandBuffer(VkCommandBuffer commandBuffer, std::vector<std::shared_ptr<Mesh>> meshes);
	// Envoi des CommandBuffers à Vulkan
	void SubmitCommandBuffers(const VkCommandBuffer* pCommandBuffers);
	// Opérateur d'égalité entre deux SwapChain, en cas de création de nouveau SwapChain
	bool operator== (const VKSwapChain& rhs) const;

private:
	void CreateSwapChain();
	void CreateImageViews();
	void CreateColorResources();
	void CreateDepthResources();
	void CreateRenderPass();
	void CreateFrameBuffers();
	void CreateSynchronizationObjects();

	void CleanUpSwapChain();
	void RecreateSwapChain();

	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

private:
	VkSwapchainKHR vkSwapChain;
	VkFormat vkSwapChainImageFormat;
	VkFormat vkSwapChainDepthFormat;
	VkExtent2D vkSwapChainExtent;

	std::vector<VkImage> vkColorImages;
	std::vector<VkDeviceMemory> vkColorImageMemories;
	std::vector<VkImageView> vkColorImageViews;

	std::vector<VkImage> vkDepthImages;
	std::vector<VkDeviceMemory> vkDepthImageMemories;
	std::vector<VkImageView> vkDepthImageViews;

	std::vector<VkImage> vkSwapChainImages;
	std::vector<VkImageView> vkSwapChainImageViews;

	VkRenderPass vkRenderPass;

	std::vector<VkFramebuffer> vkSwapChainFrameBuffers;

	std::vector<VkSemaphore> vkImageAvailableSemaphores;
	std::vector<VkSemaphore> vkRenderFinishedSemaphores;
	std::vector<VkFence> vkInFlightFences;

	uint32_t currentFrame = 0;
	uint32_t imageIndex = 0;
};

