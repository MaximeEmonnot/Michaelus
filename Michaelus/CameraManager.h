#pragma once
#include <memory>
#include <vector>

class CameraComponent;

#define CAMERA CameraManager::GetInstance()

class CameraManager
{
public:
	CameraManager() = default;
	CameraManager(const CameraManager&) = delete;
	CameraManager& operator= (const CameraManager&) = delete;
	CameraManager(CameraManager&&) = delete;
	CameraManager& operator= (CameraManager&&) = delete;

	~CameraManager();

	static CameraManager& GetInstance();

	void AddCamera(std::weak_ptr<CameraComponent> pNewCamera);
	void SetActiveCamera(std::weak_ptr<CameraComponent> pNewActiveCamera);
	std::shared_ptr<CameraComponent> GetActiveCamera() const;

private:
	static std::unique_ptr<CameraManager> pInstance;

	std::vector<std::weak_ptr<CameraComponent>> cameras;
	std::weak_ptr<CameraComponent> pActiveCamera;
};

