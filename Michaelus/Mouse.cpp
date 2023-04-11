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
		DefineObservable(pInstance.get()); // On d�finit la classe souris comme �tant un observable de la fen�tre
    }
    return *pInstance;
}

// M�thode Update issue de la classe WindowEventObserver
void Mouse::Update(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// On supprime l'�v�nement le plus vieux
	PopLastEvent();

	// On ne s'int�resse qu'aux �v�nements propres � la souris
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

// Retourne le dernier �v�nement enregistr�
Mouse::EventType Mouse::Read() const
{
    return buffer.empty() ? EventType::None : buffer.front();
}

// Retourne la position de la souris dans la fen�tre
FVec2D Mouse::GetPosition() const
{
    return position - WND.GetCenterOfScreen();
}

// Retourne les �tats des boutons (Press�/Rel�ch�)
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

// Sous-m�thodes du traitement d'�v�nements : A chaque fois, on v�rifie si l'on doit r�duire la taille du buffer d'�v�nements
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

// Vide le buffer d'�v�nements
void Mouse::FlushBuffer()
{
    buffer = std::queue<EventType>();
}

// Supprime l'�v�nement le plus vieux si l'on d�passe la taille maximale d'�v�nements
void Mouse::TrimBuffer()
{
    if (buffer.size() > bufferSize) buffer.pop();
}

// Supprime l'�v�nement le plus vieux
void Mouse::PopLastEvent()
{
    if (!buffer.empty()) buffer.pop();
}
