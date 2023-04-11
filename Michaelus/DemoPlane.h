#pragma once
#include "Actor.h"

/*
 * Classe DemoPlane
 * D�monstration d'un Actor plan, avec un Mesh
 */
class DemoPlane :
    public Actor
{
public:
    // Constructeur par d�faut
    DemoPlane() = default;
    // Constructeur d�finissant le nom de l'acteur
    DemoPlane(const std::string& name);

    // M�thode Update() de l'acteur, surcharg�e
    virtual void Update() override;

private:
    std::shared_ptr<MeshComponent> pMeshComponent;
};

