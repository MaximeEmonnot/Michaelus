#include "Game.h"

#include "Graphics.h"
#include "VKDevice.h"

Game::Game()
{
	GFX;
}

void Game::Go()
{
	GFX.BeginDraw();
	UpdateFrame();
	RenderFrame();
	GFX.EndDraw();
}

void Game::UpdateFrame() const
{
}

void Game::RenderFrame() const
{
}
