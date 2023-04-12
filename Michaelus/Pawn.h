#pragma once
#include "Actor.h"

// Forward declarations
class Controller;

/*
 * Classe Pawn
 * D�finit un Actor pouvant �tre poss�d�/contr�l� par un Controller, c'est-�-dire un Pion
 * Utilisation d'une Strat�gie pour le contr�le par le Controller
 */
class Pawn :
    public Actor
{
public:
    // Constructeur par d�faut
    Pawn() = default;
    // Constructeur d�finissant le nom du pion
    Pawn(const std::string& name);

    // D�finition du Controller du pion
    template<class T>
    void SetController()
    {
        // Une exception est lev�e si la classe entre chevrons n'h�rite pas de Controller
        if (!std::is_base_of<Controller, T>())
            throw ENGINE_EXCEPTION("Michaelus 3D Engine - Main Engine Exception", "This is not a Controller Class. Please check your call for SetController.");

        pController = std::make_shared<T>(*this);
    }

    // M�thode Update() du pion, surcharg�e
    virtual void Update() override;

    // R�cup�ration du Controller
    std::shared_ptr<Controller> GetController() const;

private:
    std::shared_ptr<Controller> pController;
};