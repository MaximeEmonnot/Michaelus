#pragma once
#include <memory>
#include <vector>

#include "EngineException.h"

#define ENGINE Engine::GetInstance()

class World;

class DemoPawn;
class DemoRat;

/*
 * Classe Engine
 * Classe principale définissant l'ensemble des scènes, et le déroulement de tout le programme
 */
class Engine
{
public:
	// Constructeur (publique car std::unique_ptr nécessite un constructeur publique) 
	Engine();
	// Destructeur par défaut
	~Engine() = default;

	// Ces différents constructeurs sont définis comme étant supprimés, pour correspondre au patron de conception Singleton
	// et également à la RO5 des normes du C++
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;
	Engine(Engine&&) = delete;
	Engine& operator=(Engine&&) = delete;

	// Méthode du patron de conception Singleton
	static Engine& GetInstance();

	// Définition du monde en cours, s'il a bien été ajouté la liste des mondes du moteur
	template<class T>
	void SetWorld()
	{
		// Une exception est levée si la classe entre chevrons n'hérite pas de World
		static_assert(std::is_base_of_v<World, T>);

		for (std::shared_ptr<World> world : worlds)
		{
			if (std::dynamic_pointer_cast<T>(world))
			{
				pCurrentWorld = world;
				return;
			}
		}
		throw ENGINE_EXCEPTION("Vulkan 3D Engine - Standard Exception", "This world doesn't exist! Please check your code.");
	}

	// Récupération du monde courant
	std::shared_ptr<World> GetCurrentWorld() const;

	// Exécution principale du moteur
	void Run();

private:
	// Ajout d'un monde dans la liste des mondes (et définition du monde courant si premier monde)
	template<class T>
	void AddWorld()
	{
		static_assert(std::is_base_of_v<World, T>);

		worlds.emplace_back(std::make_shared<T>());
		if (!pCurrentWorld) pCurrentWorld = worlds.front();
	}

	// Mise à jour de la scène courante
	void UpdateFrame() const;

private:
	static std::unique_ptr<Engine> pInstance;

	std::vector<std::shared_ptr<World>> worlds;
	std::shared_ptr<World> pCurrentWorld;
};

