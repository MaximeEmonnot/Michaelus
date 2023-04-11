#include "DemoRat.h"

#include "Material.h"
#include "MaterialFactory.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Mouse.h"
#include "SoundSystem.h"
#include "Timer.h"

// Constructeur définissant le nom de l'acteur
DemoRat::DemoRat(const std::string& name)
	:
	Actor(name)
{
	pMeshComponent = CreateComponent<MeshComponent>();
	pRootComponent = pMeshComponent;

	// On définit la liste des matériaux à appliquer au Mesh
	std::vector<std::shared_ptr<Material>> materials;
	materials.push_back(MATERIAL("Textures/rat.png", EShadingModel::Lit));

	// On créé le mesh et on lui applique les matériaux
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>("Meshes/rat2.obj", materials);
	pMeshComponent->SetMesh(mesh);
}

// Méthode Update() de l'acteur, surchargée
void DemoRat::Update()
{
	Actor::Update();
}
