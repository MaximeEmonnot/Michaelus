#pragma once
#include "Actor.h"
class DemoPlane :
    public Actor
{
public:
    DemoPlane() = default;
    DemoPlane(const std::string& name);

    virtual void Update() override;

private:
    std::shared_ptr<MeshComponent> pMeshComponent;
};

