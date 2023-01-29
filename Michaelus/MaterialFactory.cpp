#include "MaterialFactory.h"

#include "Material.h"

std::unique_ptr<MaterialFactory> MaterialFactory::pInstance = nullptr;

MaterialFactory& MaterialFactory::GetInstance()
{
    if (!pInstance)
        pInstance = std::make_unique<MaterialFactory>();
    return *pInstance;
}

void MaterialFactory::Clear()
{
    for (auto& material : materials)
        material.second->Clear();
    materials.clear();
}

Material MaterialFactory::GetMaterial(const std::string& path, EShadingModel shadingModel)
{
    SMaterialInformation key = { path, shadingModel };
    if (!materials.contains(key))
        materials.emplace(key, std::make_unique<Material>(path, shadingModel));
	return Material(*materials.at(key).get());
}
