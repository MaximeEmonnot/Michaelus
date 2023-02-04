#include "DemoPlane.h"

#include "Material.h"
#include "MaterialFactory.h"
#include "Mesh.h"

DemoPlane::DemoPlane(const std::string& name)
	:
	Actor(name)
{
	pMeshComponent = CreateComponent<MeshComponent>();
	pRootComponent = pMeshComponent;

	std::vector<std::shared_ptr<Material>> materials;
	materials.push_back(MATERIAL("Textures/texture.jpg", EShadingModel::Cel));

	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>("Meshes/plane.obj", materials);
	pMeshComponent->SetMesh(mesh);
}

void DemoPlane::Update()
{
	Actor::Update();
}
