#pragma once
#include <memory>
#include <string>
#include <Windows.h>

#define WND Window::GetInstance()

class Window
{
private:

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

private:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:

	static std::unique_ptr<Window> pInstance;

	HWND hWnd;
	static HINSTANCE hInstance;
	static std::wstring className;

	static constexpr int width = 800;
	static constexpr int height = 640;
};

