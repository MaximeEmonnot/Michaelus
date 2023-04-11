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
		DefineObservable(pInstance.get()); // On d�finit la classe clavier comme �tant un observable de la fen�tre
	}
	return *pInstance;
}

// M�thode Update issue de la classe WindowEventObserver
void Keyboard::Update(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// On supprime l'�v�nement le plus vieux
	PopLastEvents();

	// On ne s'int�resse qu'aux �v�nements propres au clavier
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

// Retourne l'�tat (Press� / Rel�ch�) de la touche associ�e au keycode
bool Keyboard::KeyIsPressed(unsigned char keycode) const
{
	return keyStates[keycode];
}

// Retourne le dernier �v�nement enregistr�
Keyboard::Event Keyboard::ReadKey() const
{
	return eventBuffer.empty() ? Event() : eventBuffer.front();
}

// Retourne le dernier caract�re enregistr�
char Keyboard::ReadChar() const
{
	return charBuffer.empty() ? 0 : charBuffer.front();
}

// Retourne l'�tat de la liste d'�v�nement (vide ou non)
bool Keyboard::KeyIsEmpty() const
{
	return eventBuffer.empty();
}

// Sous-m�thodes du traitement d'�v�nements (Touche press�e / relach�e, Caract�re appuy�) :
// A chaque fois, on v�rifie si l'on doit r�duire la taille du buffer d'�v�nements
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

// Vide les buffers d'�v�nements
void Keyboard::FlushBuffers()
{
	eventBuffer = std::queue<Event>();
	charBuffer = std::queue<char>();
}

// Supprime l'�v�nement le plus vieux si l'on d�passe la taille maximale d'�v�nements
template <typename T>
void Keyboard::TrimBuffer(std::queue<T>& buffer)
{
	if (buffer.size() > bufferSize) buffer.pop();
}

// Supprime l'�v�nement le plus vieux
void Keyboard::PopLastEvents()
{
	if (!eventBuffer.empty()) eventBuffer.pop();
	if (!charBuffer.empty()) charBuffer.pop();
}