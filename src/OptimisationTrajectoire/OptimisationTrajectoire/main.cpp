#include <tuple>
#include <iostream>
#include <fstream>
#include <math.h>
#include "State.h"
#include "Vehicule.h"
#include "Track.h"
#include "Geometry.h"
#include "QLearning.h"
#include "Config.h"
#include <engine.h>

#include <boost\geometry.hpp>

int main()
{
	Config config;
	config
		.gamma(.9f)
		.alpha(.6f)
		.steeringCostFactor(.0f)
		.rewardSteeringChange(true)
		.skidCost(0)
		.rewardSkid(true)
		.offTrackCost(-100)
		.rewardOffTrack(true)
		.finishLineReward(100)
		.rewardFinish(true)
		.attemptCount(300000)
		.maxIterationCount(1000)
		.rollingStart(true)
		.coefficientOfFriction(.6f)
		.accelerationOfGravity(9.81f);

	//TrackLine tl(20);
	//TrackTurn tl(20);
	SoftTurn tl(20);
	StateSpace ss({0, 20, 5}, {-0.5f, 0.5f, 31}, {.5f, 1.f, 2.f, 4.f, 2000.f});
	Sensors sensors(&ss, {Sensors::Left, Sensors::Front, Sensors::Right});
	Vehicule veh(&sensors);
	Quality q;
	SpeedAxisReward rp(config);
	QLearning<SpeedAxisReward> ql(config, ss, q, veh, tl, rp);

	Engine* ep = engOpen(nullptr);

	int niter = config.attemptCount();
	for(int i(0); i<niter; i++)
	{
		StateChoicePolicy scp = StateChoicePolicy::Exploration;
		if((i/1000)%50 == 0)
		{
			scp = StateChoicePolicy::Exploitation;
			std::cout << "exploitation" << std::endl;
		}

		if(ql.Sim({config.rollingStart() ? (21*(float)i/niter+0.01f) : 1, 0, 0}, std::make_tuple(20.f, 0.1f), scp)) //22*(float)i/niter+0.01f
			std::cout << "finished" << std::endl;
		if(i%1000 == 0)
			std::cout << 100*((double)i/niter) << " %" << std::endl;
		/*std::ofstream trace("C:\\Users\\Eisenheim\\Desktop\\trace");
		const std::vector<Vehicule::StateType>& pos = veh.GetPositions();
		for(const Vehicule::StateType& st : pos)
			trace << st[0] << "," << st[1] << std::endl;
		trace.close();
		engEvalString(ep, "trace = csvread('C:\\Users\\Eisenheim\\Desktop\\trace');");
		engEvalString(ep, "plot(trace(:,1), trace(:,2));");
		system("pause");*/
	}

	std::ofstream f("C:\\Users\\Eisenheim\\Desktop\\a");

	SoftTurn st(20);
	veh.Reset({1, 0, 0});
	veh.AddCommand(std::make_tuple(20.f, 0.f));
	for(int i(0); i<1000; i++)
	{
		const point_t pt = veh.GetLastPosition();
		f << pt.x() << "," << pt.y() << ",";

		st.UpdateSensors(&veh);
		const Command& c = q.GetBestCommand(veh.GetSensors()->GetObservation(), veh.GetLastCommand(), veh.GetLastCommand());
		const Observation& obs = veh.GetSensors()->GetObservation();
		f << std::get<0>(c) << "," << std::get<1>(c) << "," << obs[0] << "," << obs[1] << "," << obs[2] << std::endl;

		veh.AddCommand(c);
		veh.Sim();
		if(st.HasFinished(veh.GetLastPosition()))
			break;
	}
	f.close();

	engEvalString(ep, "a = csvread('C:\\Users\\Eisenheim\\Desktop\\a');");
	engEvalString(ep, "plot(a(:,1), a(:,2));figure;plot(a(:, 4));figure;plot(a(:, 3), 'r');");
	engEvalString(ep, "figure;plot(a(:, 5));hold on;plot(a(:, 6), 'r');hold on;plot(a(:, 7), 'g');");
	system("pause");
	engClose(ep);

	return 0;
}