#include "Window.h"

#include "Keyboard.h"
#include "Mouse.h"

std::unique_ptr<Window> Window::pInstance = nullptr;

HWND Window::hWnd;
HINSTANCE Window::hInstance;
std::wstring Window::className;

Window::Window() noexcept
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
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = className.c_str();
	wc.hIconSm = nullptr;

	RegisterClassEx(&wc);

	RECT wr;
	wr.left = 0;
	wr.top = 0;
	wr.right = width + wr.left;
	wr.bottom = height + wr.top;
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

	hWnd = CreateWindow(
		className.c_str(),
		L"Vulkan 3D Engine",
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr,
		nullptr,
		hInstance,
		nullptr
	);

	ShowWindow(hWnd, SW_SHOWDEFAULT);
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
	KBD.PopLastEvents();
	MOUSE.PopLastEvent();
	ZeroMemory(&msg, sizeof(MSG));
	while(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (msg.message == WM_QUIT) return false;
	}
	return true;
}

HWND Window::GetHWND() const
{
	return hWnd;
}

int Window::GetWidth() const
{
	return width;
}

int Window::GetHeight() const
{
	return height;
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

	case WM_KEYDOWN:
		{
		KBD.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
	case WM_KEYUP:
		{
		KBD.OnKeyReleased(static_cast<unsigned char>(wParam));
		}
		break;
	case WM_CHAR:
		{
		KBD.OnChar(static_cast<char>(wParam));
		}
		break;

	case WM_MOUSEMOVE:
		{
		const int x = LOWORD(lParam);
		const int y = HIWORD(lParam);
		MOUSE.OnMouseMove(x, y);
		}
		break;
	case WM_LBUTTONDOWN:
		{
		MOUSE.OnLeftPressed();
		}
		break;
	case WM_LBUTTONUP:
		{
		MOUSE.OnLeftReleased();
		}
		break;
	case WM_MBUTTONDOWN:
		{
		MOUSE.OnMiddlePressed();
		}
		break;
	case WM_MBUTTONUP:
		{
		MOUSE.OnMiddleReleased();
		}
		break;
	case WM_RBUTTONDOWN:
		{
		MOUSE.OnRightPressed();
		}
		break;
	case WM_RBUTTONUP:
		{
		MOUSE.OnRightReleased();
		}
		break;
	case WM_MOUSEWHEEL: 
		{
		if (GET_WHEEL_DELTA_WPARAM(wParam) >= 0) MOUSE.OnWheelUp();
		else MOUSE.OnWheelDown();
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}



