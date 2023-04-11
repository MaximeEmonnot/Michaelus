#include "BaseLogger.h"

#include <filesystem>

// VARIABLES STATIQUES
std::string BaseLogger::outDirectory;

// ********* //

// Constructeur : On créé le dossier qui contiendra les différents fichiers de Log durant l'exécution du programme
BaseLogger::BaseLogger()
{
	const time_t now = time(0);
	tm ltm;
	localtime_s(&ltm, &now);
	outDirectory = std::string("Logs/") + std::to_string(ltm.tm_mday) + std::string("-") + std::to_string(ltm.tm_mon + 1) + std::string("-") + std::to_string(ltm.tm_year + 1900);
	std::filesystem::create_directories(outDirectory);
}

// Destructeur : Fermeture du fichier de Log
BaseLogger::~BaseLogger()
{
	outFile.close();
}

// Ecrit un message à un niveau de verbosité donné
void BaseLogger::Write(const std::string& message, ELoggerVerbosity priority)
{
	// Si le niveau courant est celui de la verbosité entrée en paramètre, on écrit le message
	if (priority == verbosity) LogMessage(message);
	// S'il existe un Logger après le Logger courant, on délègue de nouveau la tâche de journalisation
	if (pNext) pNext->Write(message, priority);
}

