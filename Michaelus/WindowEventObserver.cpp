#include "WindowEventObserver.h"

#include "Window.h"

// Création du lien Observateur-Observé entre l'observateur et la fenêtre
void WindowEventObserver::DefineObservable(WindowEventObserver* observer)
{
	WND.AddObserver(observer);
}
