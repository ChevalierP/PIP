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
	boost::geometry::for_each_segment(p, [](segment_t s) {
		std::cout << "(" << s.first.x() << "," << s.first.y() << ") - (" << s.second.x() << "," << s.second.y() << ")" << std::endl;
	});*/


	/*wykobi::vector2d<float> getTrackAxis(PointType p)
	{
		wykobi::vector2d<float> v = wykobi::make_vector(-p.y, p.x);
		return v;
	}*/

	TrackLine tl(20);
	std::cout << std::boolalpha << tl.IsInside(point_t(19.5, -2.4)) << std::endl;
	return 0;
}

