// Nate Newhart
// deltatime.hpp

#pragma once

#include <chrono>

class DeltaTimeCalculator
{
public:
	DeltaTimeCalculator();

	float GetDeltaTime(); // Return seconds between last frame and current

private:
	std::chrono::high_resolution_clock::time_point prevTime; // The last time measurement
};