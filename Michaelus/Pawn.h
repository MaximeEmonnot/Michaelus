#pragma once
#include "Actor.h"

// Forward declarations
class Controller;

/*
 * Classe Pawn
 * Définit un Actor pouvant être possédé/contrôlé par un Controller, c'est-à-dire un Pion
 * Utilisation d'une Stratégie pour le contrôle par le Controller
 */
class Pawn :
    public Actor
{
public:
    // Constructeur par défaut
    Pawn() = default;
    // Constructeur définissant le nom du pion
    Pawn(const std::string& name);

    // Définition du Controller du pion
    template<class T>
    void SetController()
    {
        // Une exception est levée si la classe entre chevrons n'hérite pas de Controller
        if (!std::is_base_of<Controller, T>())
            throw ENGINE_EXCEPTION("Michaelus 3D Engine - Main Engine Exception", "This is not a Controller Class. Please check your call for SetController.");

        pController = std::make_shared<T>(*this);
    }

    // Méthode Update() du pion, surchargée
    virtual void Update() override;

    // Récupération du Controller
    std::shared_ptr<Controller> GetController() const;

private:
    std::shared_ptr<Controller> pController;
};