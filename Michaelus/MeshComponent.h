#pragma once
#include "Component.h"
class Mesh;

/*
 * Classe MeshComponent
 * Component définissant un Mesh à afficher dans la scène
 */
class MeshComponent :
    public Component
{
public:
    // Constructeur définissant l'acteur lié au Component
    MeshComponent(Actor& rOwner);

    // Méthode Update() héritée de Component
    virtual void Update() override;

    // Définition du Mesh
    void SetMesh(std::shared_ptr<Mesh> pNewMesh);
    // Récupération du Mesh
    std::shared_ptr<Mesh> GetMesh() const;

private:
    std::shared_ptr<Mesh> pMesh;
};

