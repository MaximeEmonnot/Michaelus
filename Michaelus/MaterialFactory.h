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

	std::shared_ptr<Material> GetMaterial(const std::string& path, EShadingModel shadingModel);

private:
	static std::unique_ptr<MaterialFactory> pInstance;

	std::vector<std::shared_ptr<Material>> materials;
};

