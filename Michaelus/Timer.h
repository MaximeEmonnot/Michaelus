#pragma once
#include <memory>
#include <chrono>

// Macro pour la mise à jour du Timer
#define TICKCLOCK Timer::GetInstance().Update()
// Macro pour la récupération de la variable DeltaTime
#define DELTATIME Timer::GetInstance().DeltaTime()

/*
 * Classe permettant le calcul du temps écoulé durant une frame
 */
class Timer
{
public:
	// Constructeur (public car std::unique_ptr nécessite un constructeur publique) 
	Timer();
	// Destructeur par défaut
	~Timer() = default;

	// Ces différents constructeurs sont définis comme étant supprimés, pour correspondre au patron de conception Singleton
	// et également à la RO5 des normes du C++
	Timer(const Timer&) = delete;
	Timer& operator= (const Timer&) = delete;
	Timer(Timer&&) = delete;
	Timer& operator= (Timer&&) = delete;

	// Méthode du patron de conception Singleton
	static Timer& GetInstance();

	// Mise à jour de la variable DeltaTime : Temps écoulé durant une frame
	void Update();

	// Récupération de la variable DeltaTime
	float DeltaTime() const;

private:
	static std::unique_ptr<Timer> pInstance;

	std::chrono::steady_clock::time_point last;
	float deltaTime;
};

