#include "Geometry.h"

ray_t::ray_t(const point_t& origin, const point_t& direction) :
mOrigin(origin), mDirection(direction)
{

}

float ray_t::DistanceTo(const polygon_t& polygon) const
{
	// TODO
	// for_each_segment -> DistanceTo(segment)
	return -1;
}

float ray_t::DistanceTo(const segment_t& segment) const
{
	// TODO
	return -1;
}