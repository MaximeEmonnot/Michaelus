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

	std::vector<std::shared_ptr<Material>> materials;
	//materials.push_back(MATERIAL("Textures/Shrek.jpg", EShadingModel::Lit));
	materials.push_back(MATERIAL("Textures/rat.png", EShadingModel::Lit));

	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>("Meshes/Rat.fbx", materials);
	pMeshComponent->SetMesh(mesh);
}

void TestActor::Update()
{
	Actor::Update();

	if (MOUSE.LeftIsPressed()) angle += 5.f;
	if (MOUSE.RightIsPressed()) angle -= 5.f;

	AddActorRotation(FRotator(0.f, 0.f, MMath::Rad(angle)) * DELTATIME);
}
