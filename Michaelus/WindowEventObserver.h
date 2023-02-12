#pragma once
#include "Vulkan.h"

class WindowEventObserver
{
private:
	friend class Window;
public:
	WindowEventObserver() = default;
	virtual ~WindowEventObserver() = default;

protected:
	static void DefineObservable(WindowEventObserver* observer);
	
private:
	virtual void Update(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
};
