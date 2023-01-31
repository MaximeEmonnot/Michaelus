#include "TestActor.h"

#include "Material.h"
#include "MaterialFactory.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Mouse.h"
#include "Timer.h"

TestActor::TestActor(const std::string& name)
	:
	Actor(name)
{
	pMeshComponent = CreateComponent<MeshComponent>();
	pRootComponent = pMeshComponent;

	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>("Meshes/viking_room.obj", MATERIAL("Textures/viking_room.png", EShadingModel::Lit));
	pMeshComponent->SetMesh(mesh);
}

void TestActor::Update()
{
	Actor::Update();

	if (MOUSE.LeftIsPressed()) angle += 5.f;
	if (MOUSE.RightIsPressed()) angle -= 5.f;

	AddActorRotation(FRotator(0.f, 0.f, MMath::Rad(angle)) * DELTATIME);
}
