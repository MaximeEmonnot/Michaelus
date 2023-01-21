#pragma once

#include "Vulkan.h"

#include <memory>
#include <string>

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

private:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:

	static std::unique_ptr<Window> pInstance;

	static HWND hWnd;
	static HINSTANCE hInstance;
	static std::wstring className;

	static constexpr int width = 800;
	static constexpr int height = 640;
};

