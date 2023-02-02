#include "SoundSystem.h"

#include <vector>

std::unique_ptr<SoundSystem> SoundSystem::pInstance = nullptr;

SoundSystem::SoundSystem()
{
	pAlDevice = alcOpenDevice(nullptr);
	if (pAlDevice)
	{
		pAlContext = alcCreateContext(pAlDevice, nullptr);
		alcMakeContextCurrent(pAlContext);
	}

	sources.resize(nbSources);
	alGenSources(nbSources, sources.data());
}

SoundSystem::~SoundSystem()
{
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(pAlContext);
	alcCloseDevice(pAlDevice);
}

SoundSystem& SoundSystem::GetInstance()
{
	if (!pInstance)
		pInstance = std::make_unique<SoundSystem>();
	return *pInstance;
}

void SoundSystem::PlaySoundAtLocation(const std::string& path, const FVec3D& location)
{
	if (!soundBuffers.contains(path))
		soundBuffers.emplace(path, LoadSound(path));

	for (ALuint& source : sources)
	{
		ALint status;
		alGetSourcei(source, AL_SOURCE_STATE, &status);
		if (status != AL_PLAYING)
		{
			alSource3f(source, AL_POSITION, location.x, location.y, location.z);
			alSourcei(source, AL_BUFFER, soundBuffers.at(path));
			alSourcePlay(source);
			return;
		}
	}

	
}

void SoundSystem::SetListenerLocation(const FVec3D& location)
{
	alListener3f(AL_POSITION, location.x, location.y, location.z);
}

ALuint SoundSystem::LoadSound(const std::string& path)
{
	SF_INFO fileInfos;
	SNDFILE* file = sf_open(path.c_str(), SFM_READ, &fileInfos);
	if (!file) return 0;

	ALsizei nbSamples = static_cast<ALsizei>(fileInfos.channels * fileInfos.frames);
	ALsizei sampleRate = static_cast<ALsizei>(fileInfos.samplerate);

	std::vector<ALshort> samples(nbSamples);
	if (sf_read_short(file, &samples[0], nbSamples) < nbSamples) return 0;

	sf_close(file);

	ALenum format;

	switch(fileInfos.channels)
	{
	case 1: format = AL_FORMAT_MONO16; break;
	case 2: format = AL_FORMAT_STEREO16; break;
	default: return 0;
	}

	ALuint buffer;
	alGenBuffers(1, &buffer);

	alBufferData(buffer, format, &samples[0], nbSamples * sizeof(ALushort), sampleRate);

	if (alGetError() != AL_NO_ERROR) return 0;

	return buffer;
}
