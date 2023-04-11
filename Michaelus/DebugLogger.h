#pragma once
#include "BaseLogger.h"

/*
 * Classe DebugLogger
 * Permet la journalisation au niveau de verbosité Debug
 */
class DebugLogger : public BaseLogger
{
public:
	// Constructeur par défaut
	DebugLogger();
	// Constructeur suivant le patron de conception Décorateur
	DebugLogger(std::shared_ptr<BaseLogger> pNext);

protected:
	// Override de la méthode virtuelle pure (= abstraite) héritée de BaseLogger
	// Ecrit un message dans le fichier DEBUG.log
	void LogMessage(const std::string& message) override;
};

