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

void CameraComponent::SetFieldOfView(float newFieldOfView)
{
	fieldOfView = newFieldOfView;
}

float CameraComponent::GetFieldOfView() const
{
	return fieldOfView;
}
