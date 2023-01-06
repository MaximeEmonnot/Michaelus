#pragma once
#include <memory>
#include <queue>

#include "Vec2D.h"

#define MOUSE Mouse::GetInstance()

class Mouse
{
private:
	friend class Window;
public:
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
	Mouse() = default;

	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;
	Mouse(Mouse&&) = delete;
	Mouse& operator=(Mouse&&) = delete;

	~Mouse();

	static Mouse& GetInstance();

	FVec2D GetPosition() const;

	bool LeftIsPressed() const;
	bool MiddleIsPressed() const;
	bool RightIsPressed() const;

private:
	void OnMouseMove(int x, int y);
	void OnLeftPressed();
	void OnLeftReleased();
	void OnMiddlePressed();
	void OnMiddleReleased();
	void OnRightPressed();
	void OnRightReleased();
	void OnWheelUp();
	void OnWheelDown();

	void FlushBuffer();
	void TrimBuffer();

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

