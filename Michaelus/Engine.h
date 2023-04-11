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
 * Classe principale d�finissant l'ensemble des sc�nes, et le d�roulement de tout le programme
 */
class Engine
{
public:
	// Constructeur (publique car std::unique_ptr n�cessite un constructeur publique) 
	Engine();
	// Destructeur par d�faut
	~Engine() = default;

	// Ces diff�rents constructeurs sont d�finis comme �tant supprim�s, pour correspondre au patron de conception Singleton
	// et �galement � la RO5 des normes du C++
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;
	Engine(Engine&&) = delete;
	Engine& operator=(Engine&&) = delete;

	// M�thode du patron de conception Singleton
	static Engine& GetInstance();

	// D�finition du monde en cours, s'il a bien �t� ajout� la liste des mondes du moteur
	template<class T>
	void SetWorld()
	{
		// Une exception est lev�e si la classe entre chevrons n'h�rite pas de World
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

	// R�cup�ration du monde courant
	std::shared_ptr<World> GetCurrentWorld() const;

	// Ex�cution principale du moteur
	void Run();

private:
	// Ajout d'un monde dans la liste des mondes (et d�finition du monde courant si premier monde)
	template<class T>
	void AddWorld()
	{
		static_assert(std::is_base_of_v<World, T>);

		worlds.emplace_back(std::make_shared<T>());
		if (!pCurrentWorld) pCurrentWorld = worlds.front();
	}

	// Mise � jour de la sc�ne courante
	void UpdateFrame() const;

private:
	static std::unique_ptr<Engine> pInstance;

	std::vector<std::shared_ptr<World>> worlds;
	std::shared_ptr<World> pCurrentWorld;
};

