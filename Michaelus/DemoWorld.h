#pragma once
#include "World.h"

// Forward declarations
class DemoLightAndSound;
class DemoShrek;
class DemoPlane;
class DemoPawn;
class DemoRat;

/*
 * Classe DemoWorld
 * D�monstration d'une sc�ne poss�dant plusieurs Actor et Pawn
 */
class DemoWorld :
    public World
{
public:
	// Constructeur
    DemoWorld();

	// M�thode BeginPlay(), surcharg�e
    virtual void BeginPlay() override;
	// M�thode Update(), surcharg�e
    virtual void Update() override;

private:
	std::shared_ptr<DemoRat> pTestRat1;
	std::shared_ptr<DemoRat> pTestRat2;
	std::shared_ptr<DemoRat> pTestRat3;
	std::shared_ptr<DemoRat> pTestRat4;
	std::shared_ptr<DemoRat> pTestRat5;

	std::shared_ptr<DemoPawn> pTestPawn;

	std::shared_ptr<DemoPlane> pDemoPlane;

	std::shared_ptr<DemoShrek> pDemoShrek;

	std::shared_ptr<DemoLightAndSound> pDemoLightSound;
};

