#pragma once
#include <fstream>
#include <memory>
#include <string>

/*
 * Verbosité du système de journalisation
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
 * Permet d'écrire un message dans un des niveaux de verbosité
 * Utilise les patrons de conception Decorateur et Chaine de responsabilité
 */
class BaseLogger
{
public:
	// Constructeur par défaut
	BaseLogger();
	// Destructeur virtuel
	virtual ~BaseLogger();

	// Ecrit un message à un niveau de verbosité donné
	void Write(const std::string& message, ELoggerVerbosity priority);

protected:
	// Méthode virtuelle pure (= abstraite) permettant l'écriture du message dans un fichier
	virtual void LogMessage(const std::string& message) = 0;

protected:
	static std::string outDirectory;

	std::fstream outFile;

	std::shared_ptr<BaseLogger> pNext;
	ELoggerVerbosity verbosity = ELoggerVerbosity::None;
};

