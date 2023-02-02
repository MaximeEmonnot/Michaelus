#include "LightManager.h"

std::unique_ptr<LightManager> LightManager::pInstance = nullptr;

LightManager& LightManager::GetInstance()
{
    if (!pInstance)
        pInstance = std::make_unique<LightManager>();
    return *pInstance;
}

void LightManager::SetDirectionalLight(const FVec3D& direction, const FVec3D& color, float intensity)
{
    directionalLight = { direction, color, intensity };
}

int LightManager::CreatePointLight(const FVec3D& position, const FVec3D& color, float intensity)
{
    pointLights.push_back({ position, color, intensity });

    return pointLights.size() - static_cast<size_t>(1);
}

void LightManager::SetPointLight(int pointLightIndex, const FVec3D& position, const FVec3D& color, float intensity)
{
    pointLights.at(pointLightIndex) = { position, color, intensity };
}

DirectionalLight LightManager::GetDirectionalLight() const
{
    return directionalLight;
}

std::vector<PointLight> LightManager::GetPointLights() const
{
    return pointLights;
}
