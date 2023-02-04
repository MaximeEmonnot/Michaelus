#include "Keyboard.h"

std::unique_ptr<Keyboard> Keyboard::pInstance = nullptr;

Keyboard::~Keyboard()
{
	FlushBuffers();
}

Keyboard& Keyboard::GetInstance()
{
	if (!pInstance)
		pInstance = std::make_unique<Keyboard>();
	return *pInstance;
}

bool Keyboard::KeyIsPressed(unsigned char keycode) const
{
	return keyStates[keycode];
}

Keyboard::Event Keyboard::ReadKey() const
{
	return eventBuffer.empty() ? Event() : eventBuffer.front();
}

char Keyboard::ReadChar() const
{
	return charBuffer.empty() ? 0 : charBuffer.front();
}

bool Keyboard::KeyIsEmpty() const
{
	return eventBuffer.empty();
}

void Keyboard::OnKeyPressed(unsigned char keycode)
{
	keyStates[keycode] = true;
	eventBuffer.push({ keycode, Event::Type::Pressed });
	TrimBuffer(eventBuffer);
}

void Keyboard::OnKeyReleased(unsigned char keycode)
{
	keyStates[keycode] = false;
	eventBuffer.push({ keycode, Event::Type::Released });
	TrimBuffer(eventBuffer);
}

void Keyboard::OnChar(char keycode)
{
	charBuffer.push(keycode);
	TrimBuffer(charBuffer);
}

void Keyboard::FlushBuffers()
{
	eventBuffer = std::queue<Event>();
	charBuffer = std::queue<char>();
}

template <typename T>
void Keyboard::TrimBuffer(std::queue<T>& buffer)
{
	if (buffer.size() > bufferSize) buffer.pop();
}

void Keyboard::PopLastEvents()
{
	if (!eventBuffer.empty()) eventBuffer.pop();
	if (!charBuffer.empty()) charBuffer.pop();
}