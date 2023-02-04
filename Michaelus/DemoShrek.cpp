#include "DemoShrek.h"

#include "Material.h"
#include "MaterialFactory.h"
#include "Mesh.h"

DemoShrek::DemoShrek(const std::string& name)
{
	pMeshComponent = CreateComponent<MeshComponent>();
	pRootComponent = pMeshComponent;

	std::vector<std::shared_ptr<Material>> materials;
	materials.push_back(MATERIAL("Textures/shrekball.jpeg", EShadingModel::Lit));
	materials.push_back(MATERIAL("Textures/ShrekBody_Col.png", EShadingModel::Lit));
	materials.push_back(MATERIAL("Textures/ShrekHead_Col.png", EShadingModel::Lit));

	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>("Meshes/Shrek.obj", materials);
	pMeshComponent->SetMesh(mesh);
}

void DemoShrek::Update()
{
	Actor::Update();
}
