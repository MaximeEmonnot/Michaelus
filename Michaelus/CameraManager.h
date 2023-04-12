#pragma once
#include <memory>
#include <vector>

// Forward declarations
class CameraComponent;

// Macro pour l'utilisation plus ais�e de la classe CameraManager
#define CAMERA CameraManager::GetInstance()

/*
 * Classe CameraManager
 * Permet la gestion des diff�rentes cam�ras pr�sentes dans la sc�ne (CameraComponent)
 */
class CameraManager
{
public:
	// Constructeur par d�faut (publique car std::unique_ptr n�cessite un constructeur publique) 
	CameraManager() = default;
	// Destructeur
	~CameraManager();

	// Ces diff�rents constructeurs sont d�finis comme �tant supprim�s, pour correspondre au patron de conception Singleton
	// et �galement � la RO5 des normes du C++
	CameraManager(const CameraManager&) = delete;
	CameraManager& operator= (const CameraManager&) = delete;
	CameraManager(CameraManager&&) = delete;
	CameraManager& operator= (CameraManager&&) = delete;

	// M�thode du patron de conception Singleton
	static CameraManager& GetInstance();

	// Ajout d'une nouvelle cam�ra
	void AddCamera(std::weak_ptr<CameraComponent> pNewCamera);
	// D�finition de la cam�ra active
	void SetActiveCamera(std::weak_ptr<CameraComponent> pNewActiveCamera);
	// R�cup�ration de la cam�ra active
	std::shared_ptr<CameraComponent> GetActiveCamera() const;

private:
	static std::unique_ptr<CameraManager> pInstance;

	std::vector<std::weak_ptr<CameraComponent>> cameras;
	std::weak_ptr<CameraComponent> pActiveCamera;
};

