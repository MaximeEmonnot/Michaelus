#include "Window.h"

#include "WindowEventObserver.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "resource.h"

std::unique_ptr<Window> Window::pInstance = nullptr;
std::vector<WindowEventObserver*> Window::observers;
HWND Window::hWnd;
HINSTANCE Window::hInstance;
std::wstring Window::className;

Window::Window() noexcept
	:
	width(GetSystemMetrics(SM_CXSCREEN)),
	height(GetSystemMetrics(SM_CYSCREEN))
{
	hInstance = GetModuleHandle(nullptr);
	className = L"Vulkan 3D Engine";

	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = static_cast<HICON>(LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));
	wc.hCursor = nullptr;
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = className.c_str();
	wc.hIconSm = static_cast<HICON>(LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));

	RegisterClassEx(&wc);

	hWnd = CreateWindow(
		className.c_str(),
		L"Vulkan 3D Engine",
		WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height,
		nullptr,
		nullptr,
		hInstance,
		nullptr
	);

	ShowWindow(hWnd, SW_SHOWDEFAULT);
	ShowCursor(FALSE);
}

Window::~Window()
{
	DestroyWindow(hWnd);
}

Window& Window::GetInstance()
{
	if (!pInstance) pInstance = std::make_unique<Window>();
	return *pInstance;
}

bool Window::ProcessMessages()
{
	MSG msg;
	SetCursorPos(width / 2, height / 2);
	ZeroMemory(&msg, sizeof(MSG));
	while(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (msg.message == WM_QUIT) return false;
	}
	return true;
}

void Window::AddObserver(WindowEventObserver* observer)
{
	observers.push_back(observer);
}

HWND Window::GetHWND() const
{
	return hWnd;
}

HINSTANCE Window::GetHInstance() const
{
	return hInstance;
}

int Window::GetWidth() const
{
	return width;
}

int Window::GetHeight() const
{
	return height;
}

FVec2D Window::GetCenterOfScreen() const
{
	return { static_cast<float>(width) / 2.f , static_cast<float>(height) / 2.f };
}

LRESULT Window::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch(uMsg)
	{
	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_MOVING:
		break;

	default:
		for (WindowEventObserver* pObserver : observers) pObserver->Update(uMsg, wParam, lParam);
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}



