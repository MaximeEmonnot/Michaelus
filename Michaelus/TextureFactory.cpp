#include "TextureFactory.h"

#include "VKTexture.h"

std::unique_ptr<TextureFactory> TextureFactory::pInstance = nullptr;

TextureFactory::~TextureFactory()
{

}

TextureFactory& TextureFactory::GetInstance()
{
	if (!pInstance)
		pInstance = std::make_unique<TextureFactory>();
	return *pInstance;
}

void TextureFactory::Clear()
{
	for (auto& entry : textures)
		entry.second->Destroy();
	textures.clear();
}

VKTexture& TextureFactory::GetTexture(const std::string& path)
{
	if (!textures.contains(path))
		textures.emplace(path, std::make_shared<VKTexture>(path));
	return *textures.at(path);
}
