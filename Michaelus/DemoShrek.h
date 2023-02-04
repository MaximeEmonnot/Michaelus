#pragma once
#include "Actor.h"
class DemoShrek :
    public Actor
{
public:
    DemoShrek() = default;
    DemoShrek(const std::string & name);

    virtual void Update() override;

private:
    std::shared_ptr<MeshComponent> pMeshComponent;
};

