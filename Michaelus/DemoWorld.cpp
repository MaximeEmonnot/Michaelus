#include "DemoWorld.h"

#include "Math.h"
#include "Quaternion.h"
#include "Timer.h"
#include "DemoLightAndSound.h"
#include "LightManager.h"
#include "DemoRat.h"
#include "DemoPawn.h"
#include "DemoPlane.h"
#include "DemoShrek.h"
#include "LoggerManager.h"

// Constructeur
DemoWorld::DemoWorld()
{
    // On cr�� les diff�rents Rats
    pTestRat1 = SpawnActor<DemoRat>("Test Rat 1");
    pTestRat2 = SpawnActor<DemoRat>("Test Rat 2");
    pTestRat3 = SpawnActor<DemoRat>("Test Rat 3");
    pTestRat4 = SpawnActor<DemoRat>("Test Rat 4");
    pTestRat5 = SpawnActor<DemoRat>("Test Rat 5");

    // On cr�e le pion du joueur
    pTestPawn = SpawnActor<DemoPawn>("Test Pawn");

    // On cr�e le plan
    pDemoPlane = SpawnActor<DemoPlane>("Demo Plane");

    // On cr�e Shrek
    pDemoShrek = SpawnActor<DemoShrek>("Demo Shrek");

    // On cr�e le syst�me lumineux de d�mo
    pDemoLightSound = SpawnActor<DemoLightAndSound>("Demo Light Sound");

}

// M�thode BeginPlay(), surcharg�e : On d�finit les diff�rentes positions au d�marrage de la sc�ne
void DemoWorld::BeginPlay()
{
    LOG("D�marrage DemoWorld", ELoggerVerbosity::Console)

    pTestRat1->SetActorLocation({ -2.f, 0.f, 0.f });
    pTestRat2->SetActorLocation({ 1.f, 1.f, 0.f });
    pTestRat3->SetActorLocation({ 1.f, -1.f, 0.f });
    pTestRat4->SetActorLocation({ -1.f, 1.f, 0.f });
    pTestRat5->SetActorLocation({ -1.f, -1.f, 0.f });

    pTestPawn->SetActorLocation({ -0.5f, 0.f, 1.5f });

    pDemoPlane->SetActorLocation({ 0.f, 0.f, -0.5f });

    pDemoShrek->SetActorLocation({ 0.f, 0.f, -0.5f });

    pDemoLightSound->SetActorLocation({ 0.f, 0.f, 1.f });
}

// M�thode Update(), surcharg�e
void DemoWorld::Update()
{
    // On applique une rotation aux rats
    const float angle = MMath::Rad(45.f) * DELTATIME;
    pTestRat1->AddActorRotation(FQuaternion(angle, 0.f, 0.f));
    pTestRat2->AddActorRotation(FQuaternion(0.f, angle, 0.f));
    pTestRat3->AddActorRotation(FQuaternion(0.f, 0.f, angle));
    pTestRat4->AddActorRotation(FQuaternion(angle, 0.f, angle));
    pTestRat5->AddActorRotation(FQuaternion(angle, angle, 0.f));

    // On applique une rotation � Shrek
    pDemoShrek->AddActorRotation(FQuaternion(0.f, 0.f, angle));

    // On appelle les m�thodes Update() de chaque acteur
    for (auto& actor : actors) actor->Update();
}
