#ifndef _PIP_VEHICULE_H_
#define _PIP_VEHICULE_H_

#include "State.h"

class Sensors;

class Vehicule
{
public:
	using StateType = std::array<float, 3>;

	Vehicule();

	void AddCommand(const Command& c);
	template<class Itr>
	void AddCommand(Itr begin, Itr end);

	void Sim();
	
	const std::vector<StateType>& GetPositions() const { return mPosition; }

private:
	void System(const StateType& x, StateType& dx, float t);

private:
	std::vector<Command> mCommand;
	std::vector<StateType> mPosition;
	Sensors* mSensors;
	float mGravityCenter;
	float mCurrentTime;
	float mTimeStep;
};

template<class Itr>
void Vehicule::AddCommand(Itr begin, Itr end)
{
	std::copy(begin, end, std::back_inserter(mCommand));
}

#endif // !_PIP_VEHICULE_H_