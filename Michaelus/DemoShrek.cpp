#include "DemoShrek.h"

#include "Material.h"
#include "MaterialFactory.h"
#include "Mesh.h"

// Constructeur définissant le nom de l'acteur
DemoShrek::DemoShrek(const std::string& name)
{
	pMeshComponent = CreateComponent<MeshComponent>();
	pRootComponent = pMeshComponent;

	// On définit la liste des matériaux à appliquer au Mesh
	std::vector<std::shared_ptr<Material>> materials;
	materials.push_back(MATERIAL("Textures/shrekball.jpeg", EShadingModel::Lit));
	materials.push_back(MATERIAL("Textures/ShrekBody_Col.png", EShadingModel::Lit));
	materials.push_back(MATERIAL("Textures/ShrekHead_Col.png", EShadingModel::Lit));

	// On créé le mesh et on lui applique les matériaux
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>("Meshes/Shrek.obj", materials);
	pMeshComponent->SetMesh(mesh);
}

// Méthode Update() de l'acteur, surchargée
void DemoShrek::Update()
{
	Actor::Update();
}
