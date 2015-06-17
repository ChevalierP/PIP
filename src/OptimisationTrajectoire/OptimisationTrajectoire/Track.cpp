#include <boost/geometry/geometry.hpp>
#include "Vehicule.h"
#include "Track.h"

bool Track::IsInside(point_t pt) const
{
	return boost::geometry::within(pt, mPolygon);
}

bool Track::HasFinished(point_t pt) const
{
	return boost::geometry::within(pt, mFinishArea);
}

void Track::UpdateSensors(Vehicule* veh) const
{
	point_t origin = veh->GetLastPosition();
	Sensors* sensors = veh->GetSensors();
	sensors->Foreach(veh->GetAxis(), [this, &origin](point_t direction) -> float {
		ray_t ray(origin, direction);
		return ray.DistanceTo(mPolygon, mFinishLine);
	});
}

TrackLine::TrackLine(float length)
{
	boost::geometry::append(mPolygon, point_t(0, 2.5));
	boost::geometry::append(mPolygon, point_t(length, 2.5));
	boost::geometry::append(mPolygon, point_t(length, -2.5));
	boost::geometry::append(mPolygon, point_t(0, -2.5));
	boost::geometry::append(mPolygon, point_t(0, 2.5));

	boost::geometry::append(mFinishArea, point_t(length, 2.5));
	boost::geometry::append(mFinishArea, point_t(length, -2.5));
	boost::geometry::append(mFinishArea, point_t(length + 20, -2.5));
	boost::geometry::append(mFinishArea, point_t(length + 20, 2.5));
	boost::geometry::append(mFinishArea, point_t(length, 2.5));

	mFinishLine.first = point_t(length, 2.5);
	mFinishLine.second = point_t(length, -2.5);
}

point_t TrackLine::GetTrackAxis(const point_t& pt) const
{
	return point_t(1, 0);
}

TrackTurn::TrackTurn(float length) : 
mLength(length)
{
	boost::geometry::append(mPolygon, point_t(0, 2.5f));
	boost::geometry::append(mPolygon, point_t(length + 2.5f, 2.5f));
	boost::geometry::append(mPolygon, point_t(length + 2.5f, -length));
	boost::geometry::append(mPolygon, point_t(length - 2.5f, -length));
	boost::geometry::append(mPolygon, point_t(length - 2.5f, -2.5f));
	boost::geometry::append(mPolygon, point_t(0, -2.5f));
	boost::geometry::append(mPolygon, point_t(0, 2.5f));

	boost::geometry::append(mFinishArea, point_t(length + 2.5f, -length));
	boost::geometry::append(mFinishArea, point_t(length - 2.5f, -length));
	boost::geometry::append(mFinishArea, point_t(length - 2.5f, -length - 20));
	boost::geometry::append(mFinishArea, point_t(length + 2.5f, -length - 20));
	boost::geometry::append(mFinishArea, point_t(length + 2.5f, -length));

	mFinishLine.first = point_t(length + 2.5f, -length);
	mFinishLine.second = point_t(length - 2.5f, -length);
}

point_t TrackTurn::GetTrackAxis(const point_t& pt) const
{
	return (pt.x() - mLength < pt.y() ? point_t(1, 0) : point_t(0, -1));
}

SoftTurn::SoftTurn(float length) :
mLength(length)
{
	boost::geometry::append(mPolygon, point_t(0, 2.5f));
	boost::geometry::append(mPolygon, point_t(21.f, 2.5f));
	boost::geometry::append(mPolygon, point_t(41.f, -16.f));
	boost::geometry::append(mPolygon, point_t(37.f, -20.f));
	boost::geometry::append(mPolygon, point_t(19.f, -2.5f));
	boost::geometry::append(mPolygon, point_t(0, -2.5f));
	boost::geometry::append(mPolygon, point_t(0, 2.5f));

	boost::geometry::append(mFinishArea, point_t(41.f, -16.f));
	boost::geometry::append(mFinishArea, point_t(37.f, -20.f));
	boost::geometry::append(mFinishArea, point_t(43.f, -26.f));
	boost::geometry::append(mFinishArea, point_t(46.f, -22.f));
	boost::geometry::append(mFinishArea, point_t(41.f, -16.f));

	mFinishLine.first = point_t(41.f, -16.f);
	mFinishLine.second = point_t(37.f, -20.f);
}

point_t SoftTurn::GetTrackAxis(const point_t& pt) const
{
	return (pt.x() - 18.f < pt.y() ? point_t(1, 0) : point_t(.707f, -.707f));
}