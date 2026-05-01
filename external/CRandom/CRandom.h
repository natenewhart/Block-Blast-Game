// Nate Newhart - C++ STD Random library wrapper
// All in one header file for ease of access

#pragma once

#include <random>

class CRandom
{
public:
	CRandom();
	void ResetSeed(); // Uses random_device to create new seed for mt19937 generator

	// Range Setting Functions
	void SetRangeDouble(double min, double max);
	void SetRangeFloat (float min, float max);
	void SetRangeUInt64(uint64_t min, uint64_t max);
	void SetRangeInt   (int min, int max);

	void ResetRanges(); // Reset all ranges

	// Random Functions, uses stored range values, OR range can be set within function call using overload
	double   Double();
	double   Double(double min, double max);
	float    Float();
	float    Float(float min, float max);
	uint64_t uInt64();
	uint64_t uInt64(uint64_t min, uint64_t max);
	int      Int();
	int      Int(int min, int max);

private:
	static uint64_t Seed(); // Generate 64 bit seed

private:
	std::mt19937_64 mGenerator; // 64 Bit PRNG

	std::uniform_real_distribution<double>  mDoubleDistribution; // [0, 1]
	std::uniform_real_distribution<float>   mFloatDistribution;  // [0, 1]
	std::uniform_int_distribution<uint64_t> mUInt64Distribution; // [0, 2^64 - 1]
	std::uniform_int_distribution<int>      mIntDistribution;    // [INT_MIN, INT_MAX]
};

// -------------------------------- Inline Function Definitions --------------------------------

inline CRandom::CRandom()
	: mGenerator(Seed())
	, mDoubleDistribution(0.0, 1.0)
	, mFloatDistribution (0.0f, 1.0f)
	, mUInt64Distribution(0, std::numeric_limits<uint64_t>::max())
	, mIntDistribution   (std::numeric_limits<int>::lowest(), std::numeric_limits<int>::max())
{
}

inline void CRandom::ResetSeed()
{
	mGenerator.seed(Seed());
}

inline uint64_t CRandom::Seed()
{
	auto rd = std::random_device{};
	return ((uint64_t)rd() << 32) | rd();
}

inline void CRandom::ResetRanges()
{
	mDoubleDistribution = std::uniform_real_distribution<double>(0.0, 1.0);
	mFloatDistribution  = std::uniform_real_distribution<float>(0.0f, 1.0f);
	mUInt64Distribution = std::uniform_int_distribution<uint64_t>(0, std::numeric_limits<uint64_t>::max());
	mIntDistribution    = std::uniform_int_distribution<int>(std::numeric_limits<int>::lowest(), std::numeric_limits<int>::max());
}

inline void CRandom::SetRangeDouble(double min, double max)
{
	mDoubleDistribution = std::uniform_real_distribution<double>(min, max);
}

inline void CRandom::SetRangeFloat(float min, float max)
{
	mFloatDistribution = std::uniform_real_distribution<float>(min, max);
}

inline void CRandom::SetRangeUInt64(uint64_t min, uint64_t max)
{
	mUInt64Distribution = std::uniform_int_distribution<uint64_t>(min, max);
}

inline void CRandom::SetRangeInt(int min, int max)
{
	mIntDistribution = std::uniform_int_distribution<int>(min, max);
}

inline double CRandom::Double()
{
	return mDoubleDistribution(mGenerator);
}

inline double CRandom::Double(double min, double max)
{
	return std::uniform_real_distribution<double>(min, max)(mGenerator);
}

inline float CRandom::Float()
{
	return mFloatDistribution(mGenerator);
}

inline float CRandom::Float(float min, float max)
{
	return std::uniform_real_distribution<float>(min, max)(mGenerator);
}

inline uint64_t CRandom::uInt64()
{
	return mUInt64Distribution(mGenerator);
}

inline uint64_t CRandom::uInt64(uint64_t min, uint64_t max)
{
	return std::uniform_int_distribution<uint64_t>(min, max)(mGenerator);
}

inline int CRandom::Int()
{
	return mIntDistribution(mGenerator);
}

inline int CRandom::Int(int min, int max)
{
	return std::uniform_int_distribution<int>(min, max)(mGenerator);
}
