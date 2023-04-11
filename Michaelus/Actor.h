#pragma once
#include <memory>
#include <string>
#include <vector>

#include "CameraComponent.h"
#include "CameraManager.h"
#include "EngineException.h"
#include "Graphics.h"
#include "Transform.h"
#include "MeshComponent.h"

class Component;

/*
 * Classe Actor
 * Définit un objet dans la scène. Possède 1 à plusieurs Components
 */
class Actor
{
public:
	// Constructeur par défaut
	Actor() = default;
	// Constructeur définissant le nom de l'acteur
	Actor(const std::string& name);

	// Méthode Update() de l'acteur
	virtual void Update();

	// Création d'un Component
	template <class T>
	std::shared_ptr<T> CreateComponent()
	{
		// Une exception est levée si la classe entre chevrons n'hérite pas de Component
		if (!std::is_base_of<Component, T>())
			throw ENGINE_EXCEPTION("Vulkan 3D Engine - Main Engine Exception", "This is not a Component Class. Please check your call for CreateComponent.");

		// On crée le nouveau Component, qu'on lie à l'Actor
		std::shared_ptr<Component> newComponent = std::make_shared<T>(*this);
		components.emplace_back(newComponent);

		// Si ce nouveau Component est un CameraComponent, on le lie au CameraManager
		if (auto newCamera = std::dynamic_pointer_cast<CameraComponent>(newComponent))
			CAMERA.AddCamera(newCamera);
		// Si ce nouveau Component est un MeshComponent, on le lie au système graphique
		if (auto newMesh = std::dynamic_pointer_cast<MeshComponent>(newComponent))
			GFX.AddMesh(newMesh);

		return std::dynamic_pointer_cast<T>(newComponent);
	}

	// Récupération du nom de l'acteur
	std::string GetName() const;
	// Récupération du Component racine
	std::shared_ptr<Component> GetRootComponent() const;

	// Récupération du premier Component de la classe définie entre chevrons
	template <class T>
	std::shared_ptr<T> GetComponentByClass() const
	{
		// Une exception est levée si la classe entre chevrons n'hérite pas de Component
		if (!std::is_base_of<Component, T>())
			throw ENGINE_EXCEPTION("Vulkan 3D Engine - Main Engine Exception", "This is not a Component Class. Please check your call for GetComponentByClass.");

		for (std::shared_ptr<Component> component : components)
			if (std::shared_ptr<T> comp = std::dynamic_pointer_cast<T>(component)) return comp;

		return nullptr;
	}
	// Récupération de tous les Component de la classe définie entre chevrons
	template <class T>
	std::vector<std::shared_ptr<T>> GetComponentsByClass() const
	{
		// Une exception est levée si la classe entre chevrons n'hérite pas de Component
		if (!std::is_base_of<Component, T>())
			throw ENGINE_EXCEPTION("Vulkan 3D Engine - Main Engine Exception", "This is not a Component Class. Please check your call for GetComponentsByClass.");

		std::vector<std::shared_ptr<T>> out;

		for (std::shared_ptr<Component> component : components)
			if (std::shared_ptr<T> comp = std::dynamic_pointer_cast<T>(component)) out.emplace_back(comp);

		return out;
	}

	// Définition de la position de l'Actor
	void SetActorLocation(const FVec3D& newLocation);
	// Définition de la rotation de l'Actor
	void SetActorRotation(const FQuaternion& newRotation);
	// Ajout d'un décalage à la position de l'Actor
	void AddActorLocation(const FVec3D& offsetLocation);
	// Ajout d'une rotation à la rotation de l'Actor
	void AddActorRotation(const FQuaternion& offsetRotation);
	// Récupération de la position de l'Actor
	FVec3D GetActorLocation() const;
	// Récupération de la rotation de l'Actor
	FQuaternion GetActorRotation() const;
	// Récupération du Transform de l'Actor
	FTransform GetActorTransform() const;

	// Récupération du vecteur avant de l'Actor (Axe X relatif à l'Actor)
	FVec3D GetActorForwardVector() const;
	// Récupération du vecteur droite de l'Actor (Axe Y relatif à l'Actor)
	FVec3D GetActorRightVector() const;
	// Récupération du vecteur haut de l'Actor (Axe Z relatif à l'Actor)
	FVec3D GetActorUpVector() const;

protected:
	std::shared_ptr<Component> pRootComponent;
private:
	std::string name;
	std::vector<std::shared_ptr<Component>> components;
	FTransform transform;
};

