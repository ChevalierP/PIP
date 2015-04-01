#include <boost/geometry/geometry.hpp>
#include "Vehicule.h"
#include "Track.h"

/*namespace wykobi
{
	template<typename T>
	inline int intersect_count(const ray<T, 2>& ray, const polygon<T, 2>& polygon)
	{
		if(polygon.size() < 3) return false;
		int count = 0;
		std::size_t j = polygon.size() - 1;
		for(std::size_t i = 0; i < polygon.size(); ++i)
		{
			if(intersect(ray, make_segment(polygon[i], polygon[j])))
				count++;
			j = i;
		}
		return count;
	}

	/*template<typename T>
	inline std::tuple<wykobi::point2d<T>, float> closest_intersection_point(const ray<T, 2>& ray, const polygon<T, 2>& polygon)
	{
		if(polygon.size() < 3) return false;
		int count = 0;
		std::size_t j = polygon.size() - 1;
		for(std::size_t i = 0; i < polygon.size(); ++i)
		{
			if(intersection_point(make_segment(polygon[i], polygon[j]), ray))
				count++;
			j = i;
		}
		return count;
	}
}*/

bool Track::IsInside(point_t pt) const
{
	return boost::geometry::within(pt, mPolygon);
}

void Track::UpdateSensors(Vehicule* veh) const
{
	point_t origin = veh->GetLastPosition();
	Sensors* sensors = veh->GetSensors();
	sensors->Foreach(veh->GetAxis(), [this, &origin](point_t direction) -> float {
		ray_t ray(origin, direction);
		return ray.DistanceTo(mPolygon);
	});
}