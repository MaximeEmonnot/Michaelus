#pragma once
#include "Pawn.h"

// Forward declarations
class SceneComponent;
class CameraComponent;

/*
 * Classe DemoPawn
 * Démonstration d'un pion présent dans la scène, possédant une caméra pour l'affichage
 */
class DemoPawn :
    public Pawn
{
public:
    // Constructeur définissant le nom du pion
    DemoPawn(const std::string& name);

    // Méthode Update() du pion, surchargée
    virtual void Update() override;

    // Récupération de la caméra
    std::shared_ptr<CameraComponent> GetCamera() const;

private:
    std::shared_ptr<SceneComponent> pSceneComponent;

    std::shared_ptr<CameraComponent> pCameraComponent;
};

