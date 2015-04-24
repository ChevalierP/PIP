#ifndef _PIP_QLEARNING_H_
#define _PIP_QLEARNING_H_

#include <map>
#include "State.h"
#include "Vehicule.h"
#include "Track.h"

class SpeedAxisReward
{
public:
	float GetReward(const Sensors& sensors, const Command& command);

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

private:
	SensorQuality mQuality;
};

template<class T>
class QLearning
{
public:
	QLearning(Quality& quality, Vehicule& veh, const Track& track, const T& rewardPolicy);

	void Sim();

private:
	Quality& mQuality;
	Vehicule& mVehicule;
	const Track& mTrack;
	const T& mRewardPolicy;
};

#include "QLearning.inl"

#endif // !_PIP_QLEARNING_H_