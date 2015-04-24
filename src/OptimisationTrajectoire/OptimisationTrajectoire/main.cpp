#include <tuple>
#include <iostream>
#include <math.h>
#include "State.h"
#include "Vehicule.h"
#include "Track.h"
#include "Geometry.h"

#include <wykobi\wykobi.hpp>
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
	tl.UpdateSensors(&veh);
	for(float d : sensors.GetDistances())
		std::cout << d << " ";
	std::cout << std::endl;
	for(int i : sensors.GetObservation())
		std::cout << i << " ";
	std::cout << std::endl;
	return 0;
}

