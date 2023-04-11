#include "DemoPlane.h"

#include "Material.h"
#include "MaterialFactory.h"
#include "Mesh.h"

// Constructeur d�finissant le nom de l'acteur
DemoPlane::DemoPlane(const std::string& name)
	:
	Actor(name)
{
	pMeshComponent = CreateComponent<MeshComponent>();
	pRootComponent = pMeshComponent;

	// On d�finit la liste des mat�riaux � appliquer au Mesh
	std::vector<std::shared_ptr<Material>> materials;
	materials.push_back(MATERIAL("Textures/texture.jpg", EShadingModel::Cel));

	// On cr�� le mesh et on lui applique les mat�riaux
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>("Meshes/plane.obj", materials);
	pMeshComponent->SetMesh(mesh);
}

// M�thode Update() de l'acteur, surcharg�e
void DemoPlane::Update()
{
	Actor::Update();
}
