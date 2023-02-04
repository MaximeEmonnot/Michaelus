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
}
