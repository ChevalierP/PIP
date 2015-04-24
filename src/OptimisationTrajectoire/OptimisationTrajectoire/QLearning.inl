template<class T>
QLearning<T>::QLearning(StateSpace& ss, Quality& quality, Vehicule& veh, const Track& track, const T& rewardPolicy) :
mStateSpace(ss), mQuality(quality), mVehicule(veh), mTrack(track), mRewardPolicy(rewardPolicy)
{

}

template<class T>
void QLearning<T>::Sim(const Vehicule::StateType& position, const Command& command)
{
	mVehicule.Reset(position);
	mVehicule.AddCommand(command);
	Sensors* s = mVehicule.GetSensors();
	int maxiter = 1000;
	float gamma = 0.8f;
	float alpha = 0.2f;

	int f = 0;
	while(mTrack.IsInside(mVehicule.GetLastPosition()) && maxiter--)
	{
		const Observation& obs0 = s->GetObservation();
		const Command& c0 = mVehicule.GetLastCommand();

		Command c1 = f++%20 ? mQuality.GetBestCommand(obs0, c0) : mStateSpace.GenRandomCommand();
		if(std::get<0>(c1) == 0)
			c1 = mStateSpace.GenRandomCommand();

		mVehicule.AddCommand(c1);
		mVehicule.Sim();
		mTrack.UpdateSensors(&mVehicule);
		const Observation& obs1 = s->GetObservation();

		float r = mRewardPolicy.GetReward(obs1, c1, mVehicule, mTrack);
		float q0 = mQuality.Get(obs0, c0, c1);
		float q1 = (1 - alpha)*q0 + alpha*(r + gamma*mQuality.GetBestReward(obs1, c1));

		bool isin = mTrack.IsInside(mVehicule.GetLastPosition());
		mQuality.UpdateCommandReward(obs0, c0, c1, q1);
	}
	std::cout << maxiter << std::endl;
}