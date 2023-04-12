#pragma once
#include "Vulkan.h"

#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "Transform.h"

/*
 * Structure DirectionalLightData
 * Définition des données d'une lumière directionnelle pour les Shaders
 */
struct DirectionalLightData
{
	glm::vec4 direction;
	glm::vec4 color;
};

/*
 * Structure PointLightData
 * Définition des données d'un point lumineux pour les Shaders
 */
struct PointLightData
{
	glm::vec4 position;
	glm::vec4 color;
};

/*
 * Structure UniformBufferObject
 * Définition des données pour le UniformBuffer utilisé dans les Shaders
 * Permet d'envoyer des informations à la carte graphique
 */
struct UniformBufferObject
{
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	glm::vec4 viewPosition;
	glm::vec4 lightIntensities;
	DirectionalLightData directional;
	PointLightData pointLights[32];
	int numLights;
};

/*
 * Classe VKUniformBuffer
 * Permet le lien entre le code exécuté par le CPU, et les Shaders exécutés par le GPU
 */
class VKUniformBuffer
{
public:
	// Constructeur
	VKUniformBuffer();

	// Destructeur réel pour contrôler la libération de mémoire
	void Destroy();

	// Mise à jour du UniformBuffer pour l'envoi de données vers le GPU
	void Update(const FTransform& modelTransform);

	// Récupération des UniformBuffers
	std::vector<VkBuffer> GetUniformBuffers() const;
	// Récupération des UniformBuffers mappés
	std::vector<void*> GetUniformBuffersMapped() const;
private:
	// Création des UniformBuffers
	void CreateUniformBuffers();

private:
	std::vector<VkBuffer> vkUniformBuffers;
	std::vector<VkDeviceMemory> vkUniformBuffersMemory;
	std::vector<void*> vkUniformBuffersMapped;
};

