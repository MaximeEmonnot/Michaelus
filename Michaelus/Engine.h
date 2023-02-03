#pragma once
#include <memory>
#include <vector>

#include "EngineException.h"

#define ENGINE Engine::GetInstance()

class World;

class TestPawn;
class TestActor;

class Engine
{
public:
	Engine();
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;
	Engine(Engine&&) = delete;
	Engine& operator=(Engine&&) = delete;

	static Engine& GetInstance();

	template<class T>
	void SetWorld()
	{
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

	std::shared_ptr<World> GetCurrentWorld() const;

	void Run();

private:
	template<class T>
	void AddWorld()
	{
		static_assert(std::is_base_of_v<World, T>);

		worlds.emplace_back(std::make_shared<T>());
		if (!pCurrentWorld) pCurrentWorld = worlds.front();
	}

	void UpdateFrame() const;

private:
	static std::unique_ptr<Engine> pInstance;

	std::vector<std::shared_ptr<World>> worlds;
	std::shared_ptr<World> pCurrentWorld;
};

