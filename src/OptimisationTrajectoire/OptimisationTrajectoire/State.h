#ifndef _PIP_STATE_H_
#define _PIP_STATE_H_

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <functional>
#include <random>
#include "Geometry.h"

// Speed and steering
static const int speed = 0;
static const int steering = 1;
using Command = std::tuple<float, float>;

// Distances from sensors
using Observation = std::vector<int>;

class Linspace
{
public:
	Linspace(float start, float end, int n);

	float GetRandomValue();
	float GetConstrainedValue(float min_, float max_);

	float max() const { return mEnd; }
	float min() const { return mStart; }

private:
	float mStart;
	float mEnd;
	int mCount;
	float mStep;
	static std::random_device mRD;
	std::mt19937 mGen;
	std::uniform_int_distribution<int> mRand;
};

class DiscreteSector
{
public:
	using Container = std::vector<float>;

	DiscreteSector(std::initializer_list<float> limits);

	int Convert(float distance) const;

	const Container& GetLimits() const { return mLimits; }

private:
	Container mLimits;
};

class StateSpace
{
public:
	StateSpace(const Linspace& speed, const Linspace& steering, const DiscreteSector& distance);

	Command GenRandomCommand();
	Command GenConstrainedCommand(const Command& last);
	const DiscreteSector& GetDistanceSpace() const { return mDistanceSpace; }

private:
	Linspace mSpeedSpace;
	Linspace mSteeringSpace;
	DiscreteSector mDistanceSpace;
};

class Sensors
{
public:
	static const float Front;
	static const float Right;
	static const float Left;

	using Container = std::vector<float>;

	Sensors(StateSpace* ss, std::initializer_list<float> angles);

	bool operator<(const Sensors& rhs) const;
	
	void Foreach(float axis, std::function<float(point_t)> f);

	const Container& GetDistances() const { return mDistances; }
	const Observation& GetObservation() const { return mDiscreteDistances; }

protected:
	StateSpace* mStateSpace;
	Container mAngles;
	Container mDistances;
	Observation mDiscreteDistances;

};

#endif // !_PIP_STATE_H_