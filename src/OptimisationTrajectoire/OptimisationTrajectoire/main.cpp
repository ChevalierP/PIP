#include <tuple>
#include <iostream>
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
	
	TrackLine tl(20);
	StateSpace ss({0, 20, 5}, {-3.14f, 3.14f, 5}, {1});
	Sensors sensors(&ss, {Sensors::Left, Sensors::Front, Sensors::Right});
	Vehicule veh(&sensors);
	veh.AddCommand(std::make_tuple<float, float>(20, 0));
	Quality q;
	SpeedAxisReward rp;
	QLearning<SpeedAxisReward> ql(ss, q, veh, tl, rp);
	ql.Sim();
	return 0;
}

