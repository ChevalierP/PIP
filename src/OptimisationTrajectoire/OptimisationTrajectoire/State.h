#ifndef _PIP_STATE_H_
#define _PIP_STATE_H_

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

// Commande du véhicule en vitesse et en direction
class Command
{
public:
	Command(float speed = 0, float steering = 0);

	Command& operator=(Command rhs);
	bool operator<(const Command& rhs) const;

	std::tuple<float, float> GetValue() const;

private:
	float mSpeed;
	float mSteering;
};

/*template<class T>
class DiscretizationPolicy
{
public:
	using IndexType = T;
	virtual IndexType Convert(float distance) = 0;
};*/

class DiscreteSectorPolicy
{
public:
	using IndexType = int;

	DiscreteSectorPolicy(std::initializer_list<float> limits);

	IndexType Convert(float distance);

private:
	std::vector<float> mLimits;
};

class Sensors
{
public:
	static const float Front;
	static const float Right;
	static const float Left;

	Sensors(std::initializer_list<float> angles);

	bool operator<(const Sensors& rhs) const;

protected:
	virtual bool Compare(const Sensors* rhs) const;

protected:
	std::vector<float> mAngles;
	std::vector<float> mDistances;
};

template<class T>
class DiscretizedSensors : public Sensors
{
public:
	using PolicyType = T;
	DiscretizedSensors(std::initializer_list<float> angles, PolicyType policy);

protected:
	virtual bool Compare(const Sensors* rhs) const;

protected:
	PolicyType mPolicy;
	std::vector<typename PolicyType::IndexType> mDiscreteDistances;
};

#endif // !_PIP_STATE_H_