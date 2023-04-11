#include "DemoPlane.h"

#include "Material.h"
#include "MaterialFactory.h"
#include "Mesh.h"

// Constructeur définissant le nom de l'acteur
DemoPlane::DemoPlane(const std::string& name)
	:
	Actor(name)
{
	pMeshComponent = CreateComponent<MeshComponent>();
	pRootComponent = pMeshComponent;

	// On définit la liste des matériaux à appliquer au Mesh
	std::vector<std::shared_ptr<Material>> materials;
	materials.push_back(MATERIAL("Textures/texture.jpg", EShadingModel::Cel));

	// On créé le mesh et on lui applique les matériaux
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>("Meshes/plane.obj", materials);
	pMeshComponent->SetMesh(mesh);
}

// Méthode Update() de l'acteur, surchargée
void DemoPlane::Update()
{
	Actor::Update();
}
