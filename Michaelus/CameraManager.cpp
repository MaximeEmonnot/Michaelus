#include "CameraManager.h"
#include "CameraComponent.h"

// VARIABLES STATIQUES
std::unique_ptr<CameraManager> CameraManager::pInstance = nullptr;

// ********* //

// Destructeur : Suppression des pointeurs vers les caméras 
CameraManager::~CameraManager()
{
	cameras.clear();
}

// Méthode du patron de conception Singleton
CameraManager& CameraManager::GetInstance()
{
	if (!pInstance)
		pInstance = std::make_unique<CameraManager>();
	return *pInstance;
}

// Ajout d'une nouvelle caméra
void CameraManager::AddCamera(std::weak_ptr<CameraComponent> pNewCamera)
{
	cameras.push_back(pNewCamera);
}

// Définition de la caméra active
void CameraManager::SetActiveCamera(std::weak_ptr<CameraComponent> pNewActiveCamera)
{
	pActiveCamera = pNewActiveCamera;
}

// Récupération de la caméra active
std::shared_ptr<CameraComponent> CameraManager::GetActiveCamera() const
{
	return pActiveCamera.lock();
}
