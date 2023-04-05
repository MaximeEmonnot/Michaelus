#include "DemoControllerMouse.h"

#include "DemoControllerKeyboard.h"
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

	// Position
	if (KBD.KeyIsPressed('Z')) GetPawn().AddActorLocation(GetPawn().GetActorForwardVector() * DELTATIME);
	if (KBD.KeyIsPressed('D')) GetPawn().AddActorLocation(GetPawn().GetActorRightVector() * DELTATIME);
	if (KBD.KeyIsPressed('S')) GetPawn().AddActorLocation(-GetPawn().GetActorForwardVector() * DELTATIME);
	if (KBD.KeyIsPressed('Q')) GetPawn().AddActorLocation(-GetPawn().GetActorRightVector() * DELTATIME);
	if (KBD.KeyIsPressed('E')) GetPawn().AddActorLocation(GetPawn().GetActorUpVector() * DELTATIME);
	if (KBD.KeyIsPressed('A')) GetPawn().AddActorLocation(-GetPawn().GetActorUpVector() * DELTATIME);

	// Rotation
	const FVec2D mouseMovement = MOUSE.GetPosition() * DELTATIME;
	GetPawn().AddActorRotation(FQuaternion(GetPawn().GetActorRightVector(), MMath::Rad(-mouseMovement.GetY())));
	GetPawn().AddActorRotation(FQuaternion(FVec3D(0.f, 0.f, 1.f), MMath::Rad(-mouseMovement.GetX())));

	// Switch to DemoControllerKeyboard
	if (MOUSE.Read() == Mouse::EventType::MRelease) GetPawn().SetController<DemoControllerKeyboard>();
}
