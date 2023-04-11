#include "Component.h"

#include "Actor.h"

// Constructeur d�finissant l'acteur li� au Component
Component::Component(Actor& rOwner)
	:
	rOwner(rOwner)
{
}

// Attache le Component � un Component parent
void Component::AttachTo(std::shared_ptr<Component> pNewParentComponent)
{
	pParentComponent = pNewParentComponent;
}

// D�tache le Component du Component parent
void Component::Detach()
{
	pParentComponent = nullptr;
}

// R�cup�ration de l'acteur propri�taire du Component
Actor& Component::GetOwner()
{
	return rOwner;
}

// Ajout d'un d�calage � la position relative du Component par rapport � son parent
void Component::AddRelativeLocation(const FVec3D& offsetLocation)
{
	transform.location += offsetLocation;
}

// Ajout d'une rotation � la rotation relative du Component par rapport � son parent 
void Component::AddRelativeRotation(const FQuaternion& offsetRotation)
{
	transform.rotation = transform.rotation * offsetRotation;
}

// R�cup�ration de la position relative du Component par rapport � son parent
FVec3D Component::GetRelativeLocation() const
{
	return transform.location;
}

// R�cup�ration de la rotation relative du Component par rapport � son parent
FQuaternion Component::GetRelativeRotation() const
{
	return transform.rotation;
}

// R�cup�ration de la position globale du Component : m�thode r�cursive avec la position du parent
FVec3D Component::GetWorldLocation() const
{
	if (pParentComponent) 
		return pParentComponent->GetWorldLocation() + pParentComponent->GetRelativeRotation().UnrotateVector(transform.location);
	return rOwner.GetActorLocation() + rOwner.GetActorRotation().UnrotateVector(transform.location);
}

// R�cup�ration de la rotation globale du Component : m�thode r�cursive avec la rotation du parent
FQuaternion Component::GetWorldRotation() const
{
	if (pParentComponent) return pParentComponent->GetWorldRotation() * transform.rotation;
	return rOwner.GetActorRotation() * transform.rotation;
}

// R�cup�ration du Transform du Component
FTransform Component::GetTransform() const
{
	return transform;
}

// R�cup�ration du vecteur avant du Component (Axe X relatif au Component)
FVec3D Component::GetForwardVector() const
{
	return GetWorldRotation().UnrotateVector(FVec3D(1.f, 0.f ,0.f));
}

// R�cup�ration du vecteur droite du Component (Axe Y relatif au Component)
FVec3D Component::GetRightVector() const
{
	return GetWorldRotation().UnrotateVector(FVec3D(0.f, 1.f, 0.f));
}

// R�cup�ration du vecteur haut du Component (Axe Z relatif au Component)
FVec3D Component::GetUpVector() const
{
	return GetWorldRotation().UnrotateVector(FVec3D(0.f, 0.f, 1.f));
}
