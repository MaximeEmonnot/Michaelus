#pragma once
#include <condition_variable>
#include <thread>
#include <mutex>
#include <functional>
#include <queue>
#include <memory>

#define THREAD ThreadPool::GetInstance()
/*
 * Classe ThreadPool définissant une liste de threads et une liste de tâches
 * Tous les threads sont ouverts et en attente, exécutant les tâches au fur et à mesure qu'elles sont ajoutés à la liste
 * Un thread initialement en attente est notifié à l'ajout d'une tâche
 */
class ThreadPool
{
public:
	// Constructeur (public car std::unique_ptr nécessite un constructeur publique) 
	ThreadPool();
	// Destructeur
	~ThreadPool();

	// Ces différents constructeurs sont définis comme étant supprimés, pour correspondre au patron de conception Singleton
	// et également à la RO5 des normes du C++
	ThreadPool(const ThreadPool&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;
	ThreadPool(ThreadPool&&) = delete;
	ThreadPool& operator=(ThreadPool&&) = delete;

	// Méthode du patron de conception Singleton
	static ThreadPool& GetInstance();

	// Ajoute une tâche à la liste qui sera exécutée par l'un des threads en attente
	void				Enqueue(std::function<void()> task);

private:
	static std::unique_ptr<ThreadPool>	pInstance;

	const size_t						numThreads;
	std::vector<std::thread>			threads;
	std::queue<std::function<void()>>	tasks;
	std::condition_variable				eventVar;
	std::mutex							eventMutex;
	bool								bStopping = false;
};

