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

DemoWorld::DemoWorld()
{
    pTestActor1 = SpawnActor<DemoRat>("Test Actor 1");
    pTestActor2 = SpawnActor<DemoRat>("Test Actor 1");
    pTestActor3 = SpawnActor<DemoRat>("Test Actor 1");
    pTestActor4 = SpawnActor<DemoRat>("Test Actor 1");
    pTestActor5 = SpawnActor<DemoRat>("Test Actor 1");

    pTestPawn = SpawnActor<DemoPawn>("Test Pawn");

    pDemoPlane = SpawnActor<DemoPlane>("Demo Plane");

    pDemoShrek = SpawnActor<DemoShrek>("Demo Shrek");

    pDemoLightSound = SpawnActor<DemoLightAndSound>("Demo Light Sound");

    //LIGHT_SYSTEM.SetDirectionalLight({ 1.f, 0.f, 1.f }, { 1.f, 1.f, 1.f }, 1.f);
}

void DemoWorld::BeginPlay()
{
    pTestActor1->SetActorLocation({ -2.f, 0.f, 0.f });
    pTestActor2->SetActorLocation({ 1.f, 1.f, 0.f });
    pTestActor3->SetActorLocation({ 1.f, -1.f, 0.f });
    pTestActor4->SetActorLocation({ -1.f, 1.f, 0.f });
    pTestActor5->SetActorLocation({ -1.f, -1.f, 0.f });

    pTestPawn->SetActorLocation({ -0.5f, 0.f, 1.5f });

    pDemoPlane->SetActorLocation({ 0.f, 0.f, -0.5f });

    pDemoShrek->SetActorLocation({ 0.f, 0.f, -0.5f });

    pDemoLightSound->SetActorLocation({ 0.f, 0.f, 1.f });
}

void DemoWorld::Update()
{
    const float angle = MMath::Rad(45.f) * DELTATIME;

    pTestActor1->AddActorRotation(FQuaternion(angle, 0.f, 0.f));
    pTestActor2->AddActorRotation(FQuaternion(0.f, angle, 0.f));
    pTestActor3->AddActorRotation(FQuaternion(0.f, 0.f, angle));
    pTestActor4->AddActorRotation(FQuaternion(angle, 0.f, angle));
    pTestActor5->AddActorRotation(FQuaternion(angle, angle, 0.f));

    pDemoShrek->AddActorRotation(FQuaternion(0.f, 0.f, angle));

    for (auto& actor : actors) actor->Update();
}
