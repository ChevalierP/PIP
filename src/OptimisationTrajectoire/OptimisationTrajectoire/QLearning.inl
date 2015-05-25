template<class T>
QLearning<T>::QLearning(StateSpace& ss, Quality& quality, Vehicule& veh, const Track& track, const T& rewardPolicy) :
mStateSpace(ss), mQuality(quality), mVehicule(veh), mTrack(track), mRewardPolicy(rewardPolicy), mGamma(.9f), mAlpha(.5f)
{

}

template<class T>
void QLearning<T>::Sim(const Vehicule::StateType& position, const Command& command)
{
	mVehicule.Reset(position);
	mVehicule.AddCommand(command);
	Sensors* s = mVehicule.GetSensors();
	int maxiter = 1000;

	while(mTrack.IsInside(mVehicule.GetLastPosition()) && maxiter--)
	{
		const Observation& obs0 = s->GetObservation();
		const Command& c0 = mVehicule.GetLastCommand();

		Command c1 = mStateSpace.GenRandomCommand();

		mVehicule.AddCommand(c1);
		mVehicule.Sim();
		mTrack.UpdateSensors(&mVehicule);
		const Observation& obs1 = s->GetObservation();

		float r = mRewardPolicy.GetReward(obs1, c1, c0, mVehicule, mTrack);
		float q0 = mQuality.Get(obs0, c0, c1);
		float q1 = (1 - mAlpha)*q0 + mAlpha*(r + mGamma*mQuality.GetBestReward(obs1, c1));
		mQuality.UpdateCommandReward(obs0, c0, c1, q1);
	}
	//std::cout << maxiter << std::endl;
}