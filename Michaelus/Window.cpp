#include "Window.h"

#include "WindowEventObserver.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "resource.h"

// VARIABLES STATIQUES
std::unique_ptr<Window> Window::pInstance = nullptr;
std::vector<WindowEventObserver*> Window::observers;
HWND Window::hWnd;
HINSTANCE Window::hInstance;
std::wstring Window::className;

// ********* //

// Constructeur
Window::Window() noexcept
	:
	width(GetSystemMetrics(SM_CXSCREEN)),
	height(GetSystemMetrics(SM_CYSCREEN))
{
	// On définit l'instance de la fenêtre
	hInstance = GetModuleHandle(nullptr);

	// On définit la classe de la fenêtre
	className = L"Vulkan 3D Engine";
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(wc); // Taille de la variable
	wc.style = CS_OWNDC; 
	wc.lpfnWndProc = WindowProc; // Processus des messages Windows
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance; // Instance de la fenêtre
	wc.hIcon = static_cast<HICON>(LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0)); // Icône
	wc.hCursor = nullptr; // Curseur
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)); // Couleur du background (GDI)
	wc.lpszMenuName = nullptr; // Menu
	wc.lpszClassName = className.c_str(); // Nom de la classe
	wc.hIconSm = static_cast<HICON>(LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0)); // Petite icône

	// Enregistrement de la classe
	RegisterClassEx(&wc);

	// Création de la fenêtre
	hWnd = CreateWindow(
		className.c_str(), // Nom de la classe
		L"Vulkan 3D Engine - Michaelus", // Titre de la fenêtre
		WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, // Tags
		CW_USEDEFAULT, CW_USEDEFAULT, width, height, // Dimensions
		nullptr,
		nullptr,
		hInstance, // Instance de la fenêtre
		nullptr
	);

	// On affiche la fenêtre et on masque le curseur
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	ShowCursor(FALSE);
}

// Destructeur
Window::~Window()
{
	DestroyWindow(hWnd);
}

// Méthode du patron de conception Singleton
Window& Window::GetInstance()
{
	if (!pInstance) pInstance = std::make_unique<Window>();
	return *pInstance;
}

// Traitement des messages de l'API Win32
bool Window::ProcessMessages()
{
	MSG msg;
	SetCursorPos(width / 2, height / 2); // On repositionne le curseur au centre de la fenêtre
	ZeroMemory(&msg, sizeof(MSG));
	while(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (msg.message == WM_QUIT) return false;
	}
	return true;
}

// Ajout d'un observateur de la fenêtre
void Window::AddObserver(WindowEventObserver* observer)
{
	observers.push_back(observer);
}

// Récupération du HWND de l'API Win32
HWND Window::GetHWND() const
{
	return hWnd;
}

// Récupération de l'Instance de la fenêtre de l'API Win32
HINSTANCE Window::GetHInstance() const
{
	return hInstance;
}

// Récupération de la largeur de la fenêtre
int Window::GetWidth() const
{
	return width;
}

// Récupération de la hauteur de la fenêtre
int Window::GetHeight() const
{
	return height;
}

// Récupération du centre de la fenêtre
FVec2D Window::GetCenterOfScreen() const
{
	return { static_cast<float>(width) / 2.f , static_cast<float>(height) / 2.f };
}

// Méthode du processus de la fenêtre, propre à l'API Win32
LRESULT Window::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch(uMsg)
	{
	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage(0); // Si l'on clique sur la croix ou si l'on fait Alt+F4
		return 0;
	case WM_MOVING:
		break;

	default:
		// Dans tous les autres cas, le traitement des évènements est assigné aux observateurs
		for (WindowEventObserver* pObserver : observers) pObserver->Update(uMsg, wParam, lParam);
		break;
	}

	// On retourne le processus par défaut de l'API Win32
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}



