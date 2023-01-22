#include "ModelFactory.h"

#include "VKModel.h"

std::unique_ptr<ModelFactory> ModelFactory::pInstance = nullptr;

ModelFactory::~ModelFactory()
{
	for (auto& entry : models)
		entry.second.Destroy();
}

ModelFactory& ModelFactory::GetInstance()
{
	if (!pInstance)
		pInstance = std::make_unique<ModelFactory>();
	return *pInstance;
}

VKModel& ModelFactory::GetModel(const std::string& path)
{
	if (!models.contains(path))
		models.emplace(path, VKModel(path));
	return models.at(path);
}
