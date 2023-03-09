#pragma once
#include "Pawn.h"
class SceneComponent;
class CameraComponent;

class DemoPawn :
    public Pawn
{
public:
    DemoPawn(const std::string& name);

    virtual void Update() override;

    std::shared_ptr<CameraComponent> GetCamera() const;

private:
    std::shared_ptr<SceneComponent> pSceneComponent;

    std::shared_ptr<CameraComponent> pCameraComponent;
};

