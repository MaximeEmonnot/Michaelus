#pragma once
#include <memory>
#include <vector>

// Forward declarations
class CameraComponent;

// Macro pour l'utilisation plus aisée de la classe CameraManager
#define CAMERA CameraManager::GetInstance()

/*
 * Classe CameraManager
 * Permet la gestion des différentes caméras présentes dans la scène (CameraComponent)
 */
class CameraManager
{
public:
	// Constructeur par défaut (publique car std::unique_ptr nécessite un constructeur publique) 
	CameraManager() = default;
	// Destructeur
	~CameraManager();

	// Ces différents constructeurs sont définis comme étant supprimés, pour correspondre au patron de conception Singleton
	// et également à la RO5 des normes du C++
	CameraManager(const CameraManager&) = delete;
	CameraManager& operator= (const CameraManager&) = delete;
	CameraManager(CameraManager&&) = delete;
	CameraManager& operator= (CameraManager&&) = delete;

	// Méthode du patron de conception Singleton
	static CameraManager& GetInstance();

	// Ajout d'une nouvelle caméra
	void AddCamera(std::weak_ptr<CameraComponent> pNewCamera);
	// Définition de la caméra active
	void SetActiveCamera(std::weak_ptr<CameraComponent> pNewActiveCamera);
	// Récupération de la caméra active
	std::shared_ptr<CameraComponent> GetActiveCamera() const;

private:
	static std::unique_ptr<CameraManager> pInstance;

	std::vector<std::weak_ptr<CameraComponent>> cameras;
	std::weak_ptr<CameraComponent> pActiveCamera;
};

