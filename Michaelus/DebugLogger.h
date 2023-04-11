#pragma once
#include "BaseLogger.h"

/*
 * Classe DebugLogger
 * Permet la journalisation au niveau de verbosit� Debug
 */
class DebugLogger : public BaseLogger
{
public:
	// Constructeur par d�faut
	DebugLogger();
	// Constructeur suivant le patron de conception D�corateur
	DebugLogger(std::shared_ptr<BaseLogger> pNext);

protected:
	// Override de la m�thode virtuelle pure (= abstraite) h�rit�e de BaseLogger
	// Ecrit un message dans le fichier DEBUG.log
	void LogMessage(const std::string& message) override;
};

