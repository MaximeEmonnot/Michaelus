#pragma once
#include <memory>

class TestPawn;
class TestActor;

class Game
{
public:
	Game();

	void Go();

private:
	void UpdateFrame() const;

private:
	std::shared_ptr<TestActor> pTestActor;
	std::shared_ptr<TestActor> pTestActor2;
	std::shared_ptr<TestPawn> pTestPawn;
};

