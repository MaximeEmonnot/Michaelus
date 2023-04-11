#pragma once
#include <memory>

#include "Vec3D.h"

#define LIGHT_SYSTEM LightManager::GetInstance()

/*
 * Structure DirectionalLight
 * Définit la direction de la lumière, sa couleur et son intensité
 */
struct DirectionalLight
{
	FVec3D direction;
	FVec3D color;
	float intensity;
};

/*
 * Structure PointLight
 * Définit la position de la lumière, sa couleur et son intensité
 */
struct PointLight
{
	FVec3D position;
	FVec3D color;
	float intensity;
};

/*
 * Classe LoggerManager
 * Permet la gestion des différentes lumières sur la scène
 * Définit une unique lumière directionnelle et une liste de points lumineux
 */
class LightManager
{
public:
	// Constructeur par défaut (publique car std::unique_ptr nécessite un constructeur publique) 
	LightManager() = default;
	// Destructeur par défaut
	~LightManager() = default;

	// Ces différents constructeurs sont définis comme étant supprimés, pour correspondre au patron de conception Singleton
	// et également à la RO5 des normes du C++
	LightManager(const LightManager&) = delete;
	LightManager& operator=(const LightManager&) = delete;
	LightManager(LightManager&&) = delete;
	LightManager& operator=(LightManager&&) = delete;

	// Méthode du patron de conception Singleton
	static LightManager& GetInstance();

	// Définition de la lumière directionnelle
	void SetDirectionalLight(const FVec3D& direction, const FVec3D& color, float intensity);
	// Création d'un point lumineux
	int CreatePointLight(const FVec3D& position, const FVec3D& color, float intensity);
	// Modification de la position d'un point lumineux existant
	void SetPointLight(int pointLightIndex, const FVec3D& position)
	{
		SetPointLight(pointLightIndex, position, GetPointLightColor(pointLightIndex));
	}
	// Modification de la position et de la couleur d'un point lumineux existant
	void SetPointLight(int pointLightIndex, const FVec3D& position, const FVec3D& color)
	{
		SetPointLight(pointLightIndex, position, color, GetPointLightIntensity(pointLightIndex));
	}
	// Modification de la position, de la couleur et de l'intensité d'un point lumineux existant
	void SetPointLight(int pointLightIndex, const FVec3D& position, const FVec3D& color, float intensity);

	// Récupération de la lumière directionnelle
	DirectionalLight GetDirectionalLight() const;
	// Récupération de la liste des points lumineux
	std::vector<PointLight> GetPointLights() const;

private:
	// Récupération de la couleur du point lumineux correspondant à l'index
	FVec3D GetPointLightColor(int index) const;
	// Récupération de l'intensité du point lumineux correspondant à l'index
	float GetPointLightIntensity(int index) const;

private:
	static std::unique_ptr<LightManager> pInstance;

	DirectionalLight directionalLight;

	std::vector<PointLight> pointLights;
};

