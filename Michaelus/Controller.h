#pragma once
#include <memory>

class Pawn;

class Controller
{
public:
	Controller() = default;
	Controller(Pawn& pawn);

	virtual void Update();

protected:
	Pawn GetPawn() const;

private:
	Pawn& pawn;
};

