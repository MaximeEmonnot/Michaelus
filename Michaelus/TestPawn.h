#pragma once
#include "Pawn.h"
class SceneComponent;
class CameraComponent;

class TestPawn :
    public Pawn
{
public:
    TestPawn(const std::string& name);

    virtual void Update() override;
private:
    std::shared_ptr<SceneComponent> pSceneComponent;

    std::shared_ptr<CameraComponent> pCameraComponent;
    std::shared_ptr<CameraComponent> pCameraComponent2;
    std::shared_ptr<CameraComponent> pCameraComponent3;
    std::shared_ptr<CameraComponent> pCameraComponent4;
    std::shared_ptr<CameraComponent> pCameraComponent5;
};

