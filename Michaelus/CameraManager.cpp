#include "CameraManager.h"
#include "CameraComponent.h"

// VARIABLES STATIQUES
std::unique_ptr<CameraManager> CameraManager::pInstance = nullptr;

// ********* //

// Destructeur : Suppression des pointeurs vers les cam�ras 
CameraManager::~CameraManager()
{
	cameras.clear();
}

// M�thode du patron de conception Singleton
CameraManager& CameraManager::GetInstance()
{
	if (!pInstance)
		pInstance = std::make_unique<CameraManager>();
	return *pInstance;
}

// Ajout d'une nouvelle cam�ra
void CameraManager::AddCamera(std::weak_ptr<CameraComponent> pNewCamera)
{
	cameras.push_back(pNewCamera);
}

// D�finition de la cam�ra active
void CameraManager::SetActiveCamera(std::weak_ptr<CameraComponent> pNewActiveCamera)
{
	pActiveCamera = pNewActiveCamera;
}

// R�cup�ration de la cam�ra active
std::shared_ptr<CameraComponent> CameraManager::GetActiveCamera() const
{
	return pActiveCamera.lock();
}
