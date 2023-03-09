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
    void SetController()
    {
        if (!std::is_base_of<Controller, T>())
            throw ENGINE_EXCEPTION("Vulkan 3D Engine - Main Engine Exception", "This is not a Controller Class. Please check your call for SetController.");

        pController = std::make_shared<T>(*this);
    }

    virtual void Update() override;

    std::shared_ptr<Controller> GetController() const;

private:
    std::shared_ptr<Controller> pController;
};