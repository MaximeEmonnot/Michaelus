#pragma once
#include <memory>

#include "Transform.h"

// Forward declarations
class Actor;

/*
 * Classe Component
 * Définit un élément pouvant être associé à un Actor pour lui apporter des fonctionnalités
 */
class Component
{
public:
	// Constructeur définissant l'acteur lié au Component
	Component(Actor& rOwner);

	// Méthode virtuelle pure (=abstraite) Update()
	virtual void Update() = 0;

	// Attache le Component à un Component parent
	void AttachTo(std::shared_ptr<Component> pNewParentComponent);
	// Détache le Component du Component parent
	void Detach();

	// Récupération de l'acteur propriétaire du Component
	Actor& GetOwner();

	// Ajout d'un décalage à la position relative du Component par rapport à son parent
	void AddRelativeLocation(const FVec3D& offsetLocation);
	// Ajout d'une rotation à la rotation relative du Component par rapport à son parent 
	void AddRelativeRotation(const FQuaternion& offsetRotation);

	// Récupération de la position relative du Component par rapport à son parent
	FVec3D GetRelativeLocation() const;
	// Récupération de la rotation relative du Component par rapport à son parent
	FQuaternion GetRelativeRotation() const;
	// Récupération de la position globale du Component
	FVec3D GetWorldLocation() const;
	// Récupération de la rotation globale du Component
	FQuaternion GetWorldRotation() const;

	// Récupération du Transform du Component
	FTransform GetTransform() const;

	// Récupération du vecteur avant du Component (Axe X relatif au Component)
	FVec3D GetForwardVector() const;
	// Récupération du vecteur droite du Component (Axe Y relatif au Component)
	FVec3D GetRightVector() const;
	// Récupération du vecteur haut du Component (Axe Z relatif au Component)
	FVec3D GetUpVector() const;

private:
	Actor& rOwner;

	std::shared_ptr<Component> pParentComponent;

	FTransform transform;
};
