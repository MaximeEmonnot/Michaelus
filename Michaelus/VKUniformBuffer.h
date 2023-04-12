#pragma once
#include "Vulkan.h"

#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "Transform.h"

/*
 * Structure DirectionalLightData
 * D�finition des donn�es d'une lumi�re directionnelle pour les Shaders
 */
struct DirectionalLightData
{
	glm::vec4 direction;
	glm::vec4 color;
};

/*
 * Structure PointLightData
 * D�finition des donn�es d'un point lumineux pour les Shaders
 */
struct PointLightData
{
	glm::vec4 position;
	glm::vec4 color;
};

/*
 * Structure UniformBufferObject
 * D�finition des donn�es pour le UniformBuffer utilis� dans les Shaders
 * Permet d'envoyer des informations � la carte graphique
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
 * Permet le lien entre le code ex�cut� par le CPU, et les Shaders ex�cut�s par le GPU
 */
class VKUniformBuffer
{
public:
	// Constructeur
	VKUniformBuffer();

	// Destructeur r�el pour contr�ler la lib�ration de m�moire
	void Destroy();

	// Mise � jour du UniformBuffer pour l'envoi de donn�es vers le GPU
	void Update(const FTransform& modelTransform);

	// R�cup�ration des UniformBuffers
	std::vector<VkBuffer> GetUniformBuffers() const;
	// R�cup�ration des UniformBuffers mapp�s
	std::vector<void*> GetUniformBuffersMapped() const;
private:
	// Cr�ation des UniformBuffers
	void CreateUniformBuffers();

private:
	std::vector<VkBuffer> vkUniformBuffers;
	std::vector<VkDeviceMemory> vkUniformBuffersMemory;
	std::vector<void*> vkUniformBuffersMapped;
};

