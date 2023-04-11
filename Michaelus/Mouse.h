#pragma once
#include <memory>
#include <queue>

#include "Vec2D.h"
#include "WindowEventObserver.h"

#define MOUSE Mouse::GetInstance()

/*
 * Classe enregistrant tous les évènements liés à la souris
 */
class Mouse : public WindowEventObserver
{
public:
	/*
	 * Enumération des évènements
	 * Bouton gauche pressé/relâché, Bouton du milieu pressé/relâché, Bouton droit pressé/relâché
	 * Molette Haut/Bas, Déplacement de la souris, Rien
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
	// Constructeur par défaut
	Mouse() = default;
	// Destructeur
	~Mouse() override;

	// Ces différents constructeurs sont définis comme étant supprimés, pour correspondre au patron de conception Singleton
	// et également à la RO5 des normes du C++
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;
	Mouse(Mouse&&) = delete;
	Mouse& operator=(Mouse&&) = delete;

	// Méthode du patron de conception Singleton
	static Mouse& GetInstance();

	// Méthode Update issue de la classe WindowEventObserver
	virtual void Update(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	// Retourne le dernier évènement enregistré
	Mouse::EventType Read() const;

	// Retourne la position de la souris dans la fenêtre
	FVec2D GetPosition() const;

	// Retourne les états des boutons (Pressé/Relâché)
	bool LeftIsPressed() const;
	bool MiddleIsPressed() const;
	bool RightIsPressed() const;

private:
	// Sous-méthodes du traitement d'évènements
	void OnMouseMove(int x, int y);
	void OnLeftPressed();
	void OnLeftReleased();
	void OnMiddlePressed();
	void OnMiddleReleased();
	void OnRightPressed();
	void OnRightReleased();
	void OnWheelUp();
	void OnWheelDown();

	// Vide le buffer d'évènements
	void FlushBuffer();
	// Supprime l'évènement le plus vieux si l'on dépasse la taille maximale d'évènements
	void TrimBuffer();

	// Supprime l'évènement le plus vieux
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

