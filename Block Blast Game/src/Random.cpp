#include "Random.h"

Random32::Random32()
	: m_generator(std::random_device{}())
	, m_doubleDistrib(0.0, 1.0)
	, m_floatDistrib(0.0f, 1.0f)
	, m_int32Distrib(0, std::numeric_limits<uint32_t>::max())
{}

void Random32::ResetSeed()
{
	m_generator.seed(std::random_device{}());
}

void Random32::SetRangeUInt32(const uint32_t min, const uint32_t max)
{
	m_int32Distrib = std::uniform_int_distribution<uint32_t>(min, max);
}

void Random32::ResetRange()
{
	m_int32Distrib = std::uniform_int_distribution<uint32_t>(0, std::numeric_limits<uint32_t>::max());
}

double Random32::Double()
{
	return m_doubleDistrib(m_generator);
}

float Random32::Float()
{
	return m_floatDistrib(m_generator);
}

uint32_t Random32::uInt32()
{
	return m_int32Distrib(m_generator);
}