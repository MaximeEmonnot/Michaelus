#include "MeshComponent.h"

#include "Actor.h"
#include "Mesh.h"
#include "Graphics.h"

MeshComponent::MeshComponent(Actor& rOwner)
	:
	Component(rOwner)
{
}

void MeshComponent::Update()
{
	pMesh->Update(GetOwner().GetActorTransform());
}

void MeshComponent::SetMesh(std::shared_ptr<Mesh> pNewMesh)
{
	pMesh = pNewMesh;
}

std::shared_ptr<Mesh> MeshComponent::GetMesh() const
{
	return pMesh;
}
