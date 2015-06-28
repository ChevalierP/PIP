#ifndef _PIP_VEHICULE_H_
#define _PIP_VEHICULE_H_

#include <array>
#include "State.h"
#include "Geometry.h"

class Sensors;

class Vehicule
{
public:
	// Vecteur d'état (x, y, alpha)
	// avec alpha l'angle entre l'axe des abscisses et l'axe longitudinal du véhicule
	using StateType = std::array<float, 3>;

	Vehicule(Sensors* sensors);

	// Distance en le centre de gravité et l'essieu arrière
	float GetGravityCenterPosition() const { return mGravityCenter; }

	// Ajout de commandes (vitesse, direction)
	void AddCommand(const Command& c);
	template<class Itr>
	void AddCommand(Itr begin, Itr end);

	// Simulation cinématique
	void Sim();
	
	const std::vector<StateType>& GetPositions() const { return mPosition; }
	point_t GetLastPosition() const;
	float GetAxis() const;

	const Command& GetLastCommand() const { return mCommand.back(); }

	Sensors* GetSensors() { return mSensors; }

	void Reset(const StateType& position);

private:
	// Dérivée du vecteur d'état par rapport au temps
	void System(const StateType& x, StateType& dx, float t);

private:
	std::vector<Command> mCommand;
	std::vector<StateType> mPosition;
	Sensors* mSensors;
	float mGravityCenter;
	float mCurrentTime;
	float mTimeStep;
};

template<class Itr>
void Vehicule::AddCommand(Itr begin, Itr end)
{
	std::copy(begin, end, std::back_inserter(mCommand));
}

#endif // !_PIP_VEHICULE_H_