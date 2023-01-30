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
        material->Clear();
    materials.clear();
}

std::shared_ptr<Material> MaterialFactory::GetMaterial(const std::string& path, EShadingModel shadingModel)
{
    std::shared_ptr<Material> newMaterial = std::make_unique<Material>(path, shadingModel);
    materials.push_back(newMaterial);

    return newMaterial;
}
