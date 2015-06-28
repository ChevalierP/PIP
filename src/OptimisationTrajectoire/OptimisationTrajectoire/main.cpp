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
		.gamma(.9f)								// facteur de d�valuation
		.alpha(.6f)								// taux d'apprentissage
		.steeringCostFactor(.02f)				// co�t au changement de direction
		.rewardSteeringChange(true)				// prise en compte de ce co�t
		.skidCost(-10)							// co�t du d�rapage
		.rewardSkid(true)						// prise en compte du d�rapage
		.offTrackCost(-100)						// co�t de sortie de piste
		.rewardOffTrack(true)					// prise en compte des sorties de piste
		.finishLineReward(100)					// r�compense finale
		.rewardFinish(true)						// prise en compte de la r�compense finale
		.attemptCount(40000)					// nombre de tentatives
		.maxIterationCount(1000)				// nombre d'it�rations maximal par tentative
		.rollingStart(false)					// d�part glissant
		.coefficientOfFriction(.6f)				// coefficient de frottement
		.accelerationOfGravity(9.81f)			// g
		.offTrackRewardBackpropagation(true);	// r�tropropagation du co�t de sortie de piste

	// Virage � 45�
	SoftTurn tl(20);
	// Espace d'�tat (vitesse, direction, secteurs distance)
	StateSpace ss({0, 20, 5}, {-0.5f, 0.5f, 31}, {.5f, 1.f, 2.f, 4.f});
	// Capteurs de distance
	Sensors sensors(&ss, {Sensors::Left, Sensors::Front, Sensors::Right});
	Vehicule veh(&sensors);
	// Matrice de qualit�
	Quality q;
	// Politique de r�compense
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
		// Meilleure commande d�termin�e � partir de la matrice de qualit�
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