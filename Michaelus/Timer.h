#pragma once
#include <memory>
#include <chrono>

#define TICKCLOCK Timer::GetInstance().Update()

#define DELTATIME Timer::GetInstance().DeltaTime()

class Timer
{
public:
	Timer();
	Timer(const Timer&) = delete;
	Timer& operator= (const Timer&) = delete;
	Timer(Timer&&) = delete;
	Timer& operator= (Timer&&) = delete;

	static Timer& GetInstance();

	void Update();

	float DeltaTime() const;

private:
	static std::unique_ptr<Timer> pInstance;

	std::chrono::steady_clock::time_point last;
	float deltaTime;
};

