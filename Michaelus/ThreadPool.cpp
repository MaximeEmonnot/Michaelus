#include "ThreadPool.h"

#include "EngineException.h"

// VARIABLES STATIQUES
std::unique_ptr<ThreadPool> ThreadPool::pInstance = nullptr;

// ********* //

// Constructeur
ThreadPool::ThreadPool()
	:
	numThreads(std::thread::hardware_concurrency()) // On d�finit le nombre maximal de threads comme �tant le nombre de coeur LOGIQUES du processeur
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

					// ... et une fois la notification survenue, on r�cup�re la t�che ...
					task = std::move(tasks.front());
					tasks.pop();
				}
				try {
					// ... que l'on ex�cute
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
	// On notifie tous les threads pour r�aliser un join vers le thread principal en toute s�curit�
	eventVar.notify_all();

	for (std::thread& thread : threads) thread.join();
}

// M�thode du patron de conception Singleton
ThreadPool& ThreadPool::GetInstance()
{
	if (!pInstance)
		pInstance = std::make_unique<ThreadPool>();
	return *pInstance;
}

// Ajoute une t�che � la liste qui sera ex�cut�e par l'un des threads en attente
void ThreadPool::Enqueue(std::function<void()> task)
{
	{
		std::unique_lock<std::mutex> lock(eventMutex);
		tasks.emplace(std::move(task));
	}
	eventVar.notify_one();
}