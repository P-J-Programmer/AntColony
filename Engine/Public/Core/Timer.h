#pragma once

#include <chrono>

class FTimer
{
public:
	FTimer();

public:
	float GetSecondsElapsed();
	float GetMillisecondsElapsed();
	void Reset();

private:
	using FClock = std::chrono::high_resolution_clock;

private:
	FClock::time_point m_StartTime;

};