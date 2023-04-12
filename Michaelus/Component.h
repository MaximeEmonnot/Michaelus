#pragma once
#include <memory>

#include "Transform.h"

// Forward declarations
class Actor;

/*
 * Classe Component
 * D�finit un �l�ment pouvant �tre associ� � un Actor pour lui apporter des fonctionnalit�s
 */
class Component
{
public:
	// Constructeur d�finissant l'acteur li� au Component
	Component(Actor& rOwner);

	// M�thode virtuelle pure (=abstraite) Update()
	virtual void Update() = 0;

	// Attache le Component � un Component parent
	void AttachTo(std::shared_ptr<Component> pNewParentComponent);
	// D�tache le Component du Component parent
	void Detach();

	// R�cup�ration de l'acteur propri�taire du Component
	Actor& GetOwner();

	// Ajout d'un d�calage � la position relative du Component par rapport � son parent
	void AddRelativeLocation(const FVec3D& offsetLocation);
	// Ajout d'une rotation � la rotation relative du Component par rapport � son parent 
	void AddRelativeRotation(const FQuaternion& offsetRotation);

	// R�cup�ration de la position relative du Component par rapport � son parent
	FVec3D GetRelativeLocation() const;
	// R�cup�ration de la rotation relative du Component par rapport � son parent
	FQuaternion GetRelativeRotation() const;
	// R�cup�ration de la position globale du Component
	FVec3D GetWorldLocation() const;
	// R�cup�ration de la rotation globale du Component
	FQuaternion GetWorldRotation() const;

	// R�cup�ration du Transform du Component
	FTransform GetTransform() const;

	// R�cup�ration du vecteur avant du Component (Axe X relatif au Component)
	FVec3D GetForwardVector() const;
	// R�cup�ration du vecteur droite du Component (Axe Y relatif au Component)
	FVec3D GetRightVector() const;
	// R�cup�ration du vecteur haut du Component (Axe Z relatif au Component)
	FVec3D GetUpVector() const;

private:
	Actor& rOwner;

	std::shared_ptr<Component> pParentComponent;

	FTransform transform;
};
