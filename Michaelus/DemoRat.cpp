#include "DemoRat.h"

#include "Material.h"
#include "MaterialFactory.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Mouse.h"
#include "SoundSystem.h"
#include "Timer.h"

// Constructeur d�finissant le nom de l'acteur
DemoRat::DemoRat(const std::string& name)
	:
	Actor(name)
{
	pMeshComponent = CreateComponent<MeshComponent>();
	pRootComponent = pMeshComponent;

	// On d�finit la liste des mat�riaux � appliquer au Mesh
	std::vector<std::shared_ptr<Material>> materials;
	materials.push_back(MATERIAL("Textures/rat.png", EShadingModel::Lit));

	// On cr�� le mesh et on lui applique les mat�riaux
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>("Meshes/rat2.obj", materials);
	pMeshComponent->SetMesh(mesh);
}

// M�thode Update() de l'acteur, surcharg�e
void DemoRat::Update()
{
	Actor::Update();
}
