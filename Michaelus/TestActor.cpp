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
	//materials.push_back(MATERIAL("Textures/Shrek.jpg", EShadingModel::Lit));
	materials.push_back(MATERIAL("Textures/Shrek.jpg", EShadingModel::Cel));

	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>("Meshes/Samba Dancing.fbx", materials);
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

	AddActorRotation(FRotator(0.f, 0.f, MMath::Rad(angle)) * DELTATIME);
}
