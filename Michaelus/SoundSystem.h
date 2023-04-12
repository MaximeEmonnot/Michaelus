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

// Macro pour l'utilisation plus ais�e de la classe SoundSystem
#define SFX SoundSystem::GetInstance()

/*
 * Classe d'un son, jou� par la biblioth�que OpenAL
 * Permet de r�cup�rer les donn�es d'un fichier .WAV
 */
class Sound
{
protected:
	/*
	 * Donn�es d'un fichier .WAV
	 * D�finit le nombre de samples, de taux de sampling, et les samples du fichier
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

		// On d�finit le nombre de sample et le taux de sampling
		out.nbSamples = static_cast<ALsizei>(fileInfos.channels * fileInfos.frames);
		out.sampleRate = static_cast<ALsizei>(fileInfos.samplerate);

		// On d�finit les samples
		out.samples.resize(out.nbSamples);
		if (sf_read_short(file, out.samples.data(), out.nbSamples) < out.nbSamples) throw 0;

		// On ferme le fichier
		sf_close(file);

		return out;
	}
};

/*
 * Classe Effect, d�finissant un son pouvant �tre spatialis�
 * Attention toute fois : OpenAL ne g�re la spatialisation qu'avec des sons MONO
 */
class Effect : private Sound{
public:
	// Chargement du son : On d�finit les donn�es du buffer audio via les donn�es du fichier .WAV, puis on retourne ce buffer qui sera engistr� plus tard
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
 * Classe Music, d�finissant un son pouvant �tre jou� en boucle
 * Ce son est STEREO, et ne peut pas �tre spatialis�
 */
class Music : private Sound
{
public:
	// Chargement du son : On d�finit les donn�es du buffer audio via les donn�es du fichier .WAV, puis on retourne ce buffer qui sera engistr� plus tard
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
 * Cl� de la map de sons
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
 * Syst�me audio du moteur g�r� par OpenAL
 * OpenAL permet la gestion d'un unique listener (l'utilisateur de l'application) et de plusieurs sources audio (les sons que l'on jouera)
 */
class SoundSystem
{
public:
	// Constructeur (public car std::unique_ptr n�cessite un constructeur publique) 
	SoundSystem();
	// Destructeur
	~SoundSystem();

	// Ces diff�rents constructeurs sont d�finis comme �tant supprim�s, pour correspondre au patron de conception Singleton
	// et �galement � la RO5 des normes du C++
	SoundSystem(const SoundSystem&) = delete;
	SoundSystem& operator=(const SoundSystem&) = delete;
	SoundSystem(SoundSystem&&) = delete;
	SoundSystem& operator=(SoundSystem&&) = delete;

	// M�thode du patron de conception Singleton
	static SoundSystem& GetInstance();

	/*
	 * Fonction permettant de jouer un son
	 * Prends comme param�tre le chemin du son, si l'on le joue en boucle, sa hauteur, son gain
	 * sa position, et sa v�locit�. Ces autres param�tres sont facultatifs.
	 */
	template<class SoundType>
	int PlaySound(const std::string& path, bool bIsLooping = false, float pitch = 1.f, float gain = 1.f, 
		const FVec3D& location = CAMERA.GetActiveCamera()->GetWorldLocation(), const FVec3D& velocity = FVec3D())
	{
		// On ouvre (ou r�cup�re) le son li� au path
		SoundKey key = { 0, path };
		if (!soundBuffers.contains(key)) soundBuffers.emplace(key, SoundType::LoadSound(path));

		// On parcourt donc l'ensemble des sources du syst�me...
		for (size_t i = 0; i < sources.size(); i++)
		{
			ALuint source = sources.at(i);
			// ...et on v�rifie si la source est soit en �tat initial soit arr�t�e...
			switch(GetSourceStatus(source))
			{
			case AL_INITIAL:
			case AL_STOPPED:
				{
				// ...puis on d�finit tous les param�tres (Boucle, Hauteur, Gain, Position, Velocit�, Source s�lectionn�e), avant de lancer le son
				// et de retourner l'index de la source pour l'alt�rer plus tard
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

	// Mise � jour d'un son en cours de lecture
	void UpdatePlayingSound(int index, bool bIsLooping = false, float pitch = 1.f, float gain = 1.f,
		const FVec3D& location = CAMERA.GetActiveCamera()->GetWorldLocation(), const FVec3D& velocity = FVec3D());

	// Mise en pause d'un son
	void PauseSound(int index);
	// Reprise d'un son
	void ResumeSound(int index);
	// Arr�t d'un son
	void StopSound(int index);

	// D�finition de la position du listener
	void SetListenerLocation(const FVec3D& location);

private:
	// R�cup�ration du statut d'une source (Etat Initial, En pause, En cours de lecture, A l'arr�t)
	static ALint GetSourceStatus(ALuint source);

private:
	static std::unique_ptr<SoundSystem> pInstance;

	static constexpr size_t nbSources = 64;

	std::vector<ALuint> sources;

	ALCdevice* pAlDevice;
	ALCcontext* pAlContext;

	std::map<SoundKey, ALuint, SoundKey::Comparator> soundBuffers;
};

