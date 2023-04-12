#pragma once
#include <memory>
#include <chrono>

// Macro pour la mise � jour du Timer
#define TICKCLOCK Timer::GetInstance().Update()
// Macro pour la r�cup�ration de la variable DeltaTime
#define DELTATIME Timer::GetInstance().DeltaTime()

/*
 * Classe permettant le calcul du temps �coul� durant une frame
 */
class Timer
{
public:
	// Constructeur (public car std::unique_ptr n�cessite un constructeur publique) 
	Timer();
	// Destructeur par d�faut
	~Timer() = default;

	// Ces diff�rents constructeurs sont d�finis comme �tant supprim�s, pour correspondre au patron de conception Singleton
	// et �galement � la RO5 des normes du C++
	Timer(const Timer&) = delete;
	Timer& operator= (const Timer&) = delete;
	Timer(Timer&&) = delete;
	Timer& operator= (Timer&&) = delete;

	// M�thode du patron de conception Singleton
	static Timer& GetInstance();

	// Mise � jour de la variable DeltaTime : Temps �coul� durant une frame
	void Update();

	// R�cup�ration de la variable DeltaTime
	float DeltaTime() const;

private:
	static std::unique_ptr<Timer> pInstance;

	std::chrono::steady_clock::time_point last;
	float deltaTime;
};

