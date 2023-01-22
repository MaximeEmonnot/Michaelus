#pragma once
#include "Actor.h"

class Controller;

class Pawn :
    public Actor
{
public:
    Pawn() = default;
    Pawn(const std::string& name);

    template<class T>
    void SetController();

    virtual void Update() override;

    std::shared_ptr<Controller> GetController() const;

private:
    std::shared_ptr<Controller> pController;
};