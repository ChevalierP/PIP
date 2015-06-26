#include <Eigen\Eigen>
#include <iostream>
#include <fstream>
#include "exemple.h"
#include <random>

std::random_device rd;
std::mt19937 gen(rd());

//v�rifie si l'�tat est accessible 
bool reachable(int state, int action)
{
	return m1(state, action) != '-';
}

//cr�ation d'une action accessible au hasard
int randomAction(int state)
{
	int output;
	do
	{
		std::uniform_int_distribution<> dis(min, max);
		output = dis(gen);//g�n�ration al�atoire
	} while(!reachable(state, output));//v�rification de l'accessibilit�

	return output;
}

//conversion des infos de la matrice dans exemple.h
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

//recherche de la r�compense maximale accessible depuis un �tat (maximum sur une ligne de la matrice de qualit�)
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

//calcule du nombre d'it�rations n�cessaires pour arriver au tr�sor
int ActionCountToTreasure(const Eigen::Matrix<double, m1size, m1size>& Q)
{
	const int niter = 100;
	int state = 0;
	for(int i(0); i < niter; i++)
	{
		if(state == 24) return i;
		state = maxOfRow(Q, state).second;
	}
	return niter;
}

//calcule de la r�compense totale sur le chemin optimal trouv�
int RewardToTreasure(const Eigen::Matrix<double, m1size, m1size>& Q)
{
	const int niter = 100;
	int state = 0;
	int reward = 0;
	for(int i(0); i < niter; i++)
	{
		int action = maxOfRow(Q, state).second;
		reward += rewardCharToInt(state, action);
		state = action;
		if(state == 24) return reward;
	}
	return reward;
}

int main()
{
	//creation du fichier de log du chemin optimal
	std::ofstream file("log.csv");
	Eigen::Matrix<double, m1size, m1size> Q[3]; //exploration ; exploitation et exploration (proba fixe)
	int state[3];
	int action[3];

	//initialisation des matrices de qualit� de tous les agents
	for(int i(0); i<3; i++)
	{
		Q[i].setConstant(0);
		//initialisation des �tats et actions
		state[i] = 0;
		action[i] = 0;
	}

	for (int i = 0; i < 2000; i++)
	{
		//g�n�ration d'un nombre al�atoire entre 0 et 1
		std::uniform_real_distribution<> dis(0, 1);
		double r = dis(gen);
		
		action[0] = randomAction(state[0]);
		if(r > 0.01) //Probabilit� d'exploitation = 1%
			action[1] = randomAction(state[1]); //action au hasard
		else
			action[1] = maxOfRow(Q[1], state[1]).second;//action optimale

		for(int i(0); i<3; i++)
		{
			//mise � jour des matrices de qualit�
			Q[i](state[i], action[i]) = rewardCharToInt(state[i], action[i]) + gamma*maxOfRow(Q[i], action[i]).first;
			state[i] = action[i];
			if (state[i] == 24)//si arriv�e au bout
			{
				//repositionnement al�atoire
				std::uniform_int_distribution<> dis(min, max-1);
				state[i] = dis(gen);
			}
		}
		for(int i(0); i<3; i++)
			file << RewardToTreasure(Q[i]) << ",";
		file << std::endl;
	}

	file.close();

	system("pause");
	return 0;
}