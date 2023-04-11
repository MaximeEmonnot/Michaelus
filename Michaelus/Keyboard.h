#pragma once
#include <memory>
#include <bitset>
#include <queue>

#include "WindowEventObserver.h"

#define KBD Keyboard::GetInstance()

/*
 * Classe enregistrant tous les évènements liés au clavier
 */
class Keyboard : public WindowEventObserver
{
public:
	/*
	 * Structure d'un évènement
	 * Défini par un keycode et un évènement lié au keycode
	 */
	struct Event
	{
		/*
		 * Trois types d'évènement : Pressé, Relâché, Rien
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
	// Constructeur par défaut
	Keyboard() = default;
	// Destructeur
	~Keyboard() override;

	// Ces différents constructeurs sont définis comme étant supprimés, pour correspondre au patron de conception Singleton
	// et également à la RO5 des normes du C++
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;
	Keyboard(Keyboard&&) = delete;
	Keyboard& operator=(Keyboard&&) = delete;

	// Méthode du patron de conception Singleton
	static Keyboard& GetInstance();

	// Méthode Update issue de la classe WindowEventObserver
	virtual void Update(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	// Retourne l'état (Pressé / Relâché) de la touche associée au keycode
	bool KeyIsPressed(unsigned char keycode) const;
	// Retourne le dernier évènement enregistré
	Keyboard::Event ReadKey() const;
	// Retourne le dernier caractère enregistré
	char ReadChar() const;
	// Retourne l'état de la liste d'évènement (vide ou non)
	bool KeyIsEmpty() const;

private:
	// Sous-méthodes du traitement d'évènements (Touche pressée / relachée, Caractère appuyé)
	void OnKeyPressed(unsigned char keycode);
	void OnKeyReleased(unsigned char keycode);
	void OnChar(char keycode);

	// Vide les buffers d'évènements
	void FlushBuffers();

	// Supprime l'évènement le plus vieux si l'on dépasse la taille maximale d'évènements
	template <typename T>
	void TrimBuffer(std::queue<T>& buffer);

	// Supprime l'évènement le plus vieux
	void PopLastEvents();

private:
	static std::unique_ptr<Keyboard> pInstance;

	const unsigned int bufferSize = 4u;
	std::bitset<256> keyStates;
	std::queue<Event> eventBuffer;
	std::queue<char> charBuffer;
};

