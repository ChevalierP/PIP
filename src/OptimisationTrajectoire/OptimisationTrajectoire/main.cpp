#include <tuple>
#include <iostream>
#include <fstream>
#include <math.h>
#include "State.h"
#include "Vehicule.h"
#include "Track.h"
#include "Geometry.h"
#include "QLearning.h"

#include <boost\geometry.hpp>

int main()
{
	/*std::vector<point_t> pt = {{1, 1}, {1, -1}, {-1, -1}, {-1, 1}, {1, 1}};
	polygon_t p;
	boost::geometry::assign_points(p, pt);
	const polygon_t& pp = p;
	boost::geometry::for_each_segment(pp, [](boost::geometry::model::referring_segment<const point_t> s) {
		std::cout << "(" << s.first.x() << "," << s.first.y() << ") - (" << s.second.x() << "," << s.second.y() << ")" << std::endl;
	});*/


	/*wykobi::vector2d<float> getTrackAxis(PointType p)
	{
		wykobi::vector2d<float> v = wykobi::make_vector(-p.y, p.x);
		return v;
	}*/
	
	TrackLine tl(20000);
	StateSpace ss({0, 20, 10}, {-0.785f, 0.785f, 10}, {1, 2, 3});
	Sensors sensors(&ss, {Sensors::Left, Sensors::Front, Sensors::Right});
	Vehicule veh(&sensors);
	veh.AddCommand(std::make_tuple<float, float>(20, 0));
	Quality q;
	SpeedAxisReward rp;
	QLearning<SpeedAxisReward> ql(ss, q, veh, tl, rp);
	for(int i(0); i<10000; i++)
		ql.Sim({1, 0, 0}, std::make_tuple(5.f, 0.f));

	std::ofstream f("C:\\Users\\Eisenheim\\Desktop\\a");

	veh.Reset({1, 0, 0});
	veh.AddCommand(std::make_tuple(5.f, 0.f));
	for(int i(0); i<1000; i++)
	{
		const point_t pt = veh.GetLastPosition();
		f << pt.x() << "," << pt.y() << std::endl;

		tl.UpdateSensors(&veh);
		const Command& c = q.GetBestCommand(veh.GetSensors()->GetObservation(), veh.GetLastCommand());
		veh.AddCommand(c);
		veh.Sim();
	}
	f.close();
	return 0;
}

