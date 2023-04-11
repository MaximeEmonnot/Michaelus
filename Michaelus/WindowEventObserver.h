#pragma once
#include "Vulkan.h"

/*
 * Classe abstraite d'un Observateur
 * Permet d'observer les différents évènements de la fenêtre (classe Window)
 */
class WindowEventObserver
{
private:
	friend class Window;
public:
	// Constructeur par défaut
	WindowEventObserver() = default;
	// Destructeur par défaut (virtuel pour être Override par la suite)
	virtual ~WindowEventObserver() = default;

	// Autres constructeurs par défaut, pour correspondre à la RO5 propre au C++
	WindowEventObserver(const WindowEventObserver&) = default;
	WindowEventObserver& operator=(const WindowEventObserver&) = default;
	WindowEventObserver(WindowEventObserver&&) = default;
	WindowEventObserver& operator=(WindowEventObserver&&) = default;

protected:
	// Création du lien Observateur-Observé entre l'observateur et la fenêtre
	static void DefineObservable(WindowEventObserver* observer);
	
private:
	// Classe virtuelle pure (= abstraite) pour le traitement des évènements
	// Cette classe sera définie dans les classes héritant de WindowEventObserver
	virtual void Update(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
};
