#include "TextureFactory.h"

#include "VKTexture.h"

std::unique_ptr<TextureFactory> TextureFactory::pInstance = nullptr;

TextureFactory::~TextureFactory()
{
	for (auto& entry : textures)
		entry.second.Destroy();
}

TextureFactory& TextureFactory::GetInstance()
{
	if (!pInstance)
		pInstance = std::make_unique<TextureFactory>();
	return *pInstance;
}

VKTexture& TextureFactory::GetTexture(const std::string& path)
{
	if (!textures.contains(path))
		textures.emplace(path, VKTexture(path));
	return textures.at(path);
}
