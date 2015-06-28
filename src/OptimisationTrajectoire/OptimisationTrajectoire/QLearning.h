#ifndef _PIP_QLEARNING_H_
#define _PIP_QLEARNING_H_

#include <map>
#include "State.h"
#include "Vehicule.h"
#include "Track.h"
#include "Config.h"

// Politique de r�compense
class SpeedAxisReward
{
public:
	SpeedAxisReward(Config& config);
	float GetReward(const Observation& obs, const Command& command, const Command& last, const Vehicule& veh, const Track& track) const;

protected:
	Config& mConfig;
};

// Matrice de qualit�
class Quality
{
public:
	using CommandQuality = std::map<Command, float>;
	using CommandMap = std::map<Command, CommandQuality>;
	using SensorQuality = std::map<Observation, CommandMap>;

	// Associations (�tat, action) pour une observation donn�e
	CommandMap& GetCommandMap(const Observation& obs);
	// Qualit� des actions � partir d'un �tat donn�
	CommandQuality& GetCommandQuality(const Observation& obs, const Command& current);

	// Meilleure r�compense � partir d'un �tat donn�
	float GetBestReward(const Observation& obs, const Command& current, float def = 0);
	// Meilleure commande � partir d'un �tat donn�
	const Command& GetBestCommand(const Observation& obs, const Command& current, const Command& def = Command());

	void UpdateCommandReward(const Observation& obs, const Command& from, const Command& to, float q);

	// Qualit� d'un couple (�tat, action)
	float Get(const Observation& obs, const Command& from, const Command& to) { return mQuality[obs][from][to]; }

private:
	SensorQuality mQuality;
};

enum class StateChoicePolicy { Exploration, Exploitation };
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