#include <Eigen\Eigen>
#include <iostream>
#include <fstream>
#include "exemple.h"
#include <random>

std::random_device rd;
std::mt19937 gen(rd());

double value(Eigen::Matrix<double, m1size, m1size> M, int x, int y)
{
	double value = 0;
	for (int i = 0; i < m1size; i++)
	{
		for (int j = 0; j < m1size; j++)
		{
			value += M(i,j) * pow(x, i) * pow(y, j);
		}
	}
	return value;
}

Eigen::Matrix<int, m1size, m1size> polyDerive(Eigen::Matrix<double, m1size, m1size> M, int dir) // dir = 0 --> x dir = 1 --> y
{
	Eigen::Matrix < int, m1size, m1size > M;

	if (dir = 0)
	{
		for (int i = 0; i < m1size; i++)
		{
			for (int j = 0; j < m1size; j++)
			{
				if (i = m1size) M(i, j) = 0;
				else M(i, j) = M(i + 1, j)*(i + 1);
			}
		}
	}
	if (dir = 1)
	{
		for (int i = 0; i < m1size; i++)
		{
			for (int j = 0; j < m1size; j++)
			{
				if (j = m1size) M(i, j) = 0;
				else M(i, j) = M(i, j + 1)*(j + 1);
			}
		}
	}

	return M;
}

bool reachable(std::pair<int , int> state, std::pair<int,int> action)
{
	if (action.first == -1 && action.second == 0) //up
	{
		return (state.first - 1) > 0;
	}
	if (action.first == 0 && action.second == -1) //left
	{
		return (state.second - 1) > 0;
	}
	if (action.first == 1 && action.second == 0) //down
	{
		return (state.first + 1) < dim;
	}
	if (action.first == 0 && action.second == 1) //right
	{
		return (state.second + 1) < dim;
	}
}
std::pair<int, int> randomAction(std::pair<int, int> state)
{
	std::pair<int, int> output;
	do
	{
		std::uniform_int_distribution<> dis(0, 4);
		int temp = dis(gen);
		if (temp = 0) //up
		{
			output.first = -1;
			output.second = 0;
		}
		if (temp = 1) //left
		{
			output.first = 0;
			output.second = -1;
		}
		if (temp = 2) //down
		{
			output.first = 1;
			output.second = 0;
		}
		if (temp = 3) //right
		{
			output.first = 0;
			output.second = 1;
		}
	} while (!reachable(state, output));

	return output;
}

std::pair<std::pair<int, int>, double> maxOfRow(Eigen::Matrix<double, m1size, m1size> Q, std::pair<int, int> state, Eigen::Matrix<double, m1size, m1size> dX, Eigen::Matrix<double, m1size, m1size> dY)
{
	using mypair = std::pair<std::pair<int,int>, double>;
	std::vector<mypair> row;
	if (reachable(state, std::make_pair(-1,0)))
		row.emplace_back(std::make_pair(-1,0), dX(state.first - 1, state.second));
	if (reachable(state, std::make_pair(0, -1)))
		row.emplace_back(std::make_pair(0, -1), dY(state.first , state.second - 1));
	if (reachable(state, std::make_pair(1, 0)))
		row.emplace_back(std::make_pair(1, 0), dX(state.first + 1, state.second));
	if (reachable(state, std::make_pair(0, 1)))
		row.emplace_back(std::make_pair(0, 1), dX(state.first, state.second + 1));

	std::sort(row.begin(), row.end(), [](mypair l, mypair r) { return l.second > r.second; });
	return row[0];
}

int ActionCountToTreasure(const Eigen::Matrix<double, m1size, m1size>& Q)
{
	const int niter = 100;
	int state = 0;
	for (int i(0); i < niter; i++)
	{
		if (state == 24) return i;
		state = maxOfRow(Q, state).second;
	}
	return niter;
}

int RewardToTreasure(const Eigen::Matrix<double, m1size, m1size>& Q)
{
	const int niter = 100;
	int state = 0;
	int reward = 0;
	for (int i(0); i < niter; i++)
	{
		int action = maxOfRow(Q, state).second;
		reward += value(m1, state, action);
		state = action;
		if (state == 24) return reward;
	}
	return reward;
}

int main()
{
	Eigen::Matrix<double, m1size, m1size> dX;
	Eigen::Matrix<double, m1size, m1size> dY;
	dX = polyDerive(dX, 0);
	dY = polyDerive(dY, 1);
	Eigen::Matrix<double, dim, dim> reward[2];
	for (int i = 0; i < dim; i++)
	{
		for (int j = 0; j < dim; j++)
		{
			reward[0](i, j) = value(dX, i, j);
			reward[1](i, j) = value(dY, i, j);
		}
	}

	std::ofstream file("log.csv");
	Eigen::Matrix<double, dim, dim> Q[3]; //exploration ; exploitation et exploration (proba fixe) ; exploitation puis exploration (proba variable)
	std::pair<int,int> state[3];
	std::pair<int,int> action[3];
	for (int i(0); i<3; i++)
	{
		Q[i].setConstant(0);
		state[i].first = 0;
		state[i].second = 0;
		action[i].first = 0;
		action[i].second = 0;
	}

	for (int i = 0; i < 2000; i++)
	{
		std::uniform_real_distribution<> dis(0, 1);
		double r = dis(gen);

		action[0] = randomAction(state[0]);
		if (r > 0.01)
			action[1] = randomAction(state[1]);
		else
			action[1] = maxOfRow(Q[1], state[1], dX, dY);

		for (int i(0); i<3; i++)
		{
			if (action[i].first != 0)
				Q[i](state[i], action[i]) = dY(state[i].first + action[i].first, state[i].second + action[i].second) + gamma*maxOfRow(Q[i], action[i], dX, dY).second;
			else
				Q[i](state[i], action[i]) = dX(state[i].first + action[i].first, state[i].second + action[i].second) + gamma*maxOfRow(Q[i], action[i], dX, dY).second;
			state[i] = action[i];
			if (state[i].first == 100 && state[i].second == 100)
			{
				std::uniform_int_distribution<> dis(0, dim - 1);
				state[i].first = dis(gen);
				state[i].second = dis(gen);
			}
		}
		for (int i(0); i<3; i++)
			file << RewardToTreasure(Q[i]) << ",";
		file << std::endl;
	}

	file.close();

	system("pause");
	return 0;
}