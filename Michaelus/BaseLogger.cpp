#include "BaseLogger.h"

#include <filesystem>

// VARIABLES STATIQUES
std::string BaseLogger::outDirectory;

// ********* //

// Constructeur : On cr�� le dossier qui contiendra les diff�rents fichiers de Log durant l'ex�cution du programme
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

// Ecrit un message � un niveau de verbosit� donn�
void BaseLogger::Write(const std::string& message, ELoggerVerbosity priority)
{
	// Si le niveau courant est celui de la verbosit� entr�e en param�tre, on �crit le message
	if (priority == verbosity) LogMessage(message);
	// S'il existe un Logger apr�s le Logger courant, on d�l�gue de nouveau la t�che de journalisation
	if (pNext) pNext->Write(message, priority);
}

