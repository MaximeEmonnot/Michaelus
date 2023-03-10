#pragma once
#include <memory>
#include <vector>

class Actor;

class World
{
public:
	World() = default;
	World(const World& toCopy);
	World& operator= (const World& rhs);
	World(World&& toMove);
	World& operator= (World&& rhs);
	virtual ~World();

	virtual void BeginPlay() = 0;
	virtual void Update() = 0;

	template<class T>
	std::shared_ptr<T> SpawnActor()
	{
		static_assert(std::is_base_of_v<Actor, T>);
		std::shared_ptr<T> new_actor = std::make_shared<T>();
		actors.emplace_back(new_actor);
		return new_actor;
	}

	template<class T, class... Args>
	std::shared_ptr<T> SpawnActor(Args&&... args)
	{
		static_assert(std::is_base_of_v<Actor, T>);
		std::shared_ptr<T> new_actor = std::make_shared<T>(args...);
		actors.emplace_back(new_actor);
		return new_actor;
	}

protected:
	std::vector<std::shared_ptr<Actor>> actors;
};

