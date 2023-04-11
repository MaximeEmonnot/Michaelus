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
	// Construction du logger par principe du D�corateur
	pLogger(std::make_shared<ConsoleLogger>(std::make_shared<DebugLogger>(std::make_shared<ErrorLogger>())))
{
}

// M�thode du patron de conception Singleton
LoggerManager& LoggerManager::GetInstance()
{
	if (!pInstance)
		pInstance = std::make_unique<LoggerManager>();
	return *pInstance;
}

// Fonction de journalisation, �crivant un message au niveau d�fini par la verbosit�
void LoggerManager::Log(const std::string& message, ELoggerVerbosity verbosity)
{
	pLogger->Write(message, verbosity);
}
