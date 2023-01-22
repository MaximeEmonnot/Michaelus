#pragma once
#include <map>
#include <memory>
#include <string>

class VKTexture;

#define TEXTURE(path) TextureFactory::GetInstance().GetTexture(path)

class TextureFactory
{
public:
	TextureFactory() = default;
	~TextureFactory();

	static TextureFactory& GetInstance();

	VKTexture& GetTexture(const std::string& path);

private:
	static std::unique_ptr<TextureFactory> pInstance;

	std::map<std::string, VKTexture> textures;
};

