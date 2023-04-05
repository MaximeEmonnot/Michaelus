#include "DemoControllerMouse.h"

#include "DemoPawn.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Pawn.h"
#include "Timer.h"
#include "Vec2D.h"

DemoControllerMouse::DemoControllerMouse(Pawn& pawn)
	:
	Controller(pawn)
{
}

void DemoControllerMouse::Update()
{
	Controller::Update();

	if (KBD.KeyIsPressed('Z')) GetPawn().AddActorLocation(GetPawn().GetActorForwardVector() * DELTATIME);
	if (KBD.KeyIsPressed('D')) GetPawn().AddActorLocation(GetPawn().GetActorRightVector() * DELTATIME);
	if (KBD.KeyIsPressed('S')) GetPawn().AddActorLocation(-GetPawn().GetActorForwardVector() * DELTATIME);
	if (KBD.KeyIsPressed('Q')) GetPawn().AddActorLocation(-GetPawn().GetActorRightVector() * DELTATIME);
	if (KBD.KeyIsPressed('E')) GetPawn().AddActorLocation(GetPawn().GetActorUpVector() * DELTATIME);
	if (KBD.KeyIsPressed('A')) GetPawn().AddActorLocation(-GetPawn().GetActorUpVector() * DELTATIME);

	const FVec2D mouseMovement = MOUSE.GetPosition() * DELTATIME;
	GetPawn().AddActorRotation(FQuaternion(0.f, MMath::Rad(-mouseMovement.GetY()), MMath::Rad(-mouseMovement.GetX())));
}
