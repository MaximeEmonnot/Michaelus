#pragma once

#include "BaseLogger.h"
#include "EngineException.h"

#define LOG(message, verbosity) LoggerManager::GetInstance().Log(message, verbosity);

/*
 * Classe LoggerManager
 * Permet la gestion de la journalisation
 */
class LoggerManager
{
public:
	// Constructeur (publique car std::unique_ptr nécessite un constructeur publique) 
	LoggerManager();
	// Destructeur par défaut
	~LoggerManager() = default;

	// Ces différents constructeurs sont définis comme étant supprimés, pour correspondre au patron de conception Singleton
	// et également à la RO5 des normes du C++
	LoggerManager(const LoggerManager&) = delete;
	LoggerManager& operator= (const LoggerManager&) = delete;
	LoggerManager(LoggerManager&&) = delete;
	LoggerManager& operator=(LoggerManager&&) = delete;

	// Méthode du patron de conception Singleton
	static LoggerManager& GetInstance();

	// Fonction de journalisation, écrivant un message au niveau défini par la verbosité
	void Log(const std::string& message, ELoggerVerbosity verbosity);

	// Fonction d'extension du Logger
	template <typename T>
	void ExtendLogger()
	{
		// Si la classe définie entre chevrons n'hérite pas de BaseLogger, une exception est renvoyée
		if (!std::is_base_of<BaseLogger, T>())
			throw ENGINE_EXCEPTION("Vulkan 3D Engine - Main Engine Exception", "This is not a BaseLogger Class. Please check your call for ExtendLogger.");

		// On étend le Logger par le principe du Décorateur
		pLogger = std::make_shared<T>(pLogger);
	}

private:
	static std::unique_ptr<LoggerManager> pInstance;

	std::shared_ptr<BaseLogger> pLogger;
};

