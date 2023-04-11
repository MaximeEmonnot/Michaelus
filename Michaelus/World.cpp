#include "World.h"

#include "Actor.h"

// Contructeur de copie
World::World(const World& toCopy)
{
    *this = toCopy;
}

// Op�rateur de copie
World& World::operator=(const World& rhs)
{
    actors = rhs.actors;
    return *this;
}

// Constructeur de d�placement
World::World(World&& toMove)
{
    *this = toMove;
}

// Op�rateur de d�placement
World& World::operator=(World&& rhs)
{
    actors = std::move(rhs.actors);
    return *this;
}

// Destructeur
World::~World()
{
    actors.clear();
}