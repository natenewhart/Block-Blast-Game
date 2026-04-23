#include "Deltatime.h"

DeltaTimeCalculator::DeltaTimeCalculator() : prevTime(std::chrono::high_resolution_clock::now()) {}

float DeltaTimeCalculator::GetDeltaTime()
{
	auto currentTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> deltaTime = currentTime - prevTime;
	prevTime = currentTime;
	return deltaTime.count();
}