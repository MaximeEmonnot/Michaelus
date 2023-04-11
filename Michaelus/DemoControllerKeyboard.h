#pragma once
#include "Controller.h"

/*
 * Classe DemoControllerKeyboard
 * D�monstration d'un Controller concret pour le contr�le d'un pion o� la rotation est d�finie par le clavier
 */
class DemoControllerKeyboard :
    public Controller
{
public:
    // Constructeur d�finissant le Pawn li� au Controller
    DemoControllerKeyboard(Pawn& pawn);

    // M�thode concr�te Update()
    virtual void Update() override;
};

