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
	GFX.Draw();
}

void Game::UpdateFrame() const
{
}

void Game::RenderFrame() const
{
}
