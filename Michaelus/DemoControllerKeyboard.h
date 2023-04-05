#pragma once
#include "Controller.h"
class DemoControllerKeyboard :
    public Controller
{
public:
    DemoControllerKeyboard(Pawn& pawn);

    virtual void Update() override;
};

