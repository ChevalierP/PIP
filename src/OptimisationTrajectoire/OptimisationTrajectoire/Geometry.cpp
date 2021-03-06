#include <boost\geometry.hpp>
#include "Geometry.h"


ray_t::ray_t(const point_t& origin, const point_t& direction) :
mOrigin(origin), mDirection(direction)
{
	
}

float ray_t::DistanceTo(const polygon_t& polygon, const segment_t& shadow) const
{
	float min_distance = std::numeric_limits<float>::infinity();
	boost::geometry::for_each_segment(polygon, [this, &min_distance, &shadow](ref_segment_t s) {
		if(boost::geometry::equals(s, shadow)) return;
		float distance = ray_t::DistanceTo(s);
		if(distance < min_distance)
			min_distance = distance;
	});

	return min_distance;
}

float ray_t::DistanceTo(const ref_segment_t& segment) const
{
	point_t proj_on_ray1 = ray_t::closest_point_on_ray(segment.first);
	point_t proj_on_ray2 = ray_t::closest_point_on_ray(segment.second);

	ref_segment_t proj_on_ray_segment(proj_on_ray1, proj_on_ray2);
	std::vector<point_t> output;
	boost::geometry::intersection(segment, proj_on_ray_segment, output);
	if(output.size())
	{
		point_t d(output[0].x() - mOrigin.x(), output[0].y() - mOrigin.y());
		return std::sqrt(d.x()*d.x() + d.y()*d.y());
	}
	return std::numeric_limits<float>::infinity();
}

point_t ray_t::closest_point_on_ray(const point_t point) const
{
	float norm = std::sqrt(mDirection.x()*mDirection.x() + mDirection.y()*mDirection.y());
	point_t normalized_mDirection(mDirection.x() / norm, mDirection.y() / norm);

	point_t pa(mOrigin.x() - point.x(), mOrigin.y() - point.y());
	float pan = boost::geometry::dot_product(pa, normalized_mDirection);
	point_t pann(-pan*normalized_mDirection.x(), -pan*normalized_mDirection.y());

	point_t proj_on_ray(mOrigin.x() + pann.x(), mOrigin.y() + pann.y());
	if(boost::geometry::dot_product(pann, mDirection) > 0)
		return proj_on_ray;
	return mOrigin;
}