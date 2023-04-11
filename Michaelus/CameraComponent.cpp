#include "CameraComponent.h"

#include "SoundSystem.h"

// Constructeur d�finissant l'acteur li� au Component
CameraComponent::CameraComponent(Actor& rOwner)
	:
	Component(rOwner)
{}

// M�thode Update() h�rit�e de Component : On d�finit la position du Listener de AudioManager
void CameraComponent::Update()
{
	SFX.SetListenerLocation(GetWorldLocation());
}

// D�finition de la sensibilit� de la cam�ra
void CameraComponent::SetSensitivity(float newSensitivity)
{
	sensitivity = newSensitivity;
}

// R�cup�ration de la sensibilit� de la cam�ra
float CameraComponent::GetSensitivity() const
{
	return sensitivity;
}

// D�finition du champ de vision (FOV) de la cam�ra
void CameraComponent::SetFieldOfView(float newFieldOfView)
{
	fieldOfView = newFieldOfView;
}

// R�cup�ration du champ de vision (FOV) de la cam�ra
float CameraComponent::GetFieldOfView() const
{
	return fieldOfView;
}
