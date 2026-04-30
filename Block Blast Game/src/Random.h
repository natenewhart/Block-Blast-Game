// Nate Newhart - C++ STD Random library wrapper

#pragma once

#include <random>

class Random32
{
public:
	Random32();

	void ResetSeed(); // Uses random_device to create new seed for mt19937 generator

	void SetRangeUInt32(const uint32_t min, const uint32_t max);
	void ResetRange(); // Reset Random Range to (0, uint32_t max)

	double Double();   // Return double between [0, 1]
	float  Float();    // Return float between [0, 1]
	uint32_t uInt32(); // Return uint32_t between [min, max], default is [0, 2^32 - 1]

private:
	std::mt19937 m_generator; // PRNG Generator

	// Distributions, uses default values unless changed by user
	std::uniform_real_distribution<double>  m_doubleDistrib; // [0, 1]
	std::uniform_real_distribution<float>   m_floatDistrib;  // [0, 1]
	std::uniform_int_distribution<uint32_t> m_int32Distrib;  // [0, 2^32 - 1]
};