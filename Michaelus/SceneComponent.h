#pragma once
#include "Component.h"
class SceneComponent :
    public Component
{
public:
    SceneComponent(std::weak_ptr<Actor> pOwner);

    virtual void Update() override;
};

