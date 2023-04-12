#pragma once
#include <Windows.h>
#include <exception>
#include <sstream>

// Macro définissant une exception selon son type et la note
#define ENGINE_EXCEPTION(type, note) EngineException(type, note, __FILE__, __LINE__)
// Macro permmettant l'affichage d'un message dans une boîte de dialogue
#define SHOW_MESSAGE(title, message) MessageBoxExA(nullptr, message, title, MB_ICONERROR | MB_OK, MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL));

/*
 * Classe d'exception du moteur Michaelus
 * Permet d'avoir des informations sur la ligne de l'exception, le fichier, et autres informations
 */
class EngineException : public std::exception
{
public:
	// Constructeur
	EngineException(const std::string& type, const std::string& note, const std::string& file, unsigned int line) noexcept
		:
		type(type),
		note(note),
		file(file),
		line(line)
	{
		infos = GetNote() + std::string("\nDetails:\n") + GetLocation();
	}

	// Constructeur de copie
	EngineException(const EngineException& copy) noexcept
		:
		EngineException(copy.type, copy.note, copy.file, copy.line)
	{}

	// Opérateur de copie
	EngineException& operator=(const EngineException& engineException)
	{
		*this = EngineException(engineException);
		return *this;
	}

	// Constructeur de déplacement (Move Constructor)
	EngineException(EngineException&& move) noexcept
		:
		infos(std::move(move.infos)),
		type(std::move(move.type)),
		note(std::move(move.note)),
		file(std::move(move.file)),
		line(move.line)
	{}

	// Opérateur de déplacement (Move Operator)
	EngineException& operator=(EngineException&& engineException) noexcept
	{
		*this = EngineException(engineException);
		return *this;
	}

	// Destructeur par défaut
	~EngineException() override = default;

	// Récupération du type d'exception
	inline std::string GetType() const noexcept
	{
		return type;
	}

	// Récupération du fichier d'exception
	inline std::string GetFile() const noexcept
	{
		return file;
	}

	// Récupération du message d'exception
	inline std::string GetNote() const noexcept
	{
		return note;
	}

	// Récupération de la ligne d'exception
	inline unsigned int GetLine() const noexcept
	{
		return line;
	}

	// Récupération de la position de l'exception (Fichier + Ligne)
	inline std::string GetLocation() const noexcept
	{
		return std::string("[File]: \"") + file + std::string("\"\n[Line]: \"") + std::to_string(line);
	}

	// Override de la méthode what() de la classe std::exception
	// Renvoie simplement les informations de l'exception
	const char* what() const override
	{
		return infos.c_str();
	}
protected:
	std::string		infos;
private:
	std::string		type;
	std::string		note;
	std::string		file;
	unsigned int	line;
};