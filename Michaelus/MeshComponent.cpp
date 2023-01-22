#include "MeshComponent.h"

#include "Mesh.h"

MeshComponent::MeshComponent(std::weak_ptr<Actor> pOwner)
	:
	Component(pOwner)
{
}

void MeshComponent::Update()
{
	pMesh->Update(GetTransform());
}

void MeshComponent::SetMesh(std::shared_ptr<Mesh> pNewMesh)
{
	pMesh = pNewMesh;
}

std::shared_ptr<Mesh> MeshComponent::GetMesh() const
{
	return pMesh;
}
