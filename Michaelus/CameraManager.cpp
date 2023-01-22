#include "CameraManager.h"
#include "CameraComponent.h"

std::unique_ptr<CameraManager> CameraManager::pInstance = nullptr;

CameraManager::~CameraManager()
{
	cameras.clear();
}

CameraManager& CameraManager::GetInstance()
{
	if (!pInstance)
		pInstance = std::make_unique<CameraManager>();
	return *pInstance;
}

void CameraManager::AddCamera(std::weak_ptr<CameraComponent> pNewCamera)
{
	cameras.push_back(pNewCamera);
}

void CameraManager::SetActiveCamera(std::weak_ptr<CameraComponent> pNewActiveCamera)
{
	pActiveCamera = pNewActiveCamera;
}

std::shared_ptr<CameraComponent> CameraManager::GetActiveCamera() const
{
	return pActiveCamera.lock();
}
