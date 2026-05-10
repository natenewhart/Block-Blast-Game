// Nate Newhart
// DeltaTime.hpp

#pragma once

#include <chrono>

class DeltaTimer
{
public:
	DeltaTimer();

	void ResetClock(); // Set previous time to current chrono time

	float  GetTimeFloat();  // Return seconds between last GetTime call and current as float
	double GetTimeDouble(); // Return seconds between last GetTime call and current as double

private:
	template<typename T>
	T GetDeltaTime();

private:
	std::chrono::high_resolution_clock::time_point prevTime; // The last time measurement
};

// -------------------------------- Inline Function Definitions --------------------------------

inline DeltaTimer::DeltaTimer() : prevTime(std::chrono::high_resolution_clock::now()) {}

inline void DeltaTimer::ResetClock()
{
	prevTime = std::chrono::high_resolution_clock::now();
}

template<typename T>
T DeltaTimer::GetDeltaTime()
{
	auto currentTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<T> deltaTime = currentTime - prevTime;
	prevTime = currentTime;
	return deltaTime.count();
}

inline float DeltaTimer::GetTimeFloat()
{
	return GetDeltaTime<float>();
}

inline double DeltaTimer::GetTimeDouble()
{
	return GetDeltaTime<double>();
}