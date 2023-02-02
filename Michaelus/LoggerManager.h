#pragma once

#include "BaseLogger.h"

#define LOG(message, verbosity) LoggerManager::GetInstance().Log(message, verbosity);

class LoggerManager
{
public:
	LoggerManager();
	LoggerManager(const LoggerManager&) = delete;
	LoggerManager& operator= (const LoggerManager&) = delete;
	LoggerManager(LoggerManager&&) = delete;
	LoggerManager& operator=(LoggerManager&&) = delete;
	~LoggerManager() = default;

	static LoggerManager& GetInstance();

	void Log(const std::string& message, ELoggerVerbosity verbosity);

private:
	static std::unique_ptr<LoggerManager> pInstance;

	std::shared_ptr<BaseLogger> pLogger;
};

