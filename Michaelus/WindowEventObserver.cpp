#include "WindowEventObserver.h"

#include "Window.h"

void WindowEventObserver::DefineObservable(WindowEventObserver* observer)
{
	WND.AddObserver(observer);
}
