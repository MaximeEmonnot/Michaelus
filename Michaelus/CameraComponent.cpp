#include "CameraComponent.h"

CameraComponent::CameraComponent(Actor& rOwner)
	:
	Component(rOwner)
{
}

void CameraComponent::Update()
{
}

void CameraComponent::SetFieldOfView(float newFieldOfView)
{
	fieldOfView = newFieldOfView;
}

float CameraComponent::GetFieldOfView() const
{
	return fieldOfView;
}
