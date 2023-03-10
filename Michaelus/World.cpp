#include "World.h"

#include "Actor.h"

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