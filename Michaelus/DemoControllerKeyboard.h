#pragma once
#include "Controller.h"

/*
 * Classe DemoControllerKeyboard
 * Démonstration d'un Controller concret pour le contrôle d'un pion où la rotation est définie par le clavier
 */
class DemoControllerKeyboard :
    public Controller
{
public:
    // Constructeur définissant le Pawn lié au Controller
    DemoControllerKeyboard(Pawn& pawn);

    // Méthode concrète Update()
    virtual void Update() override;
};

