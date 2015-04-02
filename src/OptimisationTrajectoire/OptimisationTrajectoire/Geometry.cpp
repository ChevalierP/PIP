#include <boost\geometry.hpp>
#include "Geometry.h"


ray_t::ray_t(const point_t& origin, const point_t& direction) :
mOrigin(origin), mDirection(direction)
{
	
}

float ray_t::DistanceTo(const polygon_t& polygon) const
{
	// TODO
	// for_each_segment -> DistanceTo(segment)
	float min_distance = std::numeric_limits<float>::infinity();
	boost::geometry::for_each_segment(polygon, [this, &min_distance](segment_t s) {
		float distance;
		 distance = ray_t::DistanceTo(s);
		 if (distance < min_distance)
			 min_distance = distance;
	});

	return min_distance;
}

float ray_t::DistanceTo(const segment_t& segment) const
{
	// TODO
	float norm = std::sqrt(mDirection.x()*mDirection.x() + mDirection.y()*mDirection.y());
	point_t normalized_mDirection(mDirection.x() / norm, mDirection.y() / norm);
	point_t pa(mOrigin.x() - segment.first.x(), mOrigin.y() - segment.first.y());
	point_t pann((pa.x()*normalized_mDirection.y() + pa.y()*normalized_mDirection.x())*normalized_mDirection.x(), (pa.x()*normalized_mDirection.y() + pa.y()*normalized_mDirection.x())*normalized_mDirection.y());

	point_t d(pa.x() - pann.x(), pa.y() - pann.y());

	return std::sqrt(d.x()*d.x() + d.y()*d.y());
}