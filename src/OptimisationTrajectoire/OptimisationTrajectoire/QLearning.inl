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

	while(mTrack.IsInside(mVehicule.GetLastPosition()) && iter--)
	{
		const Observation& obs0 = s->GetObservation();
		const Command& c0 = mVehicule.GetLastCommand();

		Command c1;
		//Command c1 = mStateSpace.GenConstrainedCommand(c0);
		switch(scp)
		{
		case StateChoicePolicy::Exploration: c1 = mStateSpace.GenRandomCommand(); break;
		case StateChoicePolicy::Exploitation: c1 = mQuality.GetBestCommand(obs0, c0, mStateSpace.GenRandomCommand()); break;
		}

		mVehicule.AddCommand(c1);
		mVehicule.Sim();
		mTrack.UpdateSensors(&mVehicule);
		const Observation& obs1 = s->GetObservation();

		float r = mRewardPolicy.GetReward(obs1, c1, c0, mVehicule, mTrack);
		if(mTrack.HasFinished(mVehicule.GetLastPosition()))
		{
			//r *= (float)iter/maxiter;
			std::cout << (100*(float)iter/maxiter) << std::endl;
		}
		float q0 = mQuality.Get(obs0, c0, c1);
		float q1 = (1 - mConfig.alpha())*q0 + mConfig.alpha()*(r + mConfig.gamma()*mQuality.GetBestReward(obs1, c1));
		mQuality.UpdateCommandReward(obs0, c0, c1, q1);
	}
	return mTrack.HasFinished(mVehicule.GetLastPosition());
}