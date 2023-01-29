#pragma once
#include <map>
#include <memory>
#include <string>

#include "Material.h"

#define MATERIAL_CLEAR MaterialFactory::GetInstance().Clear()
#define MATERIAL(path, shading_model) MaterialFactory::GetInstance().GetMaterial(path, shading_model)

class MaterialFactory
{
public:

	MaterialFactory() = default;

	static MaterialFactory& GetInstance();

	void Clear();

	Material GetMaterial(const std::string& path, EShadingModel shadingModel);

private:
	static std::unique_ptr<MaterialFactory> pInstance;

	std::map<SMaterialInformation, std::unique_ptr<Material>, SMaterialInformation::SMaterialInformationCompare> materials;
};

