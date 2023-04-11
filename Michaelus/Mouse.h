#pragma once
#include <memory>
#include <queue>

#include "Vec2D.h"
#include "WindowEventObserver.h"

#define MOUSE Mouse::GetInstance()

/*
 * Classe enregistrant tous les �v�nements li�s � la souris
 */
class Mouse : public WindowEventObserver
{
public:
	/*
	 * Enum�ration des �v�nements
	 * Bouton gauche press�/rel�ch�, Bouton du milieu press�/rel�ch�, Bouton droit press�/rel�ch�
	 * Molette Haut/Bas, D�placement de la souris, Rien
	 */
	enum class EventType
	{
		LPress,
		LRelease,
		MPress,
		MRelease,
		RPress,
		RRelease,
		WheelUp,
		WheelDown,
		Move,
		None
	};

public:
	// Constructeur par d�faut
	Mouse() = default;
	// Destructeur
	~Mouse() override;

	// Ces diff�rents constructeurs sont d�finis comme �tant supprim�s, pour correspondre au patron de conception Singleton
	// et �galement � la RO5 des normes du C++
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;
	Mouse(Mouse&&) = delete;
	Mouse& operator=(Mouse&&) = delete;

	// M�thode du patron de conception Singleton
	static Mouse& GetInstance();

	// M�thode Update issue de la classe WindowEventObserver
	virtual void Update(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	// Retourne le dernier �v�nement enregistr�
	Mouse::EventType Read() const;

	// Retourne la position de la souris dans la fen�tre
	FVec2D GetPosition() const;

	// Retourne les �tats des boutons (Press�/Rel�ch�)
	bool LeftIsPressed() const;
	bool MiddleIsPressed() const;
	bool RightIsPressed() const;

private:
	// Sous-m�thodes du traitement d'�v�nements
	void OnMouseMove(int x, int y);
	void OnLeftPressed();
	void OnLeftReleased();
	void OnMiddlePressed();
	void OnMiddleReleased();
	void OnRightPressed();
	void OnRightReleased();
	void OnWheelUp();
	void OnWheelDown();

	// Vide le buffer d'�v�nements
	void FlushBuffer();
	// Supprime l'�v�nement le plus vieux si l'on d�passe la taille maximale d'�v�nements
	void TrimBuffer();

	// Supprime l'�v�nement le plus vieux
	void PopLastEvent();

private:
	static std::unique_ptr<Mouse> pInstance;

	FVec2D position = FVec2D();
	bool bLeftIsPressed = false;
	bool bMiddleIsPressed = false;
	bool bRightIsPressed = false;
	const size_t bufferSize = 4u;
	std::queue<EventType> buffer;
};

