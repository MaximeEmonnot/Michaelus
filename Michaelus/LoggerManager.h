#pragma once

#include "BaseLogger.h"
#include "EngineException.h"

// Macro pour l'�criture d'un message � un certain niveau de verbosit�
#define LOG(message, verbosity) LoggerManager::GetInstance().Log(message, verbosity);

/*
 * Classe LoggerManager
 * Permet la gestion de la journalisation
 */
class LoggerManager
{
public:
	// Constructeur (publique car std::unique_ptr n�cessite un constructeur publique) 
	LoggerManager();
	// Destructeur par d�faut
	~LoggerManager() = default;

	// Ces diff�rents constructeurs sont d�finis comme �tant supprim�s, pour correspondre au patron de conception Singleton
	// et �galement � la RO5 des normes du C++
	LoggerManager(const LoggerManager&) = delete;
	LoggerManager& operator= (const LoggerManager&) = delete;
	LoggerManager(LoggerManager&&) = delete;
	LoggerManager& operator=(LoggerManager&&) = delete;

	// M�thode du patron de conception Singleton
	static LoggerManager& GetInstance();

	// Fonction de journalisation, �crivant un message au niveau d�fini par la verbosit�
	void Log(const std::string& message, ELoggerVerbosity verbosity);

	// Fonction d'extension du Logger
	template <typename T>
	void ExtendLogger()
	{
		// Une exception est lev�e si la classe entre chevrons n'h�rite pas de BaseLogger
		if (!std::is_base_of<BaseLogger, T>())
			throw ENGINE_EXCEPTION("Michaelus 3D Engine - Main Engine Exception", "This is not a BaseLogger Class. Please check your call for ExtendLogger.");

		// On �tend le Logger par le principe du D�corateur
		pLogger = std::make_shared<T>(pLogger);
	}

private:
	static std::unique_ptr<LoggerManager> pInstance;

	std::shared_ptr<BaseLogger> pLogger;
};

