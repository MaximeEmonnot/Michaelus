#include "Component.h"

#include "Actor.h"

// Constructeur définissant l'acteur lié au Component
Component::Component(Actor& rOwner)
	:
	rOwner(rOwner)
{
}

// Attache le Component à un Component parent
void Component::AttachTo(std::shared_ptr<Component> pNewParentComponent)
{
	pParentComponent = pNewParentComponent;
}

// Détache le Component du Component parent
void Component::Detach()
{
	pParentComponent = nullptr;
}

// Récupération de l'acteur propriétaire du Component
Actor& Component::GetOwner()
{
	return rOwner;
}

// Ajout d'un décalage à la position relative du Component par rapport à son parent
void Component::AddRelativeLocation(const FVec3D& offsetLocation)
{
	transform.location += offsetLocation;
}

// Ajout d'une rotation à la rotation relative du Component par rapport à son parent 
void Component::AddRelativeRotation(const FQuaternion& offsetRotation)
{
	transform.rotation = transform.rotation * offsetRotation;
}

// Récupération de la position relative du Component par rapport à son parent
FVec3D Component::GetRelativeLocation() const
{
	return transform.location;
}

// Récupération de la rotation relative du Component par rapport à son parent
FQuaternion Component::GetRelativeRotation() const
{
	return transform.rotation;
}

// Récupération de la position globale du Component : méthode récursive avec la position du parent
FVec3D Component::GetWorldLocation() const
{
	if (pParentComponent) 
		return pParentComponent->GetWorldLocation() + pParentComponent->GetRelativeRotation().UnrotateVector(transform.location);
	return rOwner.GetActorLocation() + rOwner.GetActorRotation().UnrotateVector(transform.location);
}

// Récupération de la rotation globale du Component : méthode récursive avec la rotation du parent
FQuaternion Component::GetWorldRotation() const
{
	if (pParentComponent) return pParentComponent->GetWorldRotation() * transform.rotation;
	return rOwner.GetActorRotation() * transform.rotation;
}

// Récupération du Transform du Component
FTransform Component::GetTransform() const
{
	return transform;
}

// Récupération du vecteur avant du Component (Axe X relatif au Component)
FVec3D Component::GetForwardVector() const
{
	return GetWorldRotation().UnrotateVector(FVec3D(1.f, 0.f ,0.f));
}

// Récupération du vecteur droite du Component (Axe Y relatif au Component)
FVec3D Component::GetRightVector() const
{
	return GetWorldRotation().UnrotateVector(FVec3D(0.f, 1.f, 0.f));
}

// Récupération du vecteur haut du Component (Axe Z relatif au Component)
FVec3D Component::GetUpVector() const
{
	return GetWorldRotation().UnrotateVector(FVec3D(0.f, 0.f, 1.f));
}
