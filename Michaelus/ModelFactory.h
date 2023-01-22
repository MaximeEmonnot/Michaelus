#pragma once
#include <map>
#include <memory>
#include <string>

class VKModel;

#define MODEL(path) ModelFactory::GetInstance().GetModel(path)

class ModelFactory
{
public:
	ModelFactory() = default;
	~ModelFactory();

	static ModelFactory& GetInstance();

	VKModel& GetModel(const std::string& path);

private:
	static std::unique_ptr<ModelFactory> pInstance;

	std::map<std::string, VKModel> models;
};

