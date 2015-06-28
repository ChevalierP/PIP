#ifndef _PIP_CONFIG_H_
#define _PIP_CONFIG_H_

#define CONFIG_VAR(T, name) \
	public: \
		T name() const { return m_##name; } \
		Config& name(T name##_) { m_##name = name##_; return *this; } \
	private: \
		T m_##name;

// Paramètres de la simulation
class Config
{
	CONFIG_VAR(float, gamma);
	CONFIG_VAR(float, alpha);
	CONFIG_VAR(float, steeringCostFactor);
	CONFIG_VAR(int, attemptCount);
	CONFIG_VAR(int, maxIterationCount);
	CONFIG_VAR(float, finishLineReward);
	CONFIG_VAR(float, offTrackCost);
	CONFIG_VAR(float, skidCost);
	CONFIG_VAR(bool, rewardFinish);
	CONFIG_VAR(bool, rewardSkid);
	CONFIG_VAR(bool, rewardOffTrack);
	CONFIG_VAR(bool, rewardSteeringChange);
	CONFIG_VAR(bool, rollingStart);
	CONFIG_VAR(float, coefficientOfFriction);
	CONFIG_VAR(float, accelerationOfGravity);
	CONFIG_VAR(bool, offTrackRewardBackpropagation);
};

#endif // !_PIP_CONFIG_H_