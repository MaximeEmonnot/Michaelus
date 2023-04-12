#include "Actor.h"

#include "CameraManager.h"
#include "Component.h"
#include "Graphics.h"

// Constructeur définissant le nom de l'acteur
Actor::Actor(const std::string& name)
	:
	name(name)
{
}

// Méthode Update() de l'acteur : Appelle les méthodes Update() de tous les Components de l'Actor
void Actor::Update()
{
	for (std::shared_ptr<Component> component : components) component->Update();
}

// Récupération du nom de l'acteur
std::string Actor::GetName() const
{
	return name;
}

// Récupération du Component racine
std::shared_ptr<Component> Actor::GetRootComponent() const
{
	return pRootComponent;
}

// Définition de la position de l'Actor
void Actor::SetActorLocation(const FVec3D& newLocation)
{
	transform.location = newLocation;
}

// Définition de la rotation de l'Actor
void Actor::SetActorRotation(const FQuaternion& newRotation)
{
	transform.rotation = newRotation;
}

// Ajout d'un décalage à la position de l'Actor
void Actor::AddActorLocation(const FVec3D& offsetLocation)
{
	transform.location += offsetLocation;
}

// Ajout d'une rotation à la rotation de l'Actor
void Actor::AddActorRotation(const FQuaternion& offsetRotation)
{
	transform.rotation = transform.rotation * offsetRotation;
}

// Récupération de la position de l'Actor
FVec3D Actor::GetActorLocation() const
{
	return transform.location;
}

// Récupération de la rotation de l'Actor
FQuaternion Actor::GetActorRotation() const
{
	return transform.rotation;
}

// Récupération du Transform de l'Actor
FTransform Actor::GetActorTransform() const
{
	return transform;
}

// Récupération du vecteur avant de l'Actor (Axe X relatif à l'Actor)
FVec3D Actor::GetActorForwardVector() const
{
	return transform.rotation.UnrotateVector(FVec3D(1.f, 0.f, 0.f));
}

// Récupération du vecteur droite de l'Actor (Axe Y relatif à l'Actor)
FVec3D Actor::GetActorRightVector() const
{
	return transform.rotation.UnrotateVector(FVec3D(0.f, 1.f, 0.f));
}

// Récupération du vecteur haut de l'Actor (Axe Z relatif à l'Actor)
FVec3D Actor::GetActorUpVector() const
{
	return transform.rotation.UnrotateVector(FVec3D(0.f, 0.f, 1.f));
}
