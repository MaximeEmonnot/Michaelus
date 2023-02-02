#include "LoggerManager.h"

#include "ConsoleLogger.h"
#include "DebugLogger.h"
#include "ErrorLogger.h"

std::unique_ptr<LoggerManager> LoggerManager::pInstance = nullptr;

LoggerManager::LoggerManager()
	:
	pLogger(std::make_shared<ConsoleLogger>())
{
	pLogger->SetNext(std::make_shared<DebugLogger>())->SetNext(std::make_shared<ErrorLogger>());
}

LoggerManager& LoggerManager::GetInstance()
{
	if (!pInstance)
		pInstance = std::make_unique<LoggerManager>();
	return *pInstance;
}

void LoggerManager::Log(const std::string& message, ELoggerVerbosity verbosity)
{
	pLogger->Write(message, verbosity);
}
