#include "MeshComponent.h"

#include "Actor.h"
#include "Mesh.h"
#include "Graphics.h"

// Constructeur d�finissant l'acteur li� au Component
MeshComponent::MeshComponent(Actor& rOwner)
	:
	Component(rOwner)
{}

// M�thode Update() h�rit�e de Component : Envoie de la position de l'acteur au Mesh pour l'affichage
void MeshComponent::Update()
{
	pMesh->Update(GetOwner().GetActorTransform());
}

// D�finition du Mesh
void MeshComponent::SetMesh(std::shared_ptr<Mesh> pNewMesh)
{
	pMesh = pNewMesh;
}

// R�cup�ration du Mesh
std::shared_ptr<Mesh> MeshComponent::GetMesh() const
{
	return pMesh;
}
