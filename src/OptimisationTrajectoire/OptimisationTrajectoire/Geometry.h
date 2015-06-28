#ifndef _PIP_GEOMETRY_H_
#define _PIP_GEOMETRY_H_

#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/segment.hpp>


using point_t = boost::geometry::model::d2::point_xy<float>;
using segment_t = boost::geometry::model::segment<point_t>;
using ref_segment_t = boost::geometry::model::referring_segment<const point_t>;
using polygon_t = boost::geometry::model::polygon<point_t>;

// Demi-droite pour le calcul de distance dans une direction donnée
class ray_t
{
public:
	ray_t(const point_t& origin = point_t(0, 0), const point_t& direction = point_t(0, 0));

	float DistanceTo(const polygon_t& polygon, const segment_t& shadow) const;
	float DistanceTo(const ref_segment_t& segment) const;

private:
	point_t ray_t::closest_point_on_ray(const point_t) const;

private:
	point_t mOrigin;
	point_t mDirection;
};

#endif // !_PIP_GEOMETRY_H_