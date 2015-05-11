#include <Eigen\Eigen>
#include <iostream>
#include <fstream>
#include "exemple.h"
#include <random>

std::random_device rd;
std::mt19937 gen(rd());

bool reachable(int state, int action)
{
	return m1(state, action) != '-';
}

int randomAction(int state)
{
	int output;
	do
	{
		std::uniform_int_distribution<> dis(min, max);
		output = dis(gen);
	} while(!reachable(state, output));

	return output;
}

int rewardCharToInt(int state, int action)
{
	switch(m1(state, action))
	{
	case 'T': return treasureReward; break;
	case 'w': return wallsReward; break;
	case 'c': return caseReward; break;
	default: return 0;
	}
}

std::pair<double,int> maxOfRow(Eigen::Matrix<double, m1size, m1size> M, int state)
{
	using mypair = std::pair<double, int>;
	std::vector<mypair> row;
	for(int i(0); i<M.cols(); i++)
		row.emplace_back(M(state, i), i);
	std::sort(row.begin(), row.end(), [](mypair l, mypair r) { return l.first > r.first; });

	for(int i(0); i<M.cols(); i++)
	{
		if(reachable(state, row[i].second))
			return row[i];
	}
	std::cout << "err";
	return std::make_pair(0, 0);
}

int main()
{
	std::ofstream file("log.csv");
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
	int compteur1 = 0;
	int compteur2 = 0;
	int compteur3 = 0;
	int compteur4 = 0;
	std::vector<int> conv1;
	std::vector<int> conv2;
	std::vector<int> conv3;
	std::vector<int> conv4;

	for (int i = 0; i < 1000000; i++)
	{
		
		compteur1++;
		compteur2++;
		compteur3++;
		compteur4++;

		std::uniform_real_distribution<> dis(0, 1);
		double r = dis(gen);

		action1 = randomAction(state1);
		action2 = maxOfRow(Q2, state2).second;
		if(r > 0.25) 
			action3 = randomAction(state3);
		else
			action3 = maxOfRow(Q3, state3).second;

		Q1(state1, action1) = rewardCharToInt(state1, action1) + gamma*maxOfRow(Q1, action1).first;
		Q2(state2, action2) = rewardCharToInt(state2, action2) + gamma*maxOfRow(Q2, action2).first;
		Q3(state3, action3) = rewardCharToInt(state3, action3) + gamma*maxOfRow(Q3, action3).first;

		state1 = action1;
		state2 = action2;
		state3 = action3;
		state4 = action4;
		if (state1 == 24)
		{
			state1 = 0;
			conv1.emplace_back(compteur1);
			compteur1 = 0;
		}
		if (state2 == 24)
		{
			state2 = 0; 
			conv2.emplace_back(compteur2);
			compteur2 = 0;
		}
		if (state3 == 24)
		{
			state3 = 0;
			conv3.emplace_back(compteur3);
			compteur3 = 0;
		}
		if (state4 == 24)
		{
			state4 = 0;
			conv4.emplace_back(compteur4);
			compteur4 = 0;
		}
	}
	for (int i = 0; i < conv1.size(); i++)
	{
		file << conv1[i] << std::endl;
	}
	file << '\t' << std::endl;
	for (int i = 0; i < conv2.size(); i++)
	{
		file << conv2[i] << std::endl;
	}
	file << '\t' << std::endl;
	for (int i = 0; i < conv3.size(); i++)
	{
		file << conv3[i] << std::endl;
	}
	file << '\t' << std::endl;
	for (int i = 0; i < conv4.size(); i++)
	{
		file << conv4[i] << std::endl;
	}

	file.close();
	/*
	Eigen::Matrix<char, 5, 5> test;
	test.setConstant('.');
	int state = 0;
	for(int i(0); i < 25; i++)
	{
		test(state/5, state%5) = 1;
		std::cout << test << std::endl << std::endl;
		std::cin.get();
		if(state == 24) break;
		state = maxOfRow(Q2, state).second;
	}
	*/
	system("pause");
	return 0;
}