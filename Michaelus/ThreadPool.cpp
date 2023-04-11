#include "ThreadPool.h"

#include "EngineException.h"

// VARIABLES STATIQUES
std::unique_ptr<ThreadPool> ThreadPool::pInstance = nullptr;

// ********* //

// Constructeur
ThreadPool::ThreadPool()
	:
	numThreads(std::thread::hardware_concurrency()) // On définit le nombre maximal de threads comme étant le nombre de coeur LOGIQUES du processeur
{
	// Initialisation des threads :
	for (size_t i = 0; i < numThreads; ++i) {
		// On lance un thread avec une boucle infinie :
		threads.emplace_back([&] {
			while (true) {
				std::function<void()> task;
				{
					// On met le thread en attente d'une notification ...
					std::unique_lock<std::mutex> lock(eventMutex);
					eventVar.wait(lock, [&] { return bStopping || !tasks.empty(); });
					if (bStopping) break;

					// ... et une fois la notification survenue, on récupère la tâche ...
					task = std::move(tasks.front());
					tasks.pop();
				}
				try {
					// ... que l'on exécute
					task();
				}
				catch (EngineException& e)
				{
					SHOW_MESSAGE("DirectX 2D Engine - Engine Exception", e.what());
				}
				catch (std::exception& e)
				{
					SHOW_MESSAGE("DirectX 2D Engine - Standard Library Exception", e.what());
				}
				catch (...)
				{
					SHOW_MESSAGE("DirectX 2D Engine - Unknown Exception", "An unknown exception has been caught.");
				}
			}
			});
	}
}

// Destructeur
ThreadPool::~ThreadPool()
{
	{
		std::unique_lock<std::mutex> lock(eventMutex);
		bStopping = true;
	}
	// On notifie tous les threads pour réaliser un join vers le thread principal en toute sécurité
	eventVar.notify_all();

	for (std::thread& thread : threads) thread.join();
}

// Méthode du patron de conception Singleton
ThreadPool& ThreadPool::GetInstance()
{
	if (!pInstance)
		pInstance = std::make_unique<ThreadPool>();
	return *pInstance;
}

// Ajoute une tâche à la liste qui sera exécutée par l'un des threads en attente
void ThreadPool::Enqueue(std::function<void()> task)
{
	{
		std::unique_lock<std::mutex> lock(eventMutex);
		tasks.emplace(std::move(task));
	}
	eventVar.notify_one();
}