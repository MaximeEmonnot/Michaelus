#include "WindowEventObserver.h"

#include "Window.h"

// Cr�ation du lien Observateur-Observ� entre l'observateur et la fen�tre
void WindowEventObserver::DefineObservable(WindowEventObserver* observer)
{
	WND.AddObserver(observer);
}
