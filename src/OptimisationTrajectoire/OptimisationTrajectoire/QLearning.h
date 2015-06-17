#ifndef _PIP_QLEARNING_H_
#define _PIP_QLEARNING_H_

#include <map>
#include "State.h"
#include "Vehicule.h"
#include "Track.h"
#include "Config.h"

class SpeedAxisReward
{
public:
	SpeedAxisReward(Config& config);
	float GetReward(const Observation& obs, const Command& command, const Command& last, const Vehicule& veh, const Track& track) const;

protected:
	Config& mConfig;
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


enum class StateChoicePolicy { Exploration, Exploitation};
template<class T>
class QLearning
{
public:
	QLearning(Config& config, StateSpace& ss, Quality& quality, Vehicule& veh, const Track& track, const T& rewardPolicy);

	bool Sim(const Vehicule::StateType& position, const Command& command, StateChoicePolicy scp);

private:
	Config& mConfig;
	StateSpace& mStateSpace;
	Quality& mQuality;
	Vehicule& mVehicule;
	const Track& mTrack;
	const T& mRewardPolicy;
};

#include "QLearning.inl"

#endif // !_PIP_QLEARNING_H_