#ifndef _PIP_QLEARNING_H_
#define _PIP_QLEARNING_H_

#include <map>
#include "State.h"
#include "Vehicule.h"
#include "Track.h"

class SpeedAxisReward
{
public:
	float GetReward(const Observation& obs, const Command& command, const Vehicule& veh, const Track& track) const;

protected:

};

class Quality
{
public:
	using CommandQuality = std::map<Command, float>;
	using CommandMap = std::map<Command, CommandQuality>;
	using SensorQuality = std::map<Observation, CommandMap>;

	CommandMap& GetCommandMap(const Observation& obs);
	CommandQuality& GetCommandQuality(const Observation& obs, const Command& current);
	float GetBestReward(const Observation& obs, const Command& current, float def = 0);
	const Command& GetBestCommand(const Observation& obs, const Command& current, const Command& def = Command());
	void UpdateCommandReward(const Observation& obs, const Command& from, const Command& to, float q);
	float Get(const Observation& obs, const Command& from, const Command& to) { return mQuality[obs][from][to]; }

private:
	SensorQuality mQuality;
};

template<class T>
class QLearning
{
public:
	QLearning(StateSpace& ss, Quality& quality, Vehicule& veh, const Track& track, const T& rewardPolicy);

	void Sim(const Vehicule::StateType& position, const Command& command);

private:
	StateSpace& mStateSpace;
	Quality& mQuality;
	Vehicule& mVehicule;
	const Track& mTrack;
	const T& mRewardPolicy;
};

#include "QLearning.inl"

#endif // !_PIP_QLEARNING_H_