#pragma once
#include "BaseLogger.h"
/*
 * Classe ErrorLogger
 * Permet la journalisation au niveau de verbosité Error
 */
class ErrorLogger : public BaseLogger
{
public:
	// Constructeur par défaut
	ErrorLogger();
	// Constructeur suivant le patron de conception Décorateur
	ErrorLogger(std::shared_ptr<BaseLogger> pNext);

protected:
	// Override de la méthode virtuelle pure (= abstraite) héritée de BaseLogger
	// Ecrit un message dans le fichier ERROR.log
	void LogMessage(const std::string& message) override;
};

