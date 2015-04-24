#include "State.h"
#include "QLearning.h"

float SpeedAxisReward::GetReward(const Observation& obs, const Command& command) const
{
	return std::get<0>(command);
}

Quality::CommandMap& Quality::GetCommandMap(const Observation& obs)
{
	SensorQuality::iterator it = mQuality.find(obs);
	if(it != mQuality.end())
		return it->second;
	mQuality[obs] = CommandMap();
	return mQuality[obs];
}

Quality::CommandQuality& Quality::GetCommandQuality(const Observation& obs, const Command& current)
{
	CommandMap& cm = GetCommandMap(obs);
	CommandMap::iterator it = cm.find(current);
	if(it != cm.end())
		return it->second;
	cm[current] = CommandQuality();
	return cm[current];
}

float Quality::GetBestReward(const Observation& obs, const Command& current, float def)
{
	CommandQuality& cq = GetCommandQuality(obs, current);
	CommandQuality::const_iterator maxit = std::max_element(cq.begin(), cq.end(),
		[](const CommandQuality::value_type& lhs, const CommandQuality::value_type& rhs) -> bool {
		return lhs.second < rhs.second;
	});
	if(maxit == cq.end())
		return def;
	return maxit->second;
}

const Command& Quality::GetBestCommand(const Observation& obs, const Command& current, const Command& def)
{
	CommandQuality& cq = GetCommandQuality(obs, current);
	CommandQuality::const_iterator maxit = std::max_element(cq.begin(), cq.end(),
		[](const CommandQuality::value_type& lhs, const CommandQuality::value_type& rhs) -> bool {
		return lhs.second < rhs.second;
	});
	if(maxit == cq.end())
		return def;
	return maxit->first;
}

void Quality::UpdateCommandReward(const Observation& obs, const Command& from, const Command& to, float q)
{
	CommandMap& cm = GetCommandMap(obs);
	cm[from][to] = q;
}