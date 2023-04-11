#pragma once
#include "Actor.h"
class SceneComponent;

/*
 * Classe DemoLightAndSound
 * D�monstration d'un Actor d�finissant plusieurs points lumineux en rotation autour d'un point
 */
class DemoLightAndSound :
    public Actor
{
public:
    // Constructeur par d�faut
    DemoLightAndSound() = default;
    // Constructeur d�finissant le nom de l'acteur
    DemoLightAndSound(const std::string& name);

    // M�thode Update() de l'acteur, surcharg�e
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

