#include <tuple>
#include <iostream>
#include <fstream>
#include <math.h>
#include "State.h"
#include "Vehicule.h"
#include "Track.h"
#include "Geometry.h"
#include "QLearning.h"
#include "Config.h"
#include <engine.h>

int main()
{
	Config config;
	config
		.gamma(.9f)								// facteur de dévaluation
		.alpha(.6f)								// taux d'apprentissage
		.steeringCostFactor(.02f)				// coût au changement de direction
		.rewardSteeringChange(true)				// prise en compte de ce coût
		.skidCost(-10)							// coût du dérapage
		.rewardSkid(true)						// prise en compte du dérapage
		.offTrackCost(-100)						// coût de sortie de piste
		.rewardOffTrack(true)					// prise en compte des sorties de piste
		.finishLineReward(100)					// récompense finale
		.rewardFinish(true)						// prise en compte de la récompense finale
		.attemptCount(40000)					// nombre de tentatives
		.maxIterationCount(1000)				// nombre d'itérations maximal par tentative
		.rollingStart(false)					// départ glissant
		.coefficientOfFriction(.6f)				// coefficient de frottement
		.accelerationOfGravity(9.81f)			// g
		.offTrackRewardBackpropagation(true);	// rétropropagation du coût de sortie de piste

	// Virage à 45°
	SoftTurn tl(20);
	// Espace d'état (vitesse, direction, secteurs distance)
	StateSpace ss({0, 20, 5}, {-0.5f, 0.5f, 31}, {.5f, 1.f, 2.f, 4.f});
	// Capteurs de distance
	Sensors sensors(&ss, {Sensors::Left, Sensors::Front, Sensors::Right});
	Vehicule veh(&sensors);
	// Matrice de qualité
	Quality q;
	// Politique de récompense
	SpeedAxisReward rp(config);
	QLearning<SpeedAxisReward> ql(config, ss, q, veh, tl, rp);

	Engine* ep = engOpen(nullptr); // Moteur MATLAB

	int niter = config.attemptCount();
	for(int i(0); i<niter; i++)
	{
		StateChoicePolicy scp = StateChoicePolicy::Exploration;
		// Simulation d'une tentative
		ql.Sim({config.rollingStart() ? (19*(float)i/niter+0.01f) : 1, 0, 0}, std::make_tuple(20.f, 0.f), scp);
		if(i%1000 == 0)
			std::cout << 100*((double)i/niter) << " %" << std::endl;
	}


	// Exploitation de la trajectoire optimale apprise
	std::ofstream f("data.csv");

	SoftTurn st(20);
	veh.Reset({1, 0, 0});
	veh.AddCommand(std::make_tuple(20.f, 0.f));
	for(int i(0); i<1000; i++)
	{
		const point_t pt = veh.GetLastPosition();
		f << pt.x() << "," << pt.y() << ",";

		st.UpdateSensors(&veh);
		// Meilleure commande déterminée à partir de la matrice de qualité
		const Command& c = q.GetBestCommand(veh.GetSensors()->GetObservation(), veh.GetLastCommand(), veh.GetLastCommand());
		const Observation& obs = veh.GetSensors()->GetObservation();
		f << std::get<0>(c) << "," << std::get<1>(c) << "," << obs[0] << "," << obs[1] << "," << obs[2] << std::endl;

		veh.AddCommand(c);
		veh.Sim();
		if(st.HasFinished(veh.GetLastPosition()))
			break;
	}
	f.close();

	engEvalString(ep, "data = csvread('a.csv');");
	engEvalString(ep, "plot(data(:,1), data(:,2));figure;plot(data(:, 4));figure;plot(data(:, 3), 'r');");
	engEvalString(ep, "figure;plot(data(:, 5));hold on;plot(data(:, 6), 'r');hold on;plot(data(:, 7), 'g');");
	system("pause");
	engClose(ep);

	return 0;
}