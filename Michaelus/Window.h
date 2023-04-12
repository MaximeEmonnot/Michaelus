#pragma once

#include "Vulkan.h"

#include <memory>
#include <string>

#include "Vec2D.h"
#include "WindowEventObserver.h"

// Macro pour l'utilisation plus aisée de la classe Window
#define WND Window::GetInstance()

/*
 * Classe définissant la fenêtre de l'application
 * Définit une taille, ainsi que le traitement des évènements via l'API Win32
 */
class Window
{
private:
	friend class Graphics;
public:
	// Constructeur (publique car std::unique_ptr nécessite un constructeur publique) 
	Window() noexcept;
	// Destructeur par défaut
	~Window();

	// Ces différents constructeurs sont définis comme étant supprimés, pour correspondre au patron de conception Singleton
	// et également à la RO5 des normes du C++
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	Window(Window&&) = delete;
	Window& operator=(Window&&) = delete;

	// Méthode du patron de conception Singleton
	static Window& GetInstance();

	// Traitement des messages de l'API Win32
	bool ProcessMessages();

	// Ajout d'un observateur de la fenêtre
	void AddObserver(WindowEventObserver* observer);

	// Récupération du HWND de l'API Win32
	HWND GetHWND() const;
	// Récupération de l'Instance de la fenêtre de l'API Win32
	HINSTANCE GetHInstance() const;

	// Récupération de la largeur de la fenêtre
	int GetWidth() const;
	// Récupération de la hauteur de la fenêtre
	int GetHeight() const;

	// Récupération du centre de la fenêtre
	FVec2D GetCenterOfScreen() const;

private:
	// Méthode du processus de la fenêtre, propre à l'API Win32
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	static std::unique_ptr<Window> pInstance;

	static std::vector<WindowEventObserver*> observers;

	static HWND hWnd;
	static HINSTANCE hInstance;
	static std::wstring className;

	int width;
	int height;
};

