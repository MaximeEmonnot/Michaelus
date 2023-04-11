#pragma once
#include <memory>

#include "Vec3D.h"

#define LIGHT_SYSTEM LightManager::GetInstance()

/*
 * Structure DirectionalLight
 * D�finit la direction de la lumi�re, sa couleur et son intensit�
 */
struct DirectionalLight
{
	FVec3D direction;
	FVec3D color;
	float intensity;
};

/*
 * Structure PointLight
 * D�finit la position de la lumi�re, sa couleur et son intensit�
 */
struct PointLight
{
	FVec3D position;
	FVec3D color;
	float intensity;
};

/*
 * Classe LoggerManager
 * Permet la gestion des diff�rentes lumi�res sur la sc�ne
 * D�finit une unique lumi�re directionnelle et une liste de points lumineux
 */
class LightManager
{
public:
	// Constructeur par d�faut (publique car std::unique_ptr n�cessite un constructeur publique) 
	LightManager() = default;
	// Destructeur par d�faut
	~LightManager() = default;

	// Ces diff�rents constructeurs sont d�finis comme �tant supprim�s, pour correspondre au patron de conception Singleton
	// et �galement � la RO5 des normes du C++
	LightManager(const LightManager&) = delete;
	LightManager& operator=(const LightManager&) = delete;
	LightManager(LightManager&&) = delete;
	LightManager& operator=(LightManager&&) = delete;

	// M�thode du patron de conception Singleton
	static LightManager& GetInstance();

	// D�finition de la lumi�re directionnelle
	void SetDirectionalLight(const FVec3D& direction, const FVec3D& color, float intensity);
	// Cr�ation d'un point lumineux
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
	// Modification de la position, de la couleur et de l'intensit� d'un point lumineux existant
	void SetPointLight(int pointLightIndex, const FVec3D& position, const FVec3D& color, float intensity);

	// R�cup�ration de la lumi�re directionnelle
	DirectionalLight GetDirectionalLight() const;
	// R�cup�ration de la liste des points lumineux
	std::vector<PointLight> GetPointLights() const;

private:
	// R�cup�ration de la couleur du point lumineux correspondant � l'index
	FVec3D GetPointLightColor(int index) const;
	// R�cup�ration de l'intensit� du point lumineux correspondant � l'index
	float GetPointLightIntensity(int index) const;

private:
	static std::unique_ptr<LightManager> pInstance;

	DirectionalLight directionalLight;

	std::vector<PointLight> pointLights;
};

