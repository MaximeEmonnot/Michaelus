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
 * D�finit un objet dans la sc�ne. Poss�de 1 � plusieurs Components
 */
class Actor
{
public:
	// Constructeur par d�faut
	Actor() = default;
	// Constructeur d�finissant le nom de l'acteur
	Actor(const std::string& name);

	// M�thode Update() de l'acteur
	virtual void Update();

	// Cr�ation d'un Component
	template <class T>
	std::shared_ptr<T> CreateComponent()
	{
		// Une exception est lev�e si la classe entre chevrons n'h�rite pas de Component
		if (!std::is_base_of<Component, T>())
			throw ENGINE_EXCEPTION("Vulkan 3D Engine - Main Engine Exception", "This is not a Component Class. Please check your call for CreateComponent.");

		// On cr�e le nouveau Component, qu'on lie � l'Actor
		std::shared_ptr<Component> newComponent = std::make_shared<T>(*this);
		components.emplace_back(newComponent);

		// Si ce nouveau Component est un CameraComponent, on le lie au CameraManager
		if (auto newCamera = std::dynamic_pointer_cast<CameraComponent>(newComponent))
			CAMERA.AddCamera(newCamera);
		// Si ce nouveau Component est un MeshComponent, on le lie au syst�me graphique
		if (auto newMesh = std::dynamic_pointer_cast<MeshComponent>(newComponent))
			GFX.AddMesh(newMesh);

		return std::dynamic_pointer_cast<T>(newComponent);
	}

	// R�cup�ration du nom de l'acteur
	std::string GetName() const;
	// R�cup�ration du Component racine
	std::shared_ptr<Component> GetRootComponent() const;

	// R�cup�ration du premier Component de la classe d�finie entre chevrons
	template <class T>
	std::shared_ptr<T> GetComponentByClass() const
	{
		// Une exception est lev�e si la classe entre chevrons n'h�rite pas de Component
		if (!std::is_base_of<Component, T>())
			throw ENGINE_EXCEPTION("Vulkan 3D Engine - Main Engine Exception", "This is not a Component Class. Please check your call for GetComponentByClass.");

		for (std::shared_ptr<Component> component : components)
			if (std::shared_ptr<T> comp = std::dynamic_pointer_cast<T>(component)) return comp;

		return nullptr;
	}
	// R�cup�ration de tous les Component de la classe d�finie entre chevrons
	template <class T>
	std::vector<std::shared_ptr<T>> GetComponentsByClass() const
	{
		// Une exception est lev�e si la classe entre chevrons n'h�rite pas de Component
		if (!std::is_base_of<Component, T>())
			throw ENGINE_EXCEPTION("Vulkan 3D Engine - Main Engine Exception", "This is not a Component Class. Please check your call for GetComponentsByClass.");

		std::vector<std::shared_ptr<T>> out;

		for (std::shared_ptr<Component> component : components)
			if (std::shared_ptr<T> comp = std::dynamic_pointer_cast<T>(component)) out.emplace_back(comp);

		return out;
	}

	// D�finition de la position de l'Actor
	void SetActorLocation(const FVec3D& newLocation);
	// D�finition de la rotation de l'Actor
	void SetActorRotation(const FQuaternion& newRotation);
	// Ajout d'un d�calage � la position de l'Actor
	void AddActorLocation(const FVec3D& offsetLocation);
	// Ajout d'une rotation � la rotation de l'Actor
	void AddActorRotation(const FQuaternion& offsetRotation);
	// R�cup�ration de la position de l'Actor
	FVec3D GetActorLocation() const;
	// R�cup�ration de la rotation de l'Actor
	FQuaternion GetActorRotation() const;
	// R�cup�ration du Transform de l'Actor
	FTransform GetActorTransform() const;

	// R�cup�ration du vecteur avant de l'Actor (Axe X relatif � l'Actor)
	FVec3D GetActorForwardVector() const;
	// R�cup�ration du vecteur droite de l'Actor (Axe Y relatif � l'Actor)
	FVec3D GetActorRightVector() const;
	// R�cup�ration du vecteur haut de l'Actor (Axe Z relatif � l'Actor)
	FVec3D GetActorUpVector() const;

protected:
	std::shared_ptr<Component> pRootComponent;
private:
	std::string name;
	std::vector<std::shared_ptr<Component>> components;
	FTransform transform;
};

