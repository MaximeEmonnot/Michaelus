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
		// Si le Device est bien ouvert, on crée le contexte permettant de jouer les différents sons
		pAlContext = alcCreateContext(pAlDevice, nullptr);
		alcMakeContextCurrent(pAlContext);
	}

	// On initialise les différentes sources qui composeront le système OpenAL
	sources.resize(nbSources);
	alGenSources(nbSources, sources.data());
}

// Destructeur
SoundSystem::~SoundSystem()
{
	// On supprime les sources qui composent le système OpenAL
	alDeleteSources(static_cast<ALint>(sources.size()), sources.data());
	for (auto& entry : soundBuffers)
		alDeleteBuffers(1, &entry.second);

	// On détruit le contexte permettant de jouer les différents sons
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

// Mise à jour d'un son en cours de lecture
void SoundSystem::UpdatePlayingSound(int index, bool bIsLooping, float pitch, float gain, const FVec3D& location,
	const FVec3D& velocity)
{
	// On récupère la source correspondant à l'index
	ALuint source = sources.at(index);

	// Si la source est bien en état de lecture, on met à jour les paramètres suivants :
	if (GetSourceStatus(source) == AL_PLAYING)
	{
		alSourcei(source, AL_LOOPING, bIsLooping); // Paramètre de boucle
		alSourcef(source, AL_PITCH, pitch); // Paramètre de hauteur
		alSourcef(source, AL_GAIN, gain); // Paramètre de gain
		alSource3f(source, AL_POSITION, location.GetY(), location.GetX(), location.GetZ()); // Paramètre de position
		alSource3f(source, AL_VELOCITY, velocity.GetY(), velocity.GetX(), velocity.GetZ()); // Paramètre de vélocité
	}
}

// Mise en pause d'un son
void SoundSystem::PauseSound(int index)
{
	// On récupère la source correspondant à l'index
	ALuint source = sources.at(index);

	// Si la source est bien en état de lecture, on met en pause la lecture
	if (GetSourceStatus(source) == AL_PLAYING)
		alSourcePause(source);
}

// Reprise d'un son
void SoundSystem::ResumeSound(int index)
{
	// On récupère la source correspondant à l'index
	ALuint source = sources.at(index);

	// Si la source est bien en état de pause, on reprend la lecture
	if (GetSourceStatus(source) == AL_PAUSED) 
		alSourcePlay(source);
}

// Arrêt d'un son
void SoundSystem::StopSound(int index)
{
	// On récupère la source correspondant à l'index
	ALuint source = sources.at(index);

	// Si la source n'est pas en état d'arrêt, on arrête la lecture
	if (GetSourceStatus(source) != AL_STOPPED)
		alSourceStop(source);
}

// Définition de la position du listener
void SoundSystem::SetListenerLocation(const FVec3D& location)
{
	alListener3f(AL_POSITION, location.GetY(), location.GetX(), location.GetZ());
}

// Récupération du statut d'une source (Etat Initial, En pause, En cours de lecture, A l'arrêt)
ALint SoundSystem::GetSourceStatus(ALuint source)
{
	ALint status;
	alGetSourcei(source, AL_SOURCE_STATE, &status);
	return status;
}
