#include "State.h"
#include "QLearning.h"

float SpeedAxisReward::GetReward(const Sensors& sensors, const Command& command)
{
	return 0;
}

const Quality::CommandMap& Quality::GetCommandMap(const Sensors& sensors) const
{
	SensorQuality::const_iterator it = mQuality.find(sensors);
	if(it != mQuality.end())
		return it->second;
	return CommandMap();
}

Quality::CommandMap& Quality::GetCommandMap(const Sensors& sensors)
{
	SensorQuality::iterator it = mQuality.find(sensors);
	if(it != mQuality.end())
		return it->second;
	return CommandMap();
}

const Quality::CommandQuality& Quality::GetCommandQuality(const Sensors& sensors, const Command& current) const
{
	const CommandMap& cm = GetCommandMap(sensors);
	CommandMap::const_iterator it = cm.find(current);
	if(it != cm.end())
		return it->second;
	return CommandQuality();
}

Quality::CommandQuality& Quality::GetCommandQuality(const Sensors& sensors, const Command& current)
{
	CommandMap& cm = GetCommandMap(sensors);
	CommandMap::iterator it = cm.find(current);
	if(it != cm.end())
		return it->second;
	return CommandQuality();
}

float Quality::GetBestReward(const Sensors& sensors, const Command& current, float def) const
{
	const CommandQuality& cq = GetCommandQuality(sensors, current);
	CommandQuality::const_iterator maxit = std::max_element(cq.begin(), cq.end(),
															[](const CommandQuality::value_type& lhs, const CommandQuality::value_type& rhs) -> bool {
		return lhs.second < rhs.second;
	});
	if(maxit == cq.end())
		return def;
	return maxit->second;
}

const Command& Quality::GetBestCommand(const Sensors& sensors, const Command& current, const Command& def) const
{
	const CommandQuality& cq = GetCommandQuality(sensors, current);
	CommandQuality::const_iterator maxit = std::max_element(cq.begin(), cq.end(),
		[](const CommandQuality::value_type& lhs, const CommandQuality::value_type& rhs) -> bool {
			return lhs.second < rhs.second;
	});
	if(maxit == cq.end())
		return def;
	return maxit->first;
}

void Quality::UpdateCommandReward(const Sensors& sensors, const Command& from, const Command& to, float q)
{
	CommandMap& cm = GetCommandMap(sensors);
	cm[from][to] = q;
}