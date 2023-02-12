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
	alDeleteSources(static_cast<ALint>(sources.size()), sources.data());
	for (auto& entry : soundBuffers)
		alDeleteBuffers(1, &entry.second);

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

void SoundSystem::UpdatePlayingSound(int index, bool bIsLooping, float pitch, float gain, const FVec3D& location,
	const FVec3D& velocity)
{
	ALuint source = sources.at(index);

	if (GetSourceStatus(source) == AL_PLAYING)
	{
		alSourcei(source, AL_LOOPING, bIsLooping);
		alSourcef(source, AL_PITCH, pitch);
		alSourcef(source, AL_GAIN, gain);
		alSource3f(source, AL_POSITION, location.y, location.x, location.z);
		alSource3f(source, AL_VELOCITY, velocity.y, velocity.x, velocity.z);
	}
}

void SoundSystem::PauseSound(int index)
{
	ALuint source = sources.at(index);

	if (GetSourceStatus(source) == AL_PLAYING)
		alSourcePause(source);
}

void SoundSystem::ResumeSound(int index)
{
	ALuint source = sources.at(index);

	if (GetSourceStatus(source) == AL_PAUSED) 
		alSourcePlay(source);
}

void SoundSystem::StopSound(int index)
{
	ALuint source = sources.at(index);

	if (GetSourceStatus(source) != AL_STOPPED)
		alSourceStop(source);
}

void SoundSystem::SetListenerLocation(const FVec3D& location)
{
	alListener3f(AL_POSITION, location.y, location.x, location.z);
}

ALint SoundSystem::GetSourceStatus(ALuint source)
{
	ALint status;
	alGetSourcei(source, AL_SOURCE_STATE, &status);
	return status;
}
