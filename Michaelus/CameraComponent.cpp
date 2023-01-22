#include "CameraComponent.h"

CameraComponent::CameraComponent(std::weak_ptr<Actor> pOwner)
	:
	Component(pOwner),
	fieldOfView(90.f)
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
