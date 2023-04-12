#pragma once
#include <Windows.h>
#include <exception>
#include <sstream>

// Macro d�finissant une exception selon son type et la note
#define ENGINE_EXCEPTION(type, note) EngineException(type, note, __FILE__, __LINE__)
// Macro permmettant l'affichage d'un message dans une bo�te de dialogue
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

	// Op�rateur de copie
	EngineException& operator=(const EngineException& engineException)
	{
		*this = EngineException(engineException);
		return *this;
	}

	// Constructeur de d�placement (Move Constructor)
	EngineException(EngineException&& move) noexcept
		:
		infos(std::move(move.infos)),
		type(std::move(move.type)),
		note(std::move(move.note)),
		file(std::move(move.file)),
		line(move.line)
	{}

	// Op�rateur de d�placement (Move Operator)
	EngineException& operator=(EngineException&& engineException) noexcept
	{
		*this = EngineException(engineException);
		return *this;
	}

	// Destructeur par d�faut
	~EngineException() override = default;

	// R�cup�ration du type d'exception
	inline std::string GetType() const noexcept
	{
		return type;
	}

	// R�cup�ration du fichier d'exception
	inline std::string GetFile() const noexcept
	{
		return file;
	}

	// R�cup�ration du message d'exception
	inline std::string GetNote() const noexcept
	{
		return note;
	}

	// R�cup�ration de la ligne d'exception
	inline unsigned int GetLine() const noexcept
	{
		return line;
	}

	// R�cup�ration de la position de l'exception (Fichier + Ligne)
	inline std::string GetLocation() const noexcept
	{
		return std::string("[File]: \"") + file + std::string("\"\n[Line]: \"") + std::to_string(line);
	}

	// Override de la m�thode what() de la classe std::exception
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