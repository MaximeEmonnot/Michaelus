#include "Game.h"

#include "Graphics.h"
#include "TestActor.h"
#include "TestPawn.h"
#include "Timer.h"
#include "VKDevice.h"

Game::Game()
{
	GFX;

	pTestActor = std::make_shared<TestActor>("Test Actor 1");
	//pTestActor2 = std::make_shared<TestActor>("Test Actor 2");
	pTestPawn = std::make_shared<TestPawn>("Test Pawn");

	pTestActor->SetActorLocation(FVec3D(1.f, 1.f, 0.f));
	//pTestActor2->SetActorLocation(FVec3D(-0.5f, 0.5f, 0.f));
}

void Game::Go()
{
	TICKCLOCK;

	GFX.BeginDraw();
	UpdateFrame();
	GFX.EndDraw();
}

void Game::UpdateFrame() const
{
	pTestActor->Update();
	//pTestActor2->Update();
	pTestPawn->Update();
}

