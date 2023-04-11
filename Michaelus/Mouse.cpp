#include "Mouse.h"

#include "Window.h"

// VARIABLES STATIQUES
std::unique_ptr<Mouse> Mouse::pInstance = nullptr;

// ********* //

// Destructeur : On vide le buffer
Mouse::~Mouse()
{
    FlushBuffer();
}

// Fonction propre au patron de conception Singleton
Mouse& Mouse::GetInstance()
{
    if (!pInstance)
    {
        pInstance = std::make_unique<Mouse>();
		DefineObservable(pInstance.get()); // On définit la classe souris comme étant un observable de la fenêtre
    }
    return *pInstance;
}

// Méthode Update issue de la classe WindowEventObserver
void Mouse::Update(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// On supprime l'évènement le plus vieux
	PopLastEvent();

	// On ne s'intéresse qu'aux évènements propres à la souris
	switch(uMsg)
    {
	case WM_MOUSEMOVE:
	{
		const int x = LOWORD(lParam);
		const int y = HIWORD(lParam);
		OnMouseMove(x, y);
	}
	break;
	case WM_LBUTTONDOWN:
	{
		OnLeftPressed();
	}
	break;
	case WM_LBUTTONUP:
	{
		OnLeftReleased();
	}
	break;
	case WM_MBUTTONDOWN:
	{
		OnMiddlePressed();
	}
	break;
	case WM_MBUTTONUP:
	{
		OnMiddleReleased();
	}
	break;
	case WM_RBUTTONDOWN:
	{
		OnRightPressed();
	}
	break;
	case WM_RBUTTONUP:
	{
		OnRightReleased();
	}
	break;
	case WM_MOUSEWHEEL:
	{
		if (GET_WHEEL_DELTA_WPARAM(wParam) >= 0) OnWheelUp();
		else OnWheelDown();
	}
	break;
    default: break;
    }
}

// Retourne le dernier évènement enregistré
Mouse::EventType Mouse::Read() const
{
    return buffer.empty() ? EventType::None : buffer.front();
}

// Retourne la position de la souris dans la fenêtre
FVec2D Mouse::GetPosition() const
{
    return position - WND.GetCenterOfScreen();
}

// Retourne les états des boutons (Pressé/Relâché)
bool Mouse::LeftIsPressed() const
{
    return bLeftIsPressed;
}

bool Mouse::MiddleIsPressed() const
{
    return bMiddleIsPressed;
}

bool Mouse::RightIsPressed() const
{
    return bRightIsPressed;
}

// Sous-méthodes du traitement d'évènements : A chaque fois, on vérifie si l'on doit réduire la taille du buffer d'évènements
void Mouse::OnMouseMove(int x, int y)
{
    position = IVec2D(x, y);
    buffer.push(EventType::Move);
    TrimBuffer();
}

void Mouse::OnLeftPressed()
{
    bLeftIsPressed = true;
    buffer.push(EventType::LPress);
    TrimBuffer();
}

void Mouse::OnLeftReleased()
{
    bLeftIsPressed = false;
    buffer.push(EventType::LRelease);
    TrimBuffer();
}

void Mouse::OnMiddlePressed()
{
    bMiddleIsPressed = true;
    buffer.push(EventType::MPress);
    TrimBuffer();
}

void Mouse::OnMiddleReleased()
{
    bMiddleIsPressed = false;
    buffer.push(EventType::MRelease);
    TrimBuffer();
}

void Mouse::OnRightPressed()
{
    bRightIsPressed = true;
    buffer.push(EventType::RPress);
    TrimBuffer();
}

void Mouse::OnRightReleased()
{
    bRightIsPressed = false;
    buffer.push(EventType::RRelease);
    TrimBuffer();
}

void Mouse::OnWheelUp()
{
    buffer.push(EventType::WheelUp);
    TrimBuffer();
}

void Mouse::OnWheelDown()
{
    buffer.push(EventType::WheelDown);
    TrimBuffer();
}

// Vide le buffer d'évènements
void Mouse::FlushBuffer()
{
    buffer = std::queue<EventType>();
}

// Supprime l'évènement le plus vieux si l'on dépasse la taille maximale d'évènements
void Mouse::TrimBuffer()
{
    if (buffer.size() > bufferSize) buffer.pop();
}

// Supprime l'évènement le plus vieux
void Mouse::PopLastEvent()
{
    if (!buffer.empty()) buffer.pop();
}
