#include <tuple>
#include <iostream>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <wykobi\wykobi.hpp>
#include <math.h>
#include "State.h"
#include "Vehicule.h"

template<unsigned int N> inline double meta_sqrt() { return sqrt(N); }

int main()
{
	Vehicule veh;
	for(int i(0); i<100; i++)
		veh.AddCommand(Command(1, 1));
	veh.Sim();
/*	wykobi::intersection_point(ray, cir, std::back_inserter(n));
	for(wykobi::point2d<float> i : n)
		std::cout << i.x << " " << i.y << std::endl;*/
	const std::vector<Vehicule::StateType>& p = veh.GetPositions();
	for(Vehicule::StateType s : p)
		std::cout << s[0] << "," << s[1] << std::endl;

	wykobi::vector2d<float> getTrackAxis(PointType p)
	{
		wykobi::vector2d<float> v = wykobi::make_vector(-p.y, p.x);
		return v;
	}


	return 0;
}

