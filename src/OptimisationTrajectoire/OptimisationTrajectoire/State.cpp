#include <algorithm>
#include <iterator>
#include <tuple>
#include "State.h"

Command::Command(float speed, float streering) : mSpeed(speed), mSteering(streering) {}

Command& Command::operator=(Command rhs)
{
	std::swap(*this, rhs);
	return *this;
}

bool Command::operator<(const Command& rhs) const
{
	return std::tie(mSpeed, mSteering) < std::tie(rhs.mSpeed, rhs.mSteering);
}

std::tuple<float, float> Command::GetValue() const
{
	return std::make_tuple(mSpeed, mSteering);
}

const float Sensors::Front = 0;
const float Sensors::Left = (float)M_PI/2;
const float Sensors::Right = -(float)M_PI/2;

Sensors::Sensors(std::initializer_list<float> angles)
{
	std::copy(angles.begin(), angles.end(), std::back_inserter(mAngles));
}

bool Sensors::operator<(const Sensors& rhs) const
{
	return Compare(&rhs);
}

bool Sensors::Compare(const Sensors* rhs) const
{
	return std::lexicographical_compare(mDistances.begin(), mDistances.end(), rhs->mDistances.begin(), rhs->mDistances.end());
}

DiscreteSectorPolicy::DiscreteSectorPolicy(std::initializer_list<float> limits)
{
	std::copy(limits.begin(), limits.end(), std::back_inserter(mLimits));
}

DiscreteSectorPolicy::IndexType DiscreteSectorPolicy::Convert(float distance)
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