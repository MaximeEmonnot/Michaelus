#pragma once
#include "Component.h"
class SceneComponent :
    public Component
{
public:
    SceneComponent(Actor& rOwner);

    virtual void Update() override;
};

