#include <algorithm>
#include <iterator>
#include <tuple>
#include "State.h"

Linspace::Linspace(float start, float end, int n) :
mStart(start), mEnd(end), mCount(n - 1), mGen(std::random_device()), mRand(0, n - 1)
{
	
}

float Linspace::GetRandomValue() const
{
	int i = mRand(mGen);
	return mStart + i*(mEnd - mStart)/mCount;
}

DiscreteSector::DiscreteSector(std::initializer_list<float> limits)
{
	std::copy(limits.begin(), limits.end(), std::back_inserter(mLimits));
}

int DiscreteSector::Convert(float distance)
{
	int index = 0;
	for(float l : mLimits)
	{
		if(distance < l)
			break;
		index++;
	}
	return index;
}

StateSpace::StateSpace(const Linspace& speed, const Linspace& steering, const DiscreteSector& distance) :
mSpeedSpace(speed), mSteeringSpace(steering), mDistanceSpace(distance)
{

}

Command StateSpace::GenRandomCommand() const
{
	return Command(mSpeedSpace.GetRandomValue(), mSteeringSpace.GetRandomValue());
}

Sensors::Sensors(StateSpace* ss, std::initializer_list<float> angles) :
mStateSpace(ss)
{
	std::copy(angles.begin(), angles.end(), std::back_inserter(mAngles));
	mDistances.resize(mAngles.size());
	mDiscreteDistances.resize(mAngles.size());
}

const float Sensors::Front = 0;
const float Sensors::Left = (float)M_PI/2;
const float Sensors::Right = -(float)M_PI/2;

bool Sensors::operator<(const Sensors& rhs) const
{
	return mDiscreteDistances < rhs.mDiscreteDistances;
}

void Sensors::Foreach(float axis, std::function<float(point_t)> f)
{
	Container::const_iterator angit;
	Container::iterator distit;
	for(angit = mAngles.begin(), distit = mDistances.begin(); angit != mAngles.end() && distit != mDistances.end(); angit++, distit++)
		*distit = f(point_t(std::cos(axis + *angit), std::sin(axis + *angit)));
}