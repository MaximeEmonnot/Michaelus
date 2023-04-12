#pragma once

#include "Vulkan.h"

#include <memory>
#include <string>

#include "Vec2D.h"
#include "WindowEventObserver.h"

// Macro pour l'utilisation plus ais�e de la classe Window
#define WND Window::GetInstance()

/*
 * Classe d�finissant la fen�tre de l'application
 * D�finit une taille, ainsi que le traitement des �v�nements via l'API Win32
 */
class Window
{
private:
	friend class Graphics;
public:
	// Constructeur (publique car std::unique_ptr n�cessite un constructeur publique) 
	Window() noexcept;
	// Destructeur par d�faut
	~Window();

	// Ces diff�rents constructeurs sont d�finis comme �tant supprim�s, pour correspondre au patron de conception Singleton
	// et �galement � la RO5 des normes du C++
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	Window(Window&&) = delete;
	Window& operator=(Window&&) = delete;

	// M�thode du patron de conception Singleton
	static Window& GetInstance();

	// Traitement des messages de l'API Win32
	bool ProcessMessages();

	// Ajout d'un observateur de la fen�tre
	void AddObserver(WindowEventObserver* observer);

	// R�cup�ration du HWND de l'API Win32
	HWND GetHWND() const;
	// R�cup�ration de l'Instance de la fen�tre de l'API Win32
	HINSTANCE GetHInstance() const;

	// R�cup�ration de la largeur de la fen�tre
	int GetWidth() const;
	// R�cup�ration de la hauteur de la fen�tre
	int GetHeight() const;

	// R�cup�ration du centre de la fen�tre
	FVec2D GetCenterOfScreen() const;

private:
	// M�thode du processus de la fen�tre, propre � l'API Win32
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

