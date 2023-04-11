#include "LightManager.h"

// VARIABLES STATIQUES
std::unique_ptr<LightManager> LightManager::pInstance = nullptr;

// ********* //

// M�thode du patron de conception Singleton
LightManager& LightManager::GetInstance()
{
    if (!pInstance)
        pInstance = std::make_unique<LightManager>();
    return *pInstance;
}

// D�finition de la lumi�re directionnelle
void LightManager::SetDirectionalLight(const FVec3D& direction, const FVec3D& color, float intensity)
{
    directionalLight = { direction, color, intensity };
}

// Cr�ation d'un point lumineux
int LightManager::CreatePointLight(const FVec3D& position, const FVec3D& color, float intensity)
{
    pointLights.push_back({ position, color, intensity });

    return pointLights.size() - static_cast<size_t>(1);
}

// Modification de la position, de la couleur et de l'intensit� d'un point lumineux existant
void LightManager::SetPointLight(int pointLightIndex, const FVec3D& position, const FVec3D& color, float intensity)
{
    pointLights.at(pointLightIndex) = { position, color, intensity };
}

// R�cup�ration de la lumi�re directionnelle
DirectionalLight LightManager::GetDirectionalLight() const
{
    return directionalLight;
}

// R�cup�ration de la liste des points lumineux
std::vector<PointLight> LightManager::GetPointLights() const
{
    return pointLights;
}

// R�cup�ration de la couleur du point lumineux correspondant � l'index
FVec3D LightManager::GetPointLightColor(int index) const
{
    return pointLights.at(index).color;
}

// R�cup�ration de l'intensit� du point lumineux correspondant � l'index
float LightManager::GetPointLightIntensity(int index) const
{
    return pointLights.at(index).intensity;
}
