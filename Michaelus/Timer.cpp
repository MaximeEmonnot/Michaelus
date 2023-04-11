#include "Timer.h"

// VARIABLES STATIQUES
std::unique_ptr<Timer> Timer::pInstance = nullptr;

// ********* //

// Constructeur
Timer::Timer()
	:
	last(std::chrono::steady_clock::now()),
	deltaTime(0.f)
{
}

// M�thode du patron de conception Singleton
Timer& Timer::GetInstance()
{
	if (!pInstance)
		pInstance = std::make_unique<Timer>();
	return *pInstance;
}

// Mise � jour de la variable DeltaTime : Temps �coul� durant une frame
void Timer::Update()
{
	const std::chrono::steady_clock::time_point old = last;
	last = std::chrono::steady_clock::now();
	deltaTime = std::chrono::duration<float>(last - old).count();
}

// R�cup�ration de la variable DeltaTime
float Timer::DeltaTime() const
{
	return deltaTime;
}
