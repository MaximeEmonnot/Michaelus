#include "Game.h"

#include "Graphics.h"

Game::Game()
{
	GFX.Draw();
}

void Game::Go()
{
	UpdateFrame();
	RenderFrame();
}

void Game::UpdateFrame() const
{
}

void Game::RenderFrame() const
{
}
