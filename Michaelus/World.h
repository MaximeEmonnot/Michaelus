#pragma once
#include <memory>
#include <vector>

class Actor;

/*
 * Classe World
 * D�finit une sc�ne du moteur et permet la gestion des diff�rents acteurs
 */
class World
{
public:
	// Constructeur par d�faut
	World() = default;
	// Constructeur de copie
	World(const World& toCopy);
	// Op�rateur de copie
	World& operator= (const World& rhs);
	// Constructeur de d�placement
	World(World&& toMove);
	// Op�rateur de d�placement
	World& operator= (World&& rhs);
	// Destructeur
	virtual ~World();

	// M�thode virtuelle pure (=abstraite) BeginPlay()
	// Ex�cut�e � chaque chargement de sc�ne
	virtual void BeginPlay() = 0;
	// M�thode virtuelle pure (=abstraite) Update()
	// Ex�cut�e � chaque frame
	virtual void Update() = 0;

	// G�n�ration d'un Actor dans la sc�ne (Factory)
	template<class T>
	std::shared_ptr<T> SpawnActor()
	{
		// Une exception est lev�e si la classe entre chevrons n'h�rite pas de Actor
		static_assert(std::is_base_of_v<Actor, T>);
		std::shared_ptr<T> new_actor = std::make_shared<T>();
		actors.emplace_back(new_actor);
		return new_actor;
	}

	// G�n�ration d'un Actor dans la sc�ne, avec des param�tres de construction (Factory)
	template<class T, class... Args>
	std::shared_ptr<T> SpawnActor(Args&&... args)
	{
		// Une exception est lev�e si la classe entre chevrons n'h�rite pas de Actor
		static_assert(std::is_base_of_v<Actor, T>);
		std::shared_ptr<T> new_actor = std::make_shared<T>(args...);
		actors.emplace_back(new_actor);
		return new_actor;
	}

protected:
	std::vector<std::shared_ptr<Actor>> actors;
};

