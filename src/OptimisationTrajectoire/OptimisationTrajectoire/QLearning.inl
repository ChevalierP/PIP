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
	int maxiter = 100;
	float gamma = 0.8f;
	while(mTrack.IsInside(mVehicule.GetLastPosition()) && maxiter--)
	{
		mTrack.UpdateSensors(&mVehicule);
		Command to = mStateSpace.GenRandomCommand();
		float q = mRewardPolicy.GetReward(s->GetObservation(), to) + gamma*mQuality.GetBestReward(s->GetObservation(), mVehicule.GetLastCommand(), 0);
		mQuality.UpdateCommandReward(s->GetObservation(), mVehicule.GetLastCommand(), to, q);
		mVehicule.AddCommand(to);
		mVehicule.Sim();
	}
	std::cout << maxiter << std::endl;
}