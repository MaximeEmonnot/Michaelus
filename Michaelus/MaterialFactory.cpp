#include "MaterialFactory.h"

#include "Material.h"
#include "VKPipeLine.h"
#include "VKDescriptor.h"
#include "VKUniformBuffer.h"

// VARIABLES STATIQUES
std::unique_ptr<MaterialFactory> MaterialFactory::pInstance = nullptr;

// ********* //

// M�thode du patron de conception Singleton
MaterialFactory& MaterialFactory::GetInstance()
{
    if (!pInstance)
        pInstance = std::make_unique<MaterialFactory>();
    return *pInstance;
}

// Destructeur r�el pour contr�ler la lib�ration de m�moire
void MaterialFactory::Clear()
{
    for (auto& material : materials)
        material->Clear();
    materials.clear();
}

// Cr�ation du mat�riau selon un path et un mod�le de rendu
std::shared_ptr<Material> MaterialFactory::GetMaterial(const std::string& path, EShadingModel shadingModel)
{
    std::shared_ptr<Material> newMaterial = std::make_unique<Material>(path, shadingModel);
    materials.push_back(newMaterial);

    return newMaterial;
}
