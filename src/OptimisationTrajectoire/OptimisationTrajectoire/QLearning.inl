template<class T>
QLearning<T>::QLearning(Config& config, StateSpace& ss, Quality& quality, Vehicule& veh, const Track& track, const T& rewardPolicy) :
mConfig(config),
mStateSpace(ss),
mQuality(quality),
mVehicule(veh),
mTrack(track),
mRewardPolicy(rewardPolicy)
{

}

template<class T>
bool QLearning<T>::Sim(const Vehicule::StateType& position, const Command& command, StateChoicePolicy scp)
{
	mVehicule.Reset(position);
	mVehicule.AddCommand(command);
	Sensors* s = mVehicule.GetSensors();
	const int maxiter = mConfig.maxIterationCount();
	int iter = maxiter;

	// Trajectoire dans l'espace d'�tat
	using StatePath = std::vector<std::tuple<Observation, Command, Command>>;
	StatePath statePath;

	// On effectue la simulation tant que le v�hicule est dans le circuit et que le
	// nombre maximal d'it�ration n'est pas atteint
	while(mTrack.IsInside(mVehicule.GetLastPosition()) && iter--)
	{
		const Observation& obs0 = s->GetObservation();
		const Command& c0 = mVehicule.GetLastCommand();

		Command c1;
		switch(scp)
		{
		case StateChoicePolicy::Exploration: c1 = mStateSpace.GenRandomCommand(); break;
		case StateChoicePolicy::Exploitation: c1 = mQuality.GetBestCommand(obs0, c0, mStateSpace.GenRandomCommand()); break;
		}

		// Simulation de la nouvelle commande
		mVehicule.AddCommand(c1);
		mVehicule.Sim();
		// Mise � jour des capteurs � partir de la nouvelle position du v�hicule
		mTrack.UpdateSensors(&mVehicule);
		const Observation& obs1 = s->GetObservation();

		// Calcul du gain
		float r = mRewardPolicy.GetReward(obs1, c1, c0, mVehicule, mTrack);

		// Mise � jour de la matrice de qualit�
		float q0 = mQuality.Get(obs0, c0, c1);
		float q1 = (1 - mConfig.alpha())*q0 + mConfig.alpha()*(r + mConfig.gamma()*mQuality.GetBestReward(obs1, c1));
		mQuality.UpdateCommandReward(obs0, c0, c1, q1);

		statePath.emplace_back(obs0, c0, c1);
	}

	// R�tropropagation du co�t de sortie de piste
	if(mConfig.offTrackRewardBackpropagation())
	{
		point_t lastpos = mVehicule.GetLastPosition();
		// Si le v�hicule est hors circuit on effectue la r�tropropagation
		if(!mTrack.IsInside(lastpos) && !mTrack.HasFinished(lastpos))
		{
			StatePath::const_reverse_iterator it = statePath.rbegin();
			Observation c0 = std::get<0>(*it);
			// La r�tropropagation est effectu�e jusqu'� "obscount" changements d'observations
			int obscount = 3;
			do
			{
				mQuality.UpdateCommandReward(std::get<0>(*it), std::get<1>(*it), std::get<2>(*it), -10);
				++it;
				if(std::get<0>(*it) != c0)
				{
					obscount--;
					c0 = std::get<0>(*it);
				}
			} while(obscount && it != statePath.rend());
		}
	}
	return mTrack.HasFinished(mVehicule.GetLastPosition());
}