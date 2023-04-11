#pragma once
#include <fstream>
#include <memory>
#include <string>

/*
 * Verbosit� du syst�me de journalisation
 * 3 Niveaux : Console, Debug, Erreur
 */
enum class ELoggerVerbosity
{
	Console,
	Debug,
	Error,
	None
};

/*
 * Classe abstraite BaseLogger
 * Permet d'�crire un message dans un des niveaux de verbosit�
 * Utilise les patrons de conception Decorateur et Chaine de responsabilit�
 */
class BaseLogger
{
public:
	// Constructeur par d�faut
	BaseLogger();
	// Destructeur virtuel
	virtual ~BaseLogger();

	// Ecrit un message � un niveau de verbosit� donn�
	void Write(const std::string& message, ELoggerVerbosity priority);

protected:
	// M�thode virtuelle pure (= abstraite) permettant l'�criture du message dans un fichier
	virtual void LogMessage(const std::string& message) = 0;

protected:
	static std::string outDirectory;

	std::fstream outFile;

	std::shared_ptr<BaseLogger> pNext;
	ELoggerVerbosity verbosity = ELoggerVerbosity::None;
};

