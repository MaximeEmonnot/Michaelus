#pragma once
#include "Component.h"
class Mesh;

class MeshComponent :
    public Component
{
public:
    MeshComponent(Actor& rOwner);

    virtual void Update() override;

    void SetMesh(std::shared_ptr<Mesh> pNewMesh);
    std::shared_ptr<Mesh> GetMesh() const;

private:
    std::shared_ptr<Mesh> pMesh;
};

