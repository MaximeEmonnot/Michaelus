#pragma once
#include "Controller.h"
class DemoControllerMouse :
    public Controller
{
public:
    DemoControllerMouse(Pawn& pawn);

    virtual void Update() override;
};

