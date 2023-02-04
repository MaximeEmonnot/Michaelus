#pragma once
#include <memory>
#include <vector>

class DemoLightAndSound;
class DemoShrek;
class DemoPlane;
class DemoPawn;
class DemoRat;
class Actor;

class World
{
public:
	World();
	World(const World& toCopy);
	World& operator= (const World& rhs);
	World(World&& toMove);
	World& operator= (World&& rhs);
	virtual ~World();

	virtual void BeginPlay();
	virtual void Update();

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

private:
	std::vector<std::shared_ptr<Actor>> actors;

	// ACTORS

	std::shared_ptr<DemoRat> pTestActor1;
	std::shared_ptr<DemoRat> pTestActor2;
	std::shared_ptr<DemoRat> pTestActor3;
	std::shared_ptr<DemoRat> pTestActor4;
	std::shared_ptr<DemoRat> pTestActor5;

	std::shared_ptr<DemoPawn> pTestPawn;

	std::shared_ptr<DemoPlane> pDemoPlane;

	std::shared_ptr<DemoShrek> pDemoShrek;

	std::shared_ptr<DemoLightAndSound> pDemoLightSound;
};

