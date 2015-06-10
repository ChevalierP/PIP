#include <Eigen\Eigen>
#include <iostream>
#include <fstream>
#include <random>

std::random_device rd;
std::mt19937 gen(rd());
int m1data[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };
const Eigen::Matrix<int, 10, 10, Eigen::RowMajor> m1(m1data);

bool reachable(std::pair<int, int> state, std::pair<int, int> action, int dimension)
{
	int f = state.first + action.first;
	int s = state.second + action.second;
	return (f < dimension && f > 0 && s < dimension && s > 0);
}
std::pair<int, int> randomAction(std::pair<int, int> state, int dimension)
{
	std::pair<int, int> output;
	do
	{
		std::uniform_int_distribution<> dis(0, 3);
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
	} while (!reachable(state, output, dimension));

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
double maxReward(Eigen::MatrixXd Q, std::pair<int, int> state, int dimension)
{
	double maxR = 0;
	std::pair<int, int> action;
	action = std::make_pair(-1, 0);
	if (reachable(state, action, dimension))
	{
		if (maxR < Q(state.first + action.first, state.second + action.second)){ maxR = Q(state.first + action.first, state.second + action.second); }
	}
	action = std::make_pair(0, -1);
	if (reachable(state, action, dimension))
	{
		if (maxR < Q(state.first + action.first, state.second + action.second)){ maxR = Q(state.first + action.first, state.second + action.second); }
	}
	action = std::make_pair(1, 0);
	if (reachable(state, action, dimension))
	{
		if (maxR < Q(state.first + action.first, state.second + action.second)){ maxR = Q(state.first + action.first, state.second + action.second); }
	}
	action = std::make_pair(0, 1);
	if (reachable(state, action, dimension))
	{
		if (maxR < Q(state.first + action.first, state.second + action.second)){ maxR = Q(state.first + action.first, state.second + action.second); }
	}

	return maxR;
}
int main()
{
	const double alpha = 1;
	const double gamma = 0.9;
	const int dimension = 100;
	/*coefficients du polynome.
	n°ligne = puissance de x
	n°colonne = puissance de y*/
	

	//creation matrices de qualité
	Eigen::MatrixXd Q[3];
	//initialisation des matrices
	Q[1].setConstant(0);
	Q[2].setConstant(0);
	Q[3].setConstant(0);
	//creation de l'état (position de l'agent)
	std::pair<int, int> state;
	std::uniform_int_distribution<> dis(0, dimension);
	//initialisation aléatoire de la position
	state.first = dis(gen);	
	state.second = dis(gen);

	for (int i = 0; i < 10000; i++)
	{
		//choix de l'action au hasard
		std::pair<int, int> action1 = randomAction(state, dimension);

		//récompense

		int s = 100 * state.first + state.second;
		int a = 100 * action1.first + action1.second;
		int s2 = a + s;
		std::pair<int, int> S2;
		S2.first = state.first + action1.first;
		S2.second = state.second + action1.second;
		double r;
		if (action1.first != 0){r = -(value(m1, state.first, state.second) + value(m1, state.first + action1.first, state.second)) / 2;}
		else{r = -(value(m1, state.first, state.second) + value(m1, state.first, state.second + action1.second)) / 2;}

		Q[1](s, a) = (1 - alpha)*Q[1](s, a) + gamma*(r + maxReward(Q[1], S2, dimension));
		state.first += action1.first;
		state.second += action1.second;

		if (state.first == dimension - 1 && state.second == dimension - 1)
		{
			state = std::make_pair(0, 0);
		}

	}
	std::ofstream file("log.csv");

	file.close();

	system("pause");
	return 0;
}