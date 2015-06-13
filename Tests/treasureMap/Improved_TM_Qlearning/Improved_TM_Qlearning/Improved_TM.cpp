#include <Eigen\Eigen>
#include <iostream>
#include <fstream>
#include <random>

std::random_device rd;
std::mt19937 gen(rd());
int m1data[] = { 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
				 1, -2, 0, 0, 0, 0, 0, 0, 0, 0,
				 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
const Eigen::Matrix<int, 10, 10, Eigen::RowMajor> m1(m1data);

using State = std::pair<int, int>;

bool reachable(State state, State action, int dimension)
{
	int f = state.first + action.first;
	int s = state.second + action.second;
	return (f < dimension && f >= 0 && s < dimension && s >= 0);
}

State genState(int i)
{
	switch(i)
	{
	case 0: return std::make_pair(-1, 0);
	case 1: return std::make_pair(0, -1);
	case 2: return std::make_pair(1, 0);
	case 3: return std::make_pair(0, 1);
	}
	return std::make_pair(0, 0);
}

State randomAction(State state, int dimension)
{
	State output;
	do
	{
		std::uniform_int_distribution<> dis(0, 3);
		output = genState(dis(gen));
	} while(!reachable(state, output, dimension));

	return output;
}
double value(const Eigen::Matrix<int, 10, 10>& poly, int x, int y)
{
	double value = 0;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			value += poly(i, j)*pow(x, i)*pow(y, j);
		}
	}
	return value;
}
double maxReward(const Eigen::MatrixXd& Q, State state, int dimension, State* newState = nullptr)
{
	double maxR = -std::numeric_limits<double>::infinity();
	for(int i(0); i < 4; i++)
	{
		State action = genState(i);
		if(!reachable(state, action, dimension)) continue;

		State s2 = std::make_pair(state.first + action.first, state.second + action.second);
		double rec = Q(dimension*state.first + state.second, dimension*s2.first + s2.second);
		if(maxR < rec)
		{
			maxR = rec;
			if(newState)
				*newState = s2;
		}
	}
	return maxR;
}

bool isTreasure(const State& state, int dimension)
{
	return state.first == dimension - 1 && state.second == dimension - 1;
}

int main()
{
	const double alpha = 1;
	const double gamma = 0.9;
	const int dimension = 30;
	const int qdim = dimension*dimension;
	/*coefficients du polynome.
	n°ligne = puissance de x
	n°colonne = puissance de y*/
	

	//creation matrices de qualité
		//exploration pure
	Eigen::MatrixXd Q(qdim, qdim);
		//Exploration + exploitation (proba constante)
	Eigen::MatrixXd Q2(qdim, qdim);
	//initialisation des matrices
	Q.setConstant(-1000);
	Q2.setConstant(-1000);
	//creation de l'état (position de l'agent)
	State state [2];

	std::uniform_int_distribution<> dis(0, dimension - 1);
	//initialisation aléatoire de la position
	state[0].first = dis(gen);	
	state[0].second = dis(gen);
	state[1].first = dis(gen);
	state[1].second = dis(gen);

	std::pair<int, int> action[2];
	for (int i = 0; i < 10000000; i++)
	{
		//choix de l'action au hasard
		action[0] = randomAction(state[0], dimension);

		//choix de  l'action (exploration + exploitation)
		std::uniform_int_distribution<> dis(0, 100);
		double temp = dis(gen);
		if (temp >= 5) 
		{
			// choix de l'action au hasard
				action[1] = randomAction(state[1], dimension);
		}
		else //optimale
		{
			State newState;
			maxReward(Q2, state[1], dimension, &newState);
			std::pair<int, int> action2 = std::make_pair(newState.first - state[1].first, newState.second - state[1].second);
		}


		//récompense
		for (int i = 0; i < 2 ; i++)
		{
			int s = dimension * state[i].first + state[i].second;
			std::pair<int, int> S;
			S.first = state[i].first + action[i].first;
			S.second = state[i].second + action[i].second;
			int nextState = dimension * S.first + S.second;
			double r;
			if (isTreasure(state[i], dimension))
				r = 100000;
			else if (action[i].first != 0)
				r = -(value(m1, state[i].first, state[i].second) + value(m1, state[i].first + action[i].first, state[i].second)) / 2;
			else
				r = -(value(m1, state[i].first, state[i].second) + value(m1, state[i].first, state[i].second + action[i].second)) / 2;

			if (i = 0)
			{
				Q(s, nextState) = (1 - alpha)*Q(s, nextState) + gamma*(r + maxReward(Q, S, dimension));
			}
			else
			{
				Q2(s, nextState) = (1 - alpha)*Q(s, nextState) + gamma*(r + maxReward(Q, S, dimension));
			}

			state[i] = S;

			if (isTreasure(state[i], dimension))
			{
				std::cout << "finished" << std::endl;
				state[i] = std::make_pair(dis(gen), dis(gen));
			}
		}
	}
	std::ofstream file("log_exploration.csv");
	std::ofstream file2("log_combinaison.csv");
	for (int i = 0; i < 2; i++)
	{
		state[i] = std::make_pair(0, 0);
		int maxIter = 200;
		do
		{
			if (i = 0)
			{
				file << state[i].first << "," << state[i].second << std::endl;
				std::cout << maxReward(Q, state[i], dimension, &state[i]) << std::endl;
			}
			if (i = 1)
			{
				file2 << state[i].first << "," << state[i].second << std::endl;
				std::cout << maxReward(Q, state[i], dimension, &state[i]) << std::endl;
			}

			
		} while (!isTreasure(state[i], dimension) && maxIter--);
	}
	file.close();
	file2.close();
	system("pause");
	return 0;
}