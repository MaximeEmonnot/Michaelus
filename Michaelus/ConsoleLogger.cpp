#include "ConsoleLogger.h"

#include <Windows.h>
#include <chrono>

// Constructeur par défaut : Création du fichier de log CONSOLE.log
ConsoleLogger::ConsoleLogger()
{
	verbosity = ELoggerVerbosity::Console;
	outFile.open(outDirectory + "/CONSOLE.log", std::ios::out | std::ios::app);
}

// Constructeur suivant le patron de conception Décorateur
// Création du fichier de log CONSOLE.log et lien avec le niveau de responsabilité suivant
ConsoleLogger::ConsoleLogger(std::shared_ptr<BaseLogger> pNext)
{
	this->pNext = pNext;
	verbosity = ELoggerVerbosity::Console;
	outFile.open(outDirectory + "/CONSOLE.log", std::ios::out | std::ios::app);
}

// Override de la méthode virtuelle pure (= abstraite) héritée de BaseLogger
// Ecrit un message dans le fichier CONSOLE.log
void ConsoleLogger::LogMessage(const std::string& message)
{
	const __time64_t timestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::string time;
	time.resize(26);
	ctime_s(time.data(), time.size(), &timestamp);
	const std::string out_message = time + std::string("[CONSOLE] : ") + message + "\n";
	outFile << out_message;
	OutputDebugStringA(out_message.c_str()); // On affiche également le message dans l'Output de Debug
}
