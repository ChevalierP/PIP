#ifndef _PIP_TRACK_H_
#define _PIP_TRACK_H_

#include <wykobi\wykobi.hpp>

class Track
{
public:
	using PolygonType = wykobi::polygon<float, 2>;

private:
	PolygonType mPolygon;
};

#endif // !_PIP_TRACK_H_