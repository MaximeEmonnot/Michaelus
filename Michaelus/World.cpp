#include "World.h"

#include "Actor.h"
#include "LightManager.h"
#include "TestActor.h"
#include "TestPawn.h"

World::World()
{
    pTestActor1 = SpawnActor<TestActor>("Test Actor 1");
    pTestActor2 = SpawnActor<TestActor>("Test Actor 1");
    pTestActor3 = SpawnActor<TestActor>("Test Actor 1");
    pTestActor4 = SpawnActor<TestActor>("Test Actor 1");
    pTestActor5 = SpawnActor<TestActor>("Test Actor 1");

    pTestPawn = SpawnActor<TestPawn>("Test Pawn");

    LIGHT_SYSTEM.SetDirectionalLight({ 1.f, 0.f, 1.f }, { 1.f, 1.f, 1.f }, 1.f);
}

World::World(const World& toCopy)
{
    *this = toCopy;
}

World& World::operator=(const World& rhs)
{
    actors = rhs.actors;
    return *this;
}

World::World(World&& toMove)
{
    *this = toMove;
}

World& World::operator=(World&& rhs)
{
    actors = std::move(rhs.actors);
    return *this;
}

World::~World()
{
    actors.clear();
}

void World::BeginPlay()
{
}

void World::Update()
{
    for (auto& actor : actors) actor->Update();
}
