#ifndef _PIP_TRACK_H_
#define _PIP_TRACK_H_

#include "Geometry.h"

class Vehicule;

class Track
{
public:
	bool IsInside(point_t pt) const;
	void UpdateSensors(Vehicule* veh) const;

	virtual point_t GetTrackAxis(const point_t& pt) const = 0;

protected:
	polygon_t mPolygon;
};

class TrackLine : public Track
{
public:
	TrackLine(float length);

protected:
	virtual point_t GetTrackAxis(const point_t& pt) const;
};

#endif // !_PIP_TRACK_H_