#pragma once
class Game
{
public:
	Game();

	void Go();

private:
	void UpdateFrame() const;
	void RenderFrame() const;
};

