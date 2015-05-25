#include <tuple>
#include <iostream>
#include <fstream>
#include <math.h>
#include "State.h"
#include "Vehicule.h"
#include "Track.h"
#include "Geometry.h"
#include "QLearning.h"
#include <engine.h>

#include <boost\geometry.hpp>

int main()
{	
	//TrackLine tl(50);
	TrackTurn tl(25);
	StateSpace ss({0, 20, 5}, {-0.2f, 0.2f, 31}, {0.5f, 1.f, 2.f}); // {-0.785f, 0.785f, 5}
	Sensors sensors(&ss, {Sensors::Left, Sensors::Front, Sensors::Right});
	Vehicule veh(&sensors);
	veh.AddCommand(std::make_tuple<float, float>(20, 0));
	Quality q;
	SpeedAxisReward rp;
	rp.steeringCostFactor(0);
	QLearning<SpeedAxisReward> ql(ss, q, veh, tl, rp);
	ql.gamma(.9f).alpha(.6f);

	for(int i(0); i<10000; i++)
		ql.Sim({1, 0, 0}, std::make_tuple(5.f, 0.f));

	std::ofstream f("C:\\Users\\Eisenheim\\Desktop\\a");

	veh.Reset({1, 0, 0});
	veh.AddCommand(std::make_tuple(5.f, 0.f));
	for(int i(0); i<1000; i++)
	{
		const point_t pt = veh.GetLastPosition();
		f << pt.x() << "," << pt.y() << ",";

		tl.UpdateSensors(&veh);
		const Command& c = q.GetBestCommand(veh.GetSensors()->GetObservation(), veh.GetLastCommand(), veh.GetLastCommand());
		f << std::get<0>(c) << "," << std::get<1>(c) << std::endl;

		veh.AddCommand(c);
		veh.Sim();
	}
	f.close();

	Engine* ep = engOpen(nullptr);
	engEvalString(ep, "a = csvread('C:\\Users\\Eisenheim\\Desktop\\a');");
	engEvalString(ep, "plot(a(:,1), a(:,2));figure;plot(a(:, 4));figure;plot(a(:, 3), 'r');");
	system("pause");
	engClose(ep);

	return 0;
}