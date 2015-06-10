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
	Eigen::MatrixXd Q(qdim, qdim);
	//initialisation des matrices
	Q.setConstant(-1000);
	//creation de l'état (position de l'agent)
	State state;
	std::uniform_int_distribution<> dis(0, dimension - 1);
	//initialisation aléatoire de la position
	state.first = dis(gen);	
	state.second = dis(gen);

	for (int i = 0; i < 10000000; i++)
	{
		//choix de l'action au hasard
		std::pair<int, int> action1 = randomAction(state, dimension);

		//récompense
		int s = dimension * state.first + state.second;
		std::pair<int, int> S2;
		S2.first = state.first + action1.first;
		S2.second = state.second + action1.second;
		int s2 = dimension * S2.first + S2.second;
		double r;
		if(isTreasure(state, dimension))
			r = 100000;
		else if (action1.first != 0)
			r = -(value(m1, state.first, state.second) + value(m1, state.first + action1.first, state.second)) / 2;
		else
			r = -(value(m1, state.first, state.second) + value(m1, state.first, state.second + action1.second)) / 2;

		Q(s, s2) = (1 - alpha)*Q(s, s2) + gamma*(r + maxReward(Q, S2, dimension));
		state = S2;

		if(isTreasure(state, dimension))
		{
			std::cout << "finished" << std::endl;
			state = std::make_pair(dis(gen), dis(gen));
		}
	}
	std::ofstream file("log.csv");

	state = std::make_pair(0, 0);
	int maxIter = 200;
	do
	{
		file << state.first << "," << state.second << std::endl;
		std::cout << maxReward(Q, state, dimension, &state) << std::endl;
	} while(!isTreasure(state, dimension) && maxIter--);

	file.close();

	system("pause");
	return 0;
}