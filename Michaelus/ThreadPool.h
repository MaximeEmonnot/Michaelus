#pragma once
#include <condition_variable>
#include <thread>
#include <mutex>
#include <functional>
#include <queue>
#include <memory>

#define THREAD ThreadPool::GetInstance()
/*
 * Classe ThreadPool d�finissant une liste de threads et une liste de t�ches
 * Tous les threads sont ouverts et en attente, ex�cutant les t�ches au fur et � mesure qu'elles sont ajout�s � la liste
 * Un thread initialement en attente est notifi� � l'ajout d'une t�che
 */
class ThreadPool
{
public:
	// Constructeur (public car std::unique_ptr n�cessite un constructeur publique) 
	ThreadPool();
	// Destructeur
	~ThreadPool();

	// Ces diff�rents constructeurs sont d�finis comme �tant supprim�s, pour correspondre au patron de conception Singleton
	// et �galement � la RO5 des normes du C++
	ThreadPool(const ThreadPool&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;
	ThreadPool(ThreadPool&&) = delete;
	ThreadPool& operator=(ThreadPool&&) = delete;

	// M�thode du patron de conception Singleton
	static ThreadPool& GetInstance();

	// Ajoute une t�che � la liste qui sera ex�cut�e par l'un des threads en attente
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

