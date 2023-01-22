#include "TestActor.h"

#include "Graphics.h"
#include "Material.h"
#include "Mesh.h"

TestActor::TestActor(const std::string& name)
	:
	Actor(name)
{
	pMeshComponent = CreateComponent<MeshComponent>();
	pRootComponent = pMeshComponent;
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>("Meshes/viking_room.obj");
	mesh->SetMaterial(std::make_shared<Material>("Textures/viking_room.png", EShadingModel::Unlit));
	pMeshComponent->SetMesh(mesh);
	GFX.AddMesh(pMeshComponent);
}

void TestActor::Update()
{
	Actor::Update();

	AddActorRotation({ 0.f, 0.0f , 0.001f });
}
