#include "CameraComponent.h"

#include "SoundSystem.h"

CameraComponent::CameraComponent(Actor& rOwner)
	:
	Component(rOwner)
{
}

void CameraComponent::Update()
{
	SFX.SetListenerLocation(GetWorldLocation());
}

void CameraComponent::SetSensitivity(float newSensitivity)
{
	sensitivity = newSensitivity;
}

float CameraComponent::GetSensitivity() const
{
	return sensitivity;
}

void CameraComponent::SetFieldOfView(float newFieldOfView)
{
	fieldOfView = newFieldOfView;
}

float CameraComponent::GetFieldOfView() const
{
	return fieldOfView;
}
