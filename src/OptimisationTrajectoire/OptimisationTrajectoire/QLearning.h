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
	using SensorQuality = std::map<Sensors, CommandMap>;

	const CommandMap& GetCommandMap(const Sensors& sensors) const;
	const CommandQuality& GetCommandQuality(const Sensors& sensors, const Command& current) const;
	float GetBestReward(const Sensors& sensors, const Command& current, float def = 0) const;
	const Command& GetBestCommand(const Sensors& sensors, const Command& current, const Command& def = Command()) const;
	void UpdateCommandReward(const Sensors& sensors, const Command& from, const Command& to, float q);

private:
	CommandMap& GetCommandMap(const Sensors& sensors);
	CommandQuality& GetCommandQuality(const Sensors& sensors, const Command& current);

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