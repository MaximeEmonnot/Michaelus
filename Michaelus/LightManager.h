#pragma once
#include <memory>

#include "Vec3D.h"

#define LIGHT_SYSTEM LightManager::GetInstance()

struct DirectionalLight
{
	FVec3D direction;
	FVec3D color;
	float intensity;
};

struct PointLight
{
	FVec3D position;
	FVec3D color;
	float intensity;
};

class LightManager
{
public:
	LightManager() = default;

	static LightManager& GetInstance();

	void SetDirectionalLight(const FVec3D& direction, const FVec3D& color, float intensity);
	int CreatePointLight(const FVec3D& position, const FVec3D& color, float intensity);
	void SetPointLight(int pointLightIndex, const FVec3D& position, const FVec3D& color, float intensity);

	DirectionalLight GetDirectionalLight() const;
	std::vector<PointLight> GetPointLights() const;

private:
	static std::unique_ptr<LightManager> pInstance;

	DirectionalLight directionalLight;

	std::vector<PointLight> pointLights;
};

