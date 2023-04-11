#include "LightManager.h"

// VARIABLES STATIQUES
std::unique_ptr<LightManager> LightManager::pInstance = nullptr;

// ********* //

// Méthode du patron de conception Singleton
LightManager& LightManager::GetInstance()
{
    if (!pInstance)
        pInstance = std::make_unique<LightManager>();
    return *pInstance;
}

// Définition de la lumière directionnelle
void LightManager::SetDirectionalLight(const FVec3D& direction, const FVec3D& color, float intensity)
{
    directionalLight = { direction, color, intensity };
}

// Création d'un point lumineux
int LightManager::CreatePointLight(const FVec3D& position, const FVec3D& color, float intensity)
{
    pointLights.push_back({ position, color, intensity });

    return pointLights.size() - static_cast<size_t>(1);
}

// Modification de la position, de la couleur et de l'intensité d'un point lumineux existant
void LightManager::SetPointLight(int pointLightIndex, const FVec3D& position, const FVec3D& color, float intensity)
{
    pointLights.at(pointLightIndex) = { position, color, intensity };
}

// Récupération de la lumière directionnelle
DirectionalLight LightManager::GetDirectionalLight() const
{
    return directionalLight;
}

// Récupération de la liste des points lumineux
std::vector<PointLight> LightManager::GetPointLights() const
{
    return pointLights;
}

// Récupération de la couleur du point lumineux correspondant à l'index
FVec3D LightManager::GetPointLightColor(int index) const
{
    return pointLights.at(index).color;
}

// Récupération de l'intensité du point lumineux correspondant à l'index
float LightManager::GetPointLightIntensity(int index) const
{
    return pointLights.at(index).intensity;
}
