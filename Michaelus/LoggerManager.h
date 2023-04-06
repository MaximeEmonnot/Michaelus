#pragma once

#include "BaseLogger.h"
#include "EngineException.h"

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

	template <typename T>
	void ExtendLogger()
	{
		if (!std::is_base_of<BaseLogger, T>())
			throw ENGINE_EXCEPTION("Vulkan 3D Engine - Main Engine Exception", "This is not a BaseLogger Class. Please check your call for ExtendLogger.");

		pLogger = std::make_shared<T>(pLogger);
	}

private:
	static std::unique_ptr<LoggerManager> pInstance;

	std::shared_ptr<BaseLogger> pLogger;
};

