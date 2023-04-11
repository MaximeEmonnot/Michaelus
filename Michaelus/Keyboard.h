#pragma once
#include <memory>
#include <bitset>
#include <queue>

#include "WindowEventObserver.h"

#define KBD Keyboard::GetInstance()

/*
 * Classe enregistrant tous les �v�nements li�s au clavier
 */
class Keyboard : public WindowEventObserver
{
public:
	/*
	 * Structure d'un �v�nement
	 * D�fini par un keycode et un �v�nement li� au keycode
	 */
	struct Event
	{
		/*
		 * Trois types d'�v�nement : Press�, Rel�ch�, Rien
		 */
		enum class Type
		{
			Pressed,
			Released,
			None
		};
		unsigned char keycode = 0u;
		Type type = Type::None;
	};

public:
	// Constructeur par d�faut
	Keyboard() = default;
	// Destructeur
	~Keyboard() override;

	// Ces diff�rents constructeurs sont d�finis comme �tant supprim�s, pour correspondre au patron de conception Singleton
	// et �galement � la RO5 des normes du C++
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;
	Keyboard(Keyboard&&) = delete;
	Keyboard& operator=(Keyboard&&) = delete;

	// M�thode du patron de conception Singleton
	static Keyboard& GetInstance();

	// M�thode Update issue de la classe WindowEventObserver
	virtual void Update(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	// Retourne l'�tat (Press� / Rel�ch�) de la touche associ�e au keycode
	bool KeyIsPressed(unsigned char keycode) const;
	// Retourne le dernier �v�nement enregistr�
	Keyboard::Event ReadKey() const;
	// Retourne le dernier caract�re enregistr�
	char ReadChar() const;
	// Retourne l'�tat de la liste d'�v�nement (vide ou non)
	bool KeyIsEmpty() const;

private:
	// Sous-m�thodes du traitement d'�v�nements (Touche press�e / relach�e, Caract�re appuy�)
	void OnKeyPressed(unsigned char keycode);
	void OnKeyReleased(unsigned char keycode);
	void OnChar(char keycode);

	// Vide les buffers d'�v�nements
	void FlushBuffers();

	// Supprime l'�v�nement le plus vieux si l'on d�passe la taille maximale d'�v�nements
	template <typename T>
	void TrimBuffer(std::queue<T>& buffer);

	// Supprime l'�v�nement le plus vieux
	void PopLastEvents();

private:
	static std::unique_ptr<Keyboard> pInstance;

	const unsigned int bufferSize = 4u;
	std::bitset<256> keyStates;
	std::queue<Event> eventBuffer;
	std::queue<char> charBuffer;
};

