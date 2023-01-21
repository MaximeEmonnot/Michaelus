#include "Game.h"

#include "Graphics.h"
#include "VKDevice.h"

Game::Game()
{
	VK_DEVICE;
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
