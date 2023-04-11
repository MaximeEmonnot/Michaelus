#include "CameraComponent.h"

#include "SoundSystem.h"

// Constructeur définissant l'acteur lié au Component
CameraComponent::CameraComponent(Actor& rOwner)
	:
	Component(rOwner)
{}

// Méthode Update() héritée de Component : On définit la position du Listener de AudioManager
void CameraComponent::Update()
{
	SFX.SetListenerLocation(GetWorldLocation());
}

// Définition de la sensibilité de la caméra
void CameraComponent::SetSensitivity(float newSensitivity)
{
	sensitivity = newSensitivity;
}

// Récupération de la sensibilité de la caméra
float CameraComponent::GetSensitivity() const
{
	return sensitivity;
}

// Définition du champ de vision (FOV) de la caméra
void CameraComponent::SetFieldOfView(float newFieldOfView)
{
	fieldOfView = newFieldOfView;
}

// Récupération du champ de vision (FOV) de la caméra
float CameraComponent::GetFieldOfView() const
{
	return fieldOfView;
}
