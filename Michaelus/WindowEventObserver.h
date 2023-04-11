#pragma once
#include "Vulkan.h"

/*
 * Classe abstraite d'un Observateur
 * Permet d'observer les diff�rents �v�nements de la fen�tre (classe Window)
 */
class WindowEventObserver
{
private:
	friend class Window;
public:
	// Constructeur par d�faut
	WindowEventObserver() = default;
	// Destructeur par d�faut (virtuel pour �tre Override par la suite)
	virtual ~WindowEventObserver() = default;

	// Autres constructeurs par d�faut, pour correspondre � la RO5 propre au C++
	WindowEventObserver(const WindowEventObserver&) = default;
	WindowEventObserver& operator=(const WindowEventObserver&) = default;
	WindowEventObserver(WindowEventObserver&&) = default;
	WindowEventObserver& operator=(WindowEventObserver&&) = default;

protected:
	// Cr�ation du lien Observateur-Observ� entre l'observateur et la fen�tre
	static void DefineObservable(WindowEventObserver* observer);
	
private:
	// Classe virtuelle pure (= abstraite) pour le traitement des �v�nements
	// Cette classe sera d�finie dans les classes h�ritant de WindowEventObserver
	virtual void Update(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
};
