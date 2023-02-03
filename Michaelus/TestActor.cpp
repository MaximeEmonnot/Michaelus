#include "TestActor.h"

#include "Material.h"
#include "MaterialFactory.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Mouse.h"
#include "SoundSystem.h"
#include "Timer.h"

TestActor::TestActor(const std::string& name)
	:
	Actor(name)
{
	pMeshComponent = CreateComponent<MeshComponent>();
	pRootComponent = pMeshComponent;

	std::vector<std::shared_ptr<Material>> materials;
	materials.push_back(MATERIAL("Textures/rat.png", EShadingModel::Lit));

	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>("Meshes/rat2.obj", materials);
	pMeshComponent->SetMesh(mesh);
}

void TestActor::Update()
{
	Actor::Update();

	switch(MOUSE.Read())
	{
	case Mouse::EventType::LPress:
		//SFX.PlaySoundAtLocation("Sounds/test.wav", FVec3D(5000.f, 0.f, 0.f));
		break;
	case Mouse::EventType::WheelDown:
		angle += 5.f;
		break;
	case Mouse::EventType::WheelUp:
		angle -= 5.f;
		break;
	}

	AddActorRotation(FRotator(MMath::Rad(angle), 0.f, 0.f) * DELTATIME);
}
