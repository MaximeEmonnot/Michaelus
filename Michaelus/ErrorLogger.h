#pragma once
#include "BaseLogger.h"
/*
 * Classe ErrorLogger
 * Permet la journalisation au niveau de verbosit� Error
 */
class ErrorLogger : public BaseLogger
{
public:
	// Constructeur par d�faut
	ErrorLogger();
	// Constructeur suivant le patron de conception D�corateur
	ErrorLogger(std::shared_ptr<BaseLogger> pNext);

protected:
	// Override de la m�thode virtuelle pure (= abstraite) h�rit�e de BaseLogger
	// Ecrit un message dans le fichier ERROR.log
	void LogMessage(const std::string& message) override;
};

