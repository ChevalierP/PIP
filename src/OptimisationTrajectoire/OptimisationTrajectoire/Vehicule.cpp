#include <boost\numeric\odeint.hpp>
#include "Vehicule.h"

Vehicule::Vehicule() :
mSensors(nullptr), mGravityCenter(1), mCurrentTime(0.f), mTimeStep(0.1f)
{
	mPosition.push_back({0, 0, 0});
}

void Vehicule::AddCommand(const Command& c)
{
	mCommand.push_back(c);
}

void Vehicule::Sim()
{
	using namespace boost::numeric;
	odeint::runge_kutta_dopri5<StateType, float> stepper;
	while(mPosition.size() < mCommand.size())
	{
		using namespace std::placeholders;
		StateType out;
		stepper.do_step(std::bind(&Vehicule::System, this, _1, _2, _3), mPosition.back(), mCurrentTime, out, mTimeStep);
		mPosition.push_back(out);
		mCurrentTime += mTimeStep;
	}
}

void Vehicule::System(const StateType& x, StateType& dx, float t)
{
	const Command& c = mCommand[mPosition.size()];
	float speed, steering;
	std::tie(speed, steering) = c.GetValue();
	dx[0] =  speed*std::cos(x[2] - steering);
	dx[1] =  speed*std::sin(x[2] - steering);
	dx[2] = -speed*std::sin(steering)/mGravityCenter;
}

point_t Vehicule::GetLastPosition() const
{
	const StateType& s = mPosition.back();
	return point_t(s[0], s[1]);
}

float Vehicule::GetAxis() const
{
	const StateType& s = mPosition.back();
	return s[2];
}