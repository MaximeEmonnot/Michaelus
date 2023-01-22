#pragma once
#include "Pawn.h"
class CameraComponent;

class TestPawn :
    public Pawn
{
public:
    TestPawn(const std::string& name);

    virtual void Update() override;
private:
    std::shared_ptr<CameraComponent> pCameraComponent;
};

