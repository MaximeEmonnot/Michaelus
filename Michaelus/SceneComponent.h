#pragma once
#include "Component.h"

/*
 * Classe SceneComponent
 * Component basique d�finissant une position relative � l'Actor
 */
class SceneComponent :
    public Component
{
public:
    // Constructeur d�finissant l'acteur li� au Component
    SceneComponent(Actor& rOwner);

    // M�thode Update() h�rit�e de Component
    virtual void Update() override;
};

