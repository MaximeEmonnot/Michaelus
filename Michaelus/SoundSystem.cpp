#include "SoundSystem.h"

#include <vector>

// VARIABLES STATIQUES
std::unique_ptr<SoundSystem> SoundSystem::pInstance = nullptr;

// ********* //

// Constructeur
SoundSystem::SoundSystem()
{
	// On ouvre le Device de OpenAL
	pAlDevice = alcOpenDevice(nullptr);
	if (pAlDevice)
	{
		// Si le Device est bien ouvert, on cr�e le contexte permettant de jouer les diff�rents sons
		pAlContext = alcCreateContext(pAlDevice, nullptr);
		alcMakeContextCurrent(pAlContext);
	}

	// On initialise les diff�rentes sources qui composeront le syst�me OpenAL
	sources.resize(nbSources);
	alGenSources(nbSources, sources.data());
}

// Destructeur
SoundSystem::~SoundSystem()
{
	// On supprime les sources qui composent le syst�me OpenAL
	alDeleteSources(static_cast<ALint>(sources.size()), sources.data());
	for (auto& entry : soundBuffers)
		alDeleteBuffers(1, &entry.second);

	// On d�truit le contexte permettant de jouer les diff�rents sons
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(pAlContext);

	// On ferme le Device OpenAL
	alcCloseDevice(pAlDevice);
}

// Fonction propre au patron de conception Singleton
SoundSystem& SoundSystem::GetInstance()
{
	if (!pInstance)
		pInstance = std::make_unique<SoundSystem>();
	return *pInstance;
}

// Mise � jour d'un son en cours de lecture
void SoundSystem::UpdatePlayingSound(int index, bool bIsLooping, float pitch, float gain, const FVec3D& location,
	const FVec3D& velocity)
{
	// On r�cup�re la source correspondant � l'index
	ALuint source = sources.at(index);

	// Si la source est bien en �tat de lecture, on met � jour les param�tres suivants :
	if (GetSourceStatus(source) == AL_PLAYING)
	{
		alSourcei(source, AL_LOOPING, bIsLooping); // Param�tre de boucle
		alSourcef(source, AL_PITCH, pitch); // Param�tre de hauteur
		alSourcef(source, AL_GAIN, gain); // Param�tre de gain
		alSource3f(source, AL_POSITION, location.GetY(), location.GetX(), location.GetZ()); // Param�tre de position
		alSource3f(source, AL_VELOCITY, velocity.GetY(), velocity.GetX(), velocity.GetZ()); // Param�tre de v�locit�
	}
}

// Mise en pause d'un son
void SoundSystem::PauseSound(int index)
{
	// On r�cup�re la source correspondant � l'index
	ALuint source = sources.at(index);

	// Si la source est bien en �tat de lecture, on met en pause la lecture
	if (GetSourceStatus(source) == AL_PLAYING)
		alSourcePause(source);
}

// Reprise d'un son
void SoundSystem::ResumeSound(int index)
{
	// On r�cup�re la source correspondant � l'index
	ALuint source = sources.at(index);

	// Si la source est bien en �tat de pause, on reprend la lecture
	if (GetSourceStatus(source) == AL_PAUSED) 
		alSourcePlay(source);
}

// Arr�t d'un son
void SoundSystem::StopSound(int index)
{
	// On r�cup�re la source correspondant � l'index
	ALuint source = sources.at(index);

	// Si la source n'est pas en �tat d'arr�t, on arr�te la lecture
	if (GetSourceStatus(source) != AL_STOPPED)
		alSourceStop(source);
}

// D�finition de la position du listener
void SoundSystem::SetListenerLocation(const FVec3D& location)
{
	alListener3f(AL_POSITION, location.GetY(), location.GetX(), location.GetZ());
}

// R�cup�ration du statut d'une source (Etat Initial, En pause, En cours de lecture, A l'arr�t)
ALint SoundSystem::GetSourceStatus(ALuint source)
{
	ALint status;
	alGetSourcei(source, AL_SOURCE_STATE, &status);
	return status;
}
