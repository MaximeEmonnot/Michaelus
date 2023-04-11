#include "World.h"

#include "Actor.h"

// Contructeur de copie
World::World(const World& toCopy)
{
    *this = toCopy;
}

// Opérateur de copie
World& World::operator=(const World& rhs)
{
    actors = rhs.actors;
    return *this;
}

// Constructeur de déplacement
World::World(World&& toMove)
{
    *this = toMove;
}

// Opérateur de déplacement
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