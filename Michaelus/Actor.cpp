#include "Actor.h"

#include "CameraManager.h"
#include "Component.h"
#include "Graphics.h"

// Constructeur d�finissant le nom de l'acteur
Actor::Actor(const std::string& name)
	:
	name(name)
{
}

// M�thode Update() de l'acteur : Appelle les m�thodes Update() de tous les Components de l'Actor
void Actor::Update()
{
	for (std::shared_ptr<Component> component : components) component->Update();
}

// R�cup�ration du nom de l'acteur
std::string Actor::GetName() const
{
	return name;
}

// R�cup�ration du Component racine
std::shared_ptr<Component> Actor::GetRootComponent() const
{
	return pRootComponent;
}

// D�finition de la position de l'Actor
void Actor::SetActorLocation(const FVec3D& newLocation)
{
	transform.location = newLocation;
}

// D�finition de la rotation de l'Actor
void Actor::SetActorRotation(const FQuaternion& newRotation)
{
	transform.rotation = newRotation;
}

// Ajout d'un d�calage � la position de l'Actor
void Actor::AddActorLocation(const FVec3D& offsetLocation)
{
	transform.location += offsetLocation;
}

// Ajout d'une rotation � la rotation de l'Actor
void Actor::AddActorRotation(const FQuaternion& offsetRotation)
{
	transform.rotation = transform.rotation * offsetRotation;
}

// R�cup�ration de la position de l'Actor
FVec3D Actor::GetActorLocation() const
{
	return transform.location;
}

// R�cup�ration de la rotation de l'Actor
FQuaternion Actor::GetActorRotation() const
{
	return transform.rotation;
}

// R�cup�ration du Transform de l'Actor
FTransform Actor::GetActorTransform() const
{
	return transform;
}

// R�cup�ration du vecteur avant de l'Actor (Axe X relatif � l'Actor)
FVec3D Actor::GetActorForwardVector() const
{
	return transform.rotation.UnrotateVector(FVec3D(1.f, 0.f, 0.f));
}

// R�cup�ration du vecteur droite de l'Actor (Axe Y relatif � l'Actor)
FVec3D Actor::GetActorRightVector() const
{
	return transform.rotation.UnrotateVector(FVec3D(0.f, 1.f, 0.f));
}

// R�cup�ration du vecteur haut de l'Actor (Axe Z relatif � l'Actor)
FVec3D Actor::GetActorUpVector() const
{
	return transform.rotation.UnrotateVector(FVec3D(0.f, 0.f, 1.f));
}
