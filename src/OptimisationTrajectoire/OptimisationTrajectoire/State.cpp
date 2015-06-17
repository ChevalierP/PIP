#include <algorithm>
#include <iterator>
#include <tuple>
#include "State.h"

std::random_device Linspace::mRD;

Linspace::Linspace(float start, float end, int n) :
mStart(start), mEnd(end), mCount(n), mStep((end - start)/(n - 1)), mGen(mRD()), mRand(0, n - 1)
{
	
}

float Linspace::GetRandomValue()
{
	int i = mRand(mGen);
	return mStart + i*mStep;
}

float Linspace::GetConstrainedValue(float min_, float max_)
{
	int minidx = static_cast<int>(std::ceil((min_ - mStart)/mStep));
	int maxidx = static_cast<int>(std::floor((max_ - mStart)/mStep));
	std::uniform_int_distribution<int> r(minidx, maxidx);
	int i = r(mGen);
	float value = mStart + i*mStep;
	return std::max(mStart, std::min(value, mEnd));
}

DiscreteSector::DiscreteSector(std::initializer_list<float> limits)
{
	std::copy(limits.begin(), limits.end(), std::back_inserter(mLimits));
}

int DiscreteSector::Convert(float distance) const
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

Command StateSpace::GenRandomCommand()
{
	return Command(mSpeedSpace.GetRandomValue(), mSteeringSpace.GetRandomValue());
}

Command StateSpace::GenConstrainedCommand(const Command& last)
{
	float maxsteer = (1 - std::get<speed>(last)/(mSpeedSpace.max() + 1))*mSteeringSpace.max();
	return Command(mSpeedSpace.GetRandomValue(), mSteeringSpace.GetConstrainedValue(-maxsteer, maxsteer));
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
const float Sensors::FrontLeft = (float)M_PI/4;
const float Sensors::FrontRight = -(float)M_PI/4;

bool Sensors::operator<(const Sensors& rhs) const
{
	return mDiscreteDistances < rhs.mDiscreteDistances;
}

void Sensors::Foreach(float axis, std::function<float(point_t)> f)
{
	Container::const_iterator angit = mAngles.begin();
	Container::iterator distit = mDistances.begin();
	Observation::iterator obsit = mDiscreteDistances.begin();
	for(; angit != mAngles.end() && distit != mDistances.end() && obsit != mDiscreteDistances.end(); angit++, distit++, obsit++)
	{
		*distit = f(point_t(std::cos(axis + *angit), std::sin(axis + *angit)));
		*obsit = mStateSpace->GetDistanceSpace().Convert(*distit);
	}
}