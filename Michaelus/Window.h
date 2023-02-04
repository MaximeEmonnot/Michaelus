#pragma once

#include "Vulkan.h"

#include <memory>
#include <string>

#include "Vec2D.h"

#define WND Window::GetInstance()

class Window
{
private:
	friend class Graphics;
public:
	Window() noexcept;
	~Window();

	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	Window(Window&&) = delete;
	Window& operator=(Window&&) = delete;

	static Window& GetInstance();

	bool ProcessMessages();

	HWND GetHWND() const;
	HINSTANCE GetHInstance() const;

	int GetWidth() const;
	int GetHeight() const;

	FVec2D GetCenterOfScreen() const;

private:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:

	static std::unique_ptr<Window> pInstance;

	static HWND hWnd;
	static HINSTANCE hInstance;
	static std::wstring className;

	int width;
	int height;
};

