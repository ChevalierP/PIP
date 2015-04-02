#ifndef _PIP_GEOMETRY_H_
#define _PIP_GEOMETRY_H_

#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/segment.hpp>
#include <boost\geometry.hpp>


using point_t = boost::geometry::model::d2::point_xy<float>;
using segment_t = boost::geometry::model::referring_segment<point_t>;
using polygon_t = boost::geometry::model::polygon<point_t>;

class ray_t
{
public:
	ray_t(const point_t& origin = point_t(0, 0), const point_t& direction = point_t(0, 0));

	float DistanceTo(const polygon_t& polygon) const;
	float DistanceTo(const segment_t& segment) const;

private:

private:
	point_t mOrigin;
	point_t mDirection;
};

#endif // !_PIP_GEOMETRY_H_