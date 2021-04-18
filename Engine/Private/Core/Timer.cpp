#include "Core/Timer.h"

FTimer::FTimer()
{
	Reset();
}

float FTimer::GetSecondsElapsed()
{
	constexpr float MillisecondsToSeconds = 1e-3f;
	return GetMillisecondsElapsed() * MillisecondsToSeconds;
}

float FTimer::GetMillisecondsElapsed()
{
	using namespace std::chrono;
	duration<float> Delta = FClock::now() - m_StartTime;
	return static_cast<float>(duration_cast<milliseconds>(Delta).count());
}

void FTimer::Reset()
{
	m_StartTime = FClock::now();
}
