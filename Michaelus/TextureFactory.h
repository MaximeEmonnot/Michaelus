#pragma once
#include <map>
#include <memory>
#include <string>

class VKTexture;

#define TEXTURE_CLEAR TextureFactory::GetInstance().Clear()
#define TEXTURE(path) TextureFactory::GetInstance().GetTexture(path)

class TextureFactory
{
public:
	TextureFactory() = default;
	~TextureFactory();

	static TextureFactory& GetInstance();

	void Clear();

	VKTexture& GetTexture(const std::string& path);

private:
	static std::unique_ptr<TextureFactory> pInstance;

	std::map<std::string, std::shared_ptr<VKTexture>> textures;
};

