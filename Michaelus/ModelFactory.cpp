#include "ModelFactory.h"

#include "VKModel.h"

std::unique_ptr<ModelFactory> ModelFactory::pInstance = nullptr;

ModelFactory::~ModelFactory()
{

}

ModelFactory& ModelFactory::GetInstance()
{
	if (!pInstance)
		pInstance = std::make_unique<ModelFactory>();
	return *pInstance;
}

void ModelFactory::Clear()
{
	for (auto& entry : models)
		entry.second->Destroy();
	models.clear();
}

VKModel& ModelFactory::GetModel(const std::string& path)
{
	if (!models.contains(path))
		models.emplace(path, std::make_shared<VKModel>(path));
	return *models.at(path);
}
