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

// Macro pour l'utilisation plus aisée de la classe SoundSystem
#define SFX SoundSystem::GetInstance()

/*
 * Classe d'un son, joué par la bibliothèque OpenAL
 * Permet de récupérer les données d'un fichier .WAV
 */
class Sound
{
protected:
	/*
	 * Données d'un fichier .WAV
	 * Définit le nombre de samples, de taux de sampling, et les samples du fichier
	 */
	struct SoundData
	{
		ALsizei nbSamples;
		ALsizei sampleRate;
		std::vector<ALshort> samples;
	};

	static SoundData GetSoundData(const std::string& path)
	{
		// On ouvre le fichier .wav
		SoundData out;
		SF_INFO fileInfos;
		SNDFILE* file = sf_open(path.c_str(), SFM_READ, &fileInfos);
		if (!file) throw 0;

		// On définit le nombre de sample et le taux de sampling
		out.nbSamples = static_cast<ALsizei>(fileInfos.channels * fileInfos.frames);
		out.sampleRate = static_cast<ALsizei>(fileInfos.samplerate);

		// On définit les samples
		out.samples.resize(out.nbSamples);
		if (sf_read_short(file, out.samples.data(), out.nbSamples) < out.nbSamples) throw 0;

		// On ferme le fichier
		sf_close(file);

		return out;
	}
};

/*
 * Classe Effect, définissant un son pouvant être spatialisé
 * Attention toute fois : OpenAL ne gère la spatialisation qu'avec des sons MONO
 */
class Effect : private Sound{
public:
	// Chargement du son : On définit les données du buffer audio via les données du fichier .WAV, puis on retourne ce buffer qui sera engistré plus tard
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

/*
 * Classe Music, définissant un son pouvant être joué en boucle
 * Ce son est STEREO, et ne peut pas être spatialisé
 */
class Music : private Sound
{
public:
	// Chargement du son : On définit les données du buffer audio via les données du fichier .WAV, puis on retourne ce buffer qui sera engistré plus tard
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

/*
 * Structure SoundKey
 * Clé de la map de sons
 */
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

/*
 * Système audio du moteur géré par OpenAL
 * OpenAL permet la gestion d'un unique listener (l'utilisateur de l'application) et de plusieurs sources audio (les sons que l'on jouera)
 */
class SoundSystem
{
public:
	// Constructeur (public car std::unique_ptr nécessite un constructeur publique) 
	SoundSystem();
	// Destructeur
	~SoundSystem();

	// Ces différents constructeurs sont définis comme étant supprimés, pour correspondre au patron de conception Singleton
	// et également à la RO5 des normes du C++
	SoundSystem(const SoundSystem&) = delete;
	SoundSystem& operator=(const SoundSystem&) = delete;
	SoundSystem(SoundSystem&&) = delete;
	SoundSystem& operator=(SoundSystem&&) = delete;

	// Méthode du patron de conception Singleton
	static SoundSystem& GetInstance();

	/*
	 * Fonction permettant de jouer un son
	 * Prends comme paramètre le chemin du son, si l'on le joue en boucle, sa hauteur, son gain
	 * sa position, et sa vélocité. Ces autres paramètres sont facultatifs.
	 */
	template<class SoundType>
	int PlaySound(const std::string& path, bool bIsLooping = false, float pitch = 1.f, float gain = 1.f, 
		const FVec3D& location = CAMERA.GetActiveCamera()->GetWorldLocation(), const FVec3D& velocity = FVec3D())
	{
		// On ouvre (ou récupère) le son lié au path
		SoundKey key = { 0, path };
		if (!soundBuffers.contains(key)) soundBuffers.emplace(key, SoundType::LoadSound(path));

		// On parcourt donc l'ensemble des sources du système...
		for (size_t i = 0; i < sources.size(); i++)
		{
			ALuint source = sources.at(i);
			// ...et on vérifie si la source est soit en état initial soit arrêtée...
			switch(GetSourceStatus(source))
			{
			case AL_INITIAL:
			case AL_STOPPED:
				{
				// ...puis on définit tous les paramètres (Boucle, Hauteur, Gain, Position, Velocité, Source sélectionnée), avant de lancer le son
				// et de retourner l'index de la source pour l'altérer plus tard
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
		// Si aucune sourcen n'est disponible, on renvoie -1
		return -1;
	}

	// Mise à jour d'un son en cours de lecture
	void UpdatePlayingSound(int index, bool bIsLooping = false, float pitch = 1.f, float gain = 1.f,
		const FVec3D& location = CAMERA.GetActiveCamera()->GetWorldLocation(), const FVec3D& velocity = FVec3D());

	// Mise en pause d'un son
	void PauseSound(int index);
	// Reprise d'un son
	void ResumeSound(int index);
	// Arrêt d'un son
	void StopSound(int index);

	// Définition de la position du listener
	void SetListenerLocation(const FVec3D& location);

private:
	// Récupération du statut d'une source (Etat Initial, En pause, En cours de lecture, A l'arrêt)
	static ALint GetSourceStatus(ALuint source);

private:
	static std::unique_ptr<SoundSystem> pInstance;

	static constexpr size_t nbSources = 64;

	std::vector<ALuint> sources;

	ALCdevice* pAlDevice;
	ALCcontext* pAlContext;

	std::map<SoundKey, ALuint, SoundKey::Comparator> soundBuffers;
};

