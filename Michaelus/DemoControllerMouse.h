#pragma once
#include "Controller.h"

/*
 * Classe DemoControllerMouse
 * D�monstration d'un Controller concret pour le contr�le d'un pion o� la rotation est d�finie par la souris
 */
class DemoControllerMouse :
    public Controller
{
public:
    // Constructeur d�finissant le Pawn li� au Controller
    DemoControllerMouse(Pawn& pawn);

    // M�thode concr�te Update()
    virtual void Update() override;
};

