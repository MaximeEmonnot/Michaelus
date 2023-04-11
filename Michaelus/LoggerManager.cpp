#include "LoggerManager.h"

#include "ConsoleLogger.h"
#include "DebugLogger.h"
#include "ErrorLogger.h"

// VARIABLES STATIQUES
std::unique_ptr<LoggerManager> LoggerManager::pInstance = nullptr;

// ********* //

// Constructeur
LoggerManager::LoggerManager()
	:
	// Construction du logger par principe du Décorateur
	pLogger(std::make_shared<ConsoleLogger>(std::make_shared<DebugLogger>(std::make_shared<ErrorLogger>())))
{
}

// Méthode du patron de conception Singleton
LoggerManager& LoggerManager::GetInstance()
{
	if (!pInstance)
		pInstance = std::make_unique<LoggerManager>();
	return *pInstance;
}

// Fonction de journalisation, écrivant un message au niveau défini par la verbosité
void LoggerManager::Log(const std::string& message, ELoggerVerbosity verbosity)
{
	pLogger->Write(message, verbosity);
}
