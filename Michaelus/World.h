#pragma once
#include <memory>
#include <vector>

class Actor;

/*
 * Classe World
 * Définit une scène du moteur et permet la gestion des différents acteurs
 */
class World
{
public:
	// Constructeur par défaut
	World() = default;
	// Constructeur de copie
	World(const World& toCopy);
	// Opérateur de copie
	World& operator= (const World& rhs);
	// Constructeur de déplacement
	World(World&& toMove);
	// Opérateur de déplacement
	World& operator= (World&& rhs);
	// Destructeur
	virtual ~World();

	// Méthode virtuelle pure (=abstraite) BeginPlay()
	// Exécutée à chaque chargement de scène
	virtual void BeginPlay() = 0;
	// Méthode virtuelle pure (=abstraite) Update()
	// Exécutée à chaque frame
	virtual void Update() = 0;

	// Génération d'un Actor dans la scène (Factory)
	template<class T>
	std::shared_ptr<T> SpawnActor()
	{
		// Une exception est levée si la classe entre chevrons n'hérite pas de Actor
		static_assert(std::is_base_of_v<Actor, T>);
		std::shared_ptr<T> new_actor = std::make_shared<T>();
		actors.emplace_back(new_actor);
		return new_actor;
	}

	// Génération d'un Actor dans la scène, avec des paramètres de construction (Factory)
	template<class T, class... Args>
	std::shared_ptr<T> SpawnActor(Args&&... args)
	{
		// Une exception est levée si la classe entre chevrons n'hérite pas de Actor
		static_assert(std::is_base_of_v<Actor, T>);
		std::shared_ptr<T> new_actor = std::make_shared<T>(args...);
		actors.emplace_back(new_actor);
		return new_actor;
	}

protected:
	std::vector<std::shared_ptr<Actor>> actors;
};

