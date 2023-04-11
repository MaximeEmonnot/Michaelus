#include "DemoShrek.h"

#include "Material.h"
#include "MaterialFactory.h"
#include "Mesh.h"

// Constructeur d�finissant le nom de l'acteur
DemoShrek::DemoShrek(const std::string& name)
{
	pMeshComponent = CreateComponent<MeshComponent>();
	pRootComponent = pMeshComponent;

	// On d�finit la liste des mat�riaux � appliquer au Mesh
	std::vector<std::shared_ptr<Material>> materials;
	materials.push_back(MATERIAL("Textures/shrekball.jpeg", EShadingModel::Lit));
	materials.push_back(MATERIAL("Textures/ShrekBody_Col.png", EShadingModel::Lit));
	materials.push_back(MATERIAL("Textures/ShrekHead_Col.png", EShadingModel::Lit));

	// On cr�� le mesh et on lui applique les mat�riaux
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>("Meshes/Shrek.obj", materials);
	pMeshComponent->SetMesh(mesh);
}

// M�thode Update() de l'acteur, surcharg�e
void DemoShrek::Update()
{
	Actor::Update();
}
