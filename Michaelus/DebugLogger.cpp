#include "DebugLogger.h"

#include <Windows.h>
#include <chrono>

// Constructeur par d�faut : Cr�ation du fichier de log DEBUG.log
DebugLogger::DebugLogger()
{
	verbosity = ELoggerVerbosity::Debug;
	outFile.open(outDirectory + "/DEBUG.log", std::ios::out | std::ios::app);
}

// Constructeur suivant le patron de conception D�corateur
// Cr�ation du fichier de log DEBUG.log et lien avec le niveau de responsabilit� suivant
DebugLogger::DebugLogger(std::shared_ptr<BaseLogger> pNext)
{
	this->pNext = pNext;
	verbosity = ELoggerVerbosity::Debug;
	outFile.open(outDirectory + "/DEBUG.log", std::ios::out | std::ios::app);
}

// Override de la m�thode virtuelle pure (= abstraite) h�rit�e de BaseLogger
// Ecrit un message dans le fichier DEBUG.log
void DebugLogger::LogMessage(const std::string& message)
{
	const __time64_t timestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::string time;
	time.resize(26);
	ctime_s(time.data(), time.size(), &timestamp);
	const std::string out_message = time + std::string("[DEBUG] : ") + message + "\n";
	outFile << out_message;
	OutputDebugStringA(out_message.c_str()); // On affiche �galement le message dans l'Output de Debug
}
