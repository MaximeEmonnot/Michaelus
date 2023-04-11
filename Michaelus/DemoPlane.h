#pragma once
#include "Actor.h"

/*
 * Classe DemoPlane
 * Démonstration d'un Actor plan, avec un Mesh
 */
class DemoPlane :
    public Actor
{
public:
    // Constructeur par défaut
    DemoPlane() = default;
    // Constructeur définissant le nom de l'acteur
    DemoPlane(const std::string& name);

    // Méthode Update() de l'acteur, surchargée
    virtual void Update() override;

private:
    std::shared_ptr<MeshComponent> pMeshComponent;
};

