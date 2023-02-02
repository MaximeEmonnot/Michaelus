#pragma once
#include <al.h>
#include <alc.h>
#include <map>
#include <sndfile.h>
#include <memory>
#include <string>

#include "Vec3D.h"

#define SFX SoundSystem::GetInstance()

class SoundSystem
{
public:
	SoundSystem();
	~SoundSystem();

	static SoundSystem& GetInstance();

	void PlaySoundAtLocation(const std::string& path, const FVec3D& location);

	void SetListenerLocation(const FVec3D& location);

private:

	ALuint LoadSound(const std::string& path);

private:
	static std::unique_ptr<SoundSystem> pInstance;

	static constexpr size_t nbSources = 64;

	std::vector<ALuint> sources;

	ALCdevice* pAlDevice;
	ALCcontext* pAlContext;

	std::map<std::string, ALuint> soundBuffers;
};

