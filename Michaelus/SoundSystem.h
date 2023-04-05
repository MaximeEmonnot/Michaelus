#pragma once
#include <al.h>
#include <alc.h>
#include <map>
#include <sndfile.h>
#include <memory>
#include <string>

#include "CameraComponent.h"
#include "CameraManager.h"
#include "Vec3D.h"

#define SFX SoundSystem::GetInstance()


class Sound
{
protected:
	struct SoundData
	{
		ALsizei nbSamples;
		ALsizei sampleRate;
		std::vector<ALshort> samples;
	};

	static SoundData GetSoundData(const std::string& path)
	{
		SoundData out;
		SF_INFO fileInfos;
		SNDFILE* file = sf_open(path.c_str(), SFM_READ, &fileInfos);
		if (!file) throw 0;

		out.nbSamples = static_cast<ALsizei>(fileInfos.channels * fileInfos.frames);
		out.sampleRate = static_cast<ALsizei>(fileInfos.samplerate);

		out.samples.resize(out.nbSamples);
		if (sf_read_short(file, out.samples.data(), out.nbSamples) < out.nbSamples) throw 0;

		sf_close(file);

		return out;
	}
};

class Effect : private Sound{
public:
	static ALuint LoadSound(const std::string& path)
	{
		SoundData data = GetSoundData(path);

		ALuint buffer;
		alGenBuffers(1, &buffer);

		alBufferData(buffer, AL_FORMAT_MONO16, data.samples.data(), data.nbSamples * sizeof(ALushort), data.sampleRate * 2);

		if (alGetError() != AL_NO_ERROR) return 0;

		return buffer;
	}
};

class Music : private Sound
{
public:
	static ALuint LoadSound(const std::string& path)
	{
		SoundData data = GetSoundData(path);

		ALuint buffer;
		alGenBuffers(1, &buffer);

		alBufferData(buffer, AL_FORMAT_STEREO16, data.samples.data(), data.nbSamples * sizeof(ALushort), data.sampleRate);

		if (alGetError() != AL_NO_ERROR) return 0;

		return buffer;
	}
};

struct SoundKey
{
	class Comparator
	{
	public:
		bool operator() (const SoundKey& lhs, const SoundKey& rhs) const
		{
			return lhs.id < rhs.id && lhs.path < rhs.path;
		}
	};

	int id;
	std::string path;
};

class SoundSystem
{
public:
	SoundSystem();
	~SoundSystem();

	static SoundSystem& GetInstance();

	template<class SoundType>
	int PlaySound(const std::string& path, bool bIsLooping = false, float pitch = 1.f, float gain = 1.f, 
		const FVec3D& location = CAMERA.GetActiveCamera()->GetWorldLocation(), const FVec3D& velocity = FVec3D())
	{
		SoundKey key = { 0, path };
		if (!soundBuffers.contains(key)) soundBuffers.emplace(key, SoundType::LoadSound(path));

		for (size_t i = 0; i < sources.size(); i++)
		{
			ALuint source = sources.at(i);

			switch(GetSourceStatus(source))
			{
			case AL_INITIAL:
			case AL_STOPPED:
				{
				alSourcei(source, AL_LOOPING, bIsLooping);
				alSourcef(source, AL_PITCH, pitch);
				alSourcef(source, AL_GAIN, gain);
				alSource3f(source, AL_POSITION, location.GetY(), location.GetX(), location.GetZ());
				alSource3f(source, AL_VELOCITY, velocity.GetY(), velocity.GetX(), velocity.GetZ());
				alSourcei(source, AL_BUFFER, soundBuffers.at(key));
				alSourcePlay(source);
				return static_cast<int>(i);
				}
			default:
				break;
			}
		}
		return -1;
	}

	void UpdatePlayingSound(int index, bool bIsLooping = false, float pitch = 1.f, float gain = 1.f,
		const FVec3D& location = CAMERA.GetActiveCamera()->GetWorldLocation(), const FVec3D& velocity = FVec3D());

	void PauseSound(int index);
	void ResumeSound(int index);
	void StopSound(int index);

	void SetListenerLocation(const FVec3D& location);

private:
	static ALint GetSourceStatus(ALuint source);

private:
	static std::unique_ptr<SoundSystem> pInstance;

	static constexpr size_t nbSources = 64;

	std::vector<ALuint> sources;

	ALCdevice* pAlDevice;
	ALCcontext* pAlContext;

	std::map<SoundKey, ALuint, SoundKey::Comparator> soundBuffers;
};

