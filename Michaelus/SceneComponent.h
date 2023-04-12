#pragma once
#include "Component.h"

/*
 * Classe SceneComponent
 * Component basique définissant une position relative à l'Actor
 */
class SceneComponent :
    public Component
{
public:
    // Constructeur définissant l'acteur lié au Component
    SceneComponent(Actor& rOwner);

    // Méthode Update() héritée de Component
    virtual void Update() override;
};

