#ifndef _PIP_TRACK_H_
#define _PIP_TRACK_H_

#include "Geometry.h"

class Vehicule;

// Classe abstraite d�finissant un circuit
class Track
{
public:
	bool IsInside(point_t pt) const;
	bool HasFinished(point_t pt) const;

	// Calcul des distances des capteurs du v�hicule
	void UpdateSensors(Vehicule* veh) const;

	// Axe moyen du circuit en un point donn�
	virtual point_t GetTrackAxis(const point_t& pt) const = 0;

protected:
	polygon_t mPolygon;
	polygon_t mFinishArea;
	segment_t mFinishLine;
};

// Ligne droite
class TrackLine : public Track
{
public:
	TrackLine(float length);

protected:
	virtual point_t GetTrackAxis(const point_t& pt) const;
};

// Virage � 90�
class TrackTurn : public Track
{
public:
	TrackTurn(float length);
	
protected:
	virtual point_t GetTrackAxis(const point_t& pt) const;

protected:
	float mLength;
};

// Virage � 45�
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