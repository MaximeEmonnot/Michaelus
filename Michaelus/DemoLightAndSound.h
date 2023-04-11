#pragma once
#include "Actor.h"
class SceneComponent;

/*
 * Classe DemoLightAndSound
 * Démonstration d'un Actor définissant plusieurs points lumineux en rotation autour d'un point
 */
class DemoLightAndSound :
    public Actor
{
public:
    // Constructeur par défaut
    DemoLightAndSound() = default;
    // Constructeur définissant le nom de l'acteur
    DemoLightAndSound(const std::string& name);

    // Méthode Update() de l'acteur, surchargée
    virtual void Update() override;

private:
    std::shared_ptr<SceneComponent> pRoot;

    std::shared_ptr<SceneComponent> pLightRoot;
    std::shared_ptr<SceneComponent> pLightPoint1;
    std::shared_ptr<SceneComponent> pLightPoint2;
    std::shared_ptr<SceneComponent> pLightPoint3;
    std::shared_ptr<SceneComponent> pLightPoint4;
    std::shared_ptr<SceneComponent> pLightPoint5;
    std::shared_ptr<SceneComponent> pLightPoint6;
    std::shared_ptr<SceneComponent> pLightPoint7;
    std::shared_ptr<SceneComponent> pLightPoint8;

    std::shared_ptr<SceneComponent> pSoundRoot;
    std::shared_ptr<SceneComponent> pSoundSource;

    // LIGHTS
    std::vector<std::shared_ptr<SceneComponent>> pointLightLocations;
    std::vector<size_t> pointLights;

    // Sound
    int soundSourceIndex;
};

