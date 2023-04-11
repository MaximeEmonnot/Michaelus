#include "Keyboard.h"

// VARIABLES STATIQUES
std::unique_ptr<Keyboard> Keyboard::pInstance = nullptr;

// ********* //

// Destructeur : On vide les buffers
Keyboard::~Keyboard()
{
	FlushBuffers();
}

// Fonction propre au patron de conception Singleton
Keyboard& Keyboard::GetInstance()
{
	if (!pInstance) {
		pInstance = std::make_unique<Keyboard>();
		DefineObservable(pInstance.get()); // On définit la classe clavier comme étant un observable de la fenêtre
	}
	return *pInstance;
}

// Méthode Update issue de la classe WindowEventObserver
void Keyboard::Update(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// On supprime l'évènement le plus vieux
	PopLastEvents();

	// On ne s'intéresse qu'aux évènements propres au clavier
	switch (uMsg)
	{
	case WM_KEYDOWN:
	{
		OnKeyPressed(static_cast<unsigned char>(wParam));
	}
	break;
	case WM_KEYUP:
	{
		OnKeyReleased(static_cast<unsigned char>(wParam));
	}
	break;
	case WM_CHAR:
	{
		OnChar(static_cast<char>(wParam));
	}
	break;
	default: break;
	}
}

// Retourne l'état (Pressé / Relâché) de la touche associée au keycode
bool Keyboard::KeyIsPressed(unsigned char keycode) const
{
	return keyStates[keycode];
}

// Retourne le dernier évènement enregistré
Keyboard::Event Keyboard::ReadKey() const
{
	return eventBuffer.empty() ? Event() : eventBuffer.front();
}

// Retourne le dernier caractère enregistré
char Keyboard::ReadChar() const
{
	return charBuffer.empty() ? 0 : charBuffer.front();
}

// Retourne l'état de la liste d'évènement (vide ou non)
bool Keyboard::KeyIsEmpty() const
{
	return eventBuffer.empty();
}

// Sous-méthodes du traitement d'évènements (Touche pressée / relachée, Caractère appuyé) :
// A chaque fois, on vérifie si l'on doit réduire la taille du buffer d'évènements
void Keyboard::OnKeyPressed(unsigned char keycode)
{
	keyStates[keycode] = true;
	eventBuffer.push({ keycode, Event::Type::Pressed });
	TrimBuffer(eventBuffer);
}

void Keyboard::OnKeyReleased(unsigned char keycode)
{
	keyStates[keycode] = false;
	eventBuffer.push({ keycode, Event::Type::Released });
	TrimBuffer(eventBuffer);
}

void Keyboard::OnChar(char keycode)
{
	charBuffer.push(keycode);
	TrimBuffer(charBuffer);
}

// Vide les buffers d'évènements
void Keyboard::FlushBuffers()
{
	eventBuffer = std::queue<Event>();
	charBuffer = std::queue<char>();
}

// Supprime l'évènement le plus vieux si l'on dépasse la taille maximale d'évènements
template <typename T>
void Keyboard::TrimBuffer(std::queue<T>& buffer)
{
	if (buffer.size() > bufferSize) buffer.pop();
}

// Supprime l'évènement le plus vieux
void Keyboard::PopLastEvents()
{
	if (!eventBuffer.empty()) eventBuffer.pop();
	if (!charBuffer.empty()) charBuffer.pop();
}