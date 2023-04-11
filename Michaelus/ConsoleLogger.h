#pragma once
#include "BaseLogger.h"

/*
 * Classe ConsoleLogger
 * Permet la journalisation au niveau de verbosité Console
 */
class ConsoleLogger : public BaseLogger
{
public:
	// Constructeur par défaut
	ConsoleLogger();
	// Constructeur suivant le patron de conception Décorateur
	ConsoleLogger(std::shared_ptr<BaseLogger> pNext);

protected:
	// Override de la méthode virtuelle pure (= abstraite) héritée de BaseLogger
	// Ecrit un message dans le fichier CONSOLE.log
	void LogMessage(const std::string& message) override;
};

