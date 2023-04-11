#pragma once
#include "Controller.h"

/*
 * Classe DemoControllerMouse
 * Démonstration d'un Controller concret pour le contrôle d'un pion où la rotation est définie par la souris
 */
class DemoControllerMouse :
    public Controller
{
public:
    // Constructeur définissant le Pawn lié au Controller
    DemoControllerMouse(Pawn& pawn);

    // Méthode concrète Update()
    virtual void Update() override;
};

