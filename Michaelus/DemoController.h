#pragma once
#include "Controller.h"
class DemoController :
    public Controller
{
public:
    DemoController(Pawn& pawn);

    virtual void Update() override;
};

