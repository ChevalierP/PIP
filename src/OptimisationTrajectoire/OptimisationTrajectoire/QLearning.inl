template<class T>
QLearning<T>::QLearning(Quality& quality, Vehicule& veh, const Track& track, const T& rewardPolicy) :
mQuality(quality), mVehicule(veh), mTrack(track), mRewardPolicy(rewardPolicy)
{

}

template<class T>
void QLearning<T>::Sim()
{
	Sensors* s = mVehicule.GetSensors();
	int maxiter = 100;
	while(mTrack.isInside(mVehicule.GetLastPosition()) && maxiter--)
	{
		Command to;
		float q = mRewardPolicy.GetReward(s, to) + gamma*mQuality.GetBestReward(*s, mVehicule.GetLastCommand(), to);
		mQuality.UpdateCommandReward(*s, mVehicule.GetLastCommand(), to, q);
		mVehicule.AddCommand(to);
		mVehicule.Sim();
	}
}