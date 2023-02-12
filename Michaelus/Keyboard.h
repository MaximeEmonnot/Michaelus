#pragma once
#include <memory>
#include <bitset>
#include <queue>

#include "WindowEventObserver.h"

#define KBD Keyboard::GetInstance()

class Keyboard : public WindowEventObserver
{
public:
	struct Event
	{
		enum class Type
		{
			Pressed,
			Released,
			None
		};
		unsigned char keycode = 0u;
		Type type = Type::None;
	};

public:
	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;
	Keyboard(Keyboard&&) = delete;
	Keyboard& operator=(Keyboard&&) = delete;

	~Keyboard() override;

	static Keyboard& GetInstance();

	virtual void Update(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	bool KeyIsPressed(unsigned char keycode) const;
	Keyboard::Event ReadKey() const;
	char ReadChar() const;
	bool KeyIsEmpty() const;

private:
	void OnKeyPressed(unsigned char keycode);
	void OnKeyReleased(unsigned char keycode);
	void OnChar(char keycode);

	void FlushBuffers();
	template <typename T>
	void TrimBuffer(std::queue<T>& buffer);

	void PopLastEvents();

private:
	static std::unique_ptr<Keyboard> pInstance;

	const unsigned int bufferSize = 4u;
	std::bitset<256> keyStates;
	std::queue<Event> eventBuffer;
	std::queue<char> charBuffer;
};

