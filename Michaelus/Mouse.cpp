#include "Mouse.h"

std::unique_ptr<Mouse> Mouse::pInstance = nullptr;

Mouse::~Mouse()
{
    FlushBuffer();
}

Mouse& Mouse::GetInstance()
{
    if (!pInstance)
        pInstance = std::make_unique<Mouse>();
    return *pInstance;
}

FVec2D Mouse::GetPosition() const
{
    return position;
}

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

void Mouse::FlushBuffer()
{
    buffer = std::queue<EventType>();
}

void Mouse::TrimBuffer()
{
    if (buffer.size() > bufferSize) buffer.pop();
}

void Mouse::PopLastEvent()
{
    if (!buffer.empty()) buffer.pop();
}
