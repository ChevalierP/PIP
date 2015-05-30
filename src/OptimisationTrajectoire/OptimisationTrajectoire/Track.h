#ifndef _PIP_TRACK_H_
#define _PIP_TRACK_H_

#include "Geometry.h"

class Vehicule;

class Track
{
public:
	bool IsInside(point_t pt) const;
	bool HasFinished(point_t pt) const;
	void UpdateSensors(Vehicule* veh) const;

	virtual point_t GetTrackAxis(const point_t& pt) const = 0;

protected:
	polygon_t mPolygon;
	polygon_t mFinishArea;
	segment_t mFinishLine;
};

class TrackLine : public Track
{
public:
	TrackLine(float length);

protected:
	virtual point_t GetTrackAxis(const point_t& pt) const;
};

class TrackTurn : public Track
{
public:
	TrackTurn(float length);
	
protected:
	virtual point_t GetTrackAxis(const point_t& pt) const;

protected:
	float mLength;
};

class SoftTurn : public Track
{
public:
	SoftTurn(float length);

protected:
	virtual point_t GetTrackAxis(const point_t& pt) const;

protected:
	float mLength;
};

#endif // !_PIP_TRACK_H_