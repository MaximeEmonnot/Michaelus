#pragma once
#include <memory>

class Pawn;

class Controller
{
public:
	Controller() = default;
	Controller(std::weak_ptr<Pawn> pPawn);

	virtual void Update();

protected:
	std::shared_ptr<Pawn> GetPawn();

private:
	std::weak_ptr<Pawn> pPawn;
};

