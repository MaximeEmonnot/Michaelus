#include "MeshComponent.h"

#include "Actor.h"
#include "Mesh.h"
#include "Graphics.h"

// Constructeur définissant l'acteur lié au Component
MeshComponent::MeshComponent(Actor& rOwner)
	:
	Component(rOwner)
{}

// Méthode Update() héritée de Component : Envoie de la position de l'acteur au Mesh pour l'affichage
void MeshComponent::Update()
{
	pMesh->Update(GetOwner().GetActorTransform());
}

// Définition du Mesh
void MeshComponent::SetMesh(std::shared_ptr<Mesh> pNewMesh)
{
	pMesh = pNewMesh;
}

// Récupération du Mesh
std::shared_ptr<Mesh> MeshComponent::GetMesh() const
{
	return pMesh;
}
