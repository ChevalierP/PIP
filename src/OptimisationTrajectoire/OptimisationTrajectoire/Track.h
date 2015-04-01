#ifndef _PIP_TRACK_H_
#define _PIP_TRACK_H_

#include "Geometry.h"

class Vehicule;

class Track
{
public:
	bool IsInside(point_t pt) const;
	void UpdateSensors(Vehicule* veh) const;

protected:
	//virtual VectorType GetTrackAxis(PointType pt) const = 0;

protected:
	polygon_t mPolygon;
};

class TrackLine : public Track
{
public:

};

#endif // !_PIP_TRACK_H_