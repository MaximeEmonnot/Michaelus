#pragma once
#include "BaseLogger.h"

/*
 * Classe ConsoleLogger
 * Permet la journalisation au niveau de verbosit� Console
 */
class ConsoleLogger : public BaseLogger
{
public:
	// Constructeur par d�faut
	ConsoleLogger();
	// Constructeur suivant le patron de conception D�corateur
	ConsoleLogger(std::shared_ptr<BaseLogger> pNext);

protected:
	// Override de la m�thode virtuelle pure (= abstraite) h�rit�e de BaseLogger
	// Ecrit un message dans le fichier CONSOLE.log
	void LogMessage(const std::string& message) override;
};

