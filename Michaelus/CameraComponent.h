#pragma once
#include "Component.h"
class CameraComponent :
    public Component
{
public:
	CameraComponent(Actor& rOwner);

    virtual void Update() override;

    void SetFieldOfView(float newFieldOfView);

    float GetFieldOfView() const;

private:
    float fieldOfView = 0.f;
};

