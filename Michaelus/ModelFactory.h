#pragma once
#include <map>
#include <memory>
#include <string>

class VKModel;

#define MODEL_CLEAR ModelFactory::GetInstance().Clear()
#define MODEL(path) ModelFactory::GetInstance().GetModel(path)

class ModelFactory
{
public:
	ModelFactory() = default;
	~ModelFactory();

	static ModelFactory& GetInstance();

	void Clear();

	VKModel& GetModel(const std::string& path);

private:
	static std::unique_ptr<ModelFactory> pInstance;

	std::map<std::string, std::shared_ptr<VKModel>> models;
};

