#include <Eigen\Eigen>
#include <iostream>
#include <fstream>
#include "exemple.h"



bool reachable( int state, int action)
{
	if (m1(state,action) != '-')
		return true;
	else
		return false;
}

int randomAction(int state)
{
	int output;
	do
	{
		output = min + (rand() * (int)(max - min) / max);

	} while (!reachable(state, output));

	return output;
}

int rewardCharToInt(int state, int action)
{
	if (m1(state, action) == 'T')
		return treasureReward;
	else if (m1(state, action) == 'w')
		return wallsReward;
	else
		return 0;
}

std::pair<double,int> maxOfRow(Eigen::Matrix<double, m1size, m1size> M, int state)
{
	std::pair<double, int> temp;
	temp.first = 0;
	temp.second = 0;

	for (int i = 0; i < M.cols(); i++)
	{
		if (M(state, i) >= temp.first)
		{
			temp.first = M(state, i);
			temp.second = i;
		}
	}
	return temp;
}


int main()
{
	std::ofstream file("log.txt");
	Eigen::Matrix<double, m1size, m1size> Q1; //exploration
	Q1.setConstant(0);
	Eigen::Matrix<double, m1size, m1size> Q2; //exploitation
	Q2.setConstant(0);
	Eigen::Matrix<double, m1size, m1size> Q3; //exploitation et exploration (proba fixe)
	Q3.setConstant(0);
	Eigen::Matrix<double, m1size, m1size> Q4; //exploitation puis exploration (proba variable)
	Q4.setConstant(0);
	int state1 = 0, state2 = 0, state3 = 0, state4 = 0;
	int action1 = 0, action2 = 0, action3 = 0, action4 = 0;

	for (int i = 0; i < 100; i++)
	{
		double r = rand();

		if (r > 0.25)
			action3 = randomAction(state3);
		else
			action3 = maxOfRow(Q3, state3).second;

		action1 = randomAction(state1);
		action2 = maxOfRow(Q2, state2).second;
		std::cout << "choix actions" << std::endl;


		Q1(state1, action1) = rewardCharToInt(state1, action1) + gamma*maxOfRow(Q1, action1).first;
		Q2(state2, action2) = rewardCharToInt(state2, action2) + gamma*maxOfRow(Q2, action2).first;
		Q3(state3, action3) = rewardCharToInt(state3, action2) + gamma*maxOfRow(Q3, action2).first;
		std::cout << "matrice qualité" << std::endl;

		state1 = action1;
		state2 = action2;
		state3 = action3;
		state4 = action4;
		if (state1 == 25)
			state1 = 0;
		if (state2 == 25)
			state2 = 0;
		if (state3 == 25)
			state3 = 0;
		if (state4 == 25)
			state4 = 0;

	}
	std::cout << Q1 << std::endl;
	file.close();

	return 0;
}