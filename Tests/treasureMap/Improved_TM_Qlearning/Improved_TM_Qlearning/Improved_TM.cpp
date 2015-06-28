#include <Eigen\Eigen>
#include <iostream>
#include <fstream>
#include <random>

std::random_device rd;
std::mt19937 gen(rd());
//ensemble des parametres modifables
const double alpha = 0.9;
const double gamma = 0.85;
static const int datasize = 20; //taille de la carte
const int dimension = datasize; 
const int qdim = dimension*dimension;// nb d'états
const double niter = 1e5; // nb total d'itération
const double treasureReward = 1000; // valeur du trésor
const int init = 0; //initialisation matrices de qualité

double m1data[] = { 0.24657, 0.17179, 0.038265, -0.13313, -0.28582, -0.33684, -0.26323, -0.15434, -0.11914, -0.16229, -0.20573, -0.2017, -0.15819, -0.0788, 0.050236, 0.21142, 0.33254, 0.33436, 0.21819, 0.09324,
0.11087, 0.073258, 0.00098544, -0.1035, -0.20755, -0.24532, -0.18853, -0.10021, -0.067608, -0.10112, -0.14494, -0.15873, -0.13609, -0.069618, 0.046796, 0.18696, 0.29025, 0.30071, 0.2234, 0.13796,
-0.066879, -0.058019, -0.051736, -0.068434, -0.1081, -0.12921, -0.096145, -0.03507, -0.0039162, -0.023034, -0.068801, -0.11036, -0.11922, -0.071404, 0.027635, 0.137, 0.20982, 0.22266, 0.18876, 0.15078,
-0.20358, -0.16545, -0.10569, -0.058428, -0.049682, -0.059161, -0.044687, -0.00018725, 0.039355, 0.03955, -0.0080009, -0.07982, -0.12384, -0.097972, -0.017355, 0.061046, 0.098317, 0.098542, 0.086119, 0.078018,
-0.26869, -0.22725, -0.15447, -0.084992, -0.057461, -0.063784, -0.056125, -0.0079067, 0.055482, 0.082175, 0.037639, -0.059113, -0.13729, -0.13884, -0.081436, -0.031659, -0.026326, -0.046505, -0.059162, -0.058353,
-0.28204, -0.25227, -0.19115, -0.1269, -0.10185, -0.11236, -0.10449, -0.040167, 0.054791, 0.11159, 0.078842, -0.026714, -0.12669, -0.15666, -0.13104, -0.11313, -0.13474, -0.17143, -0.18884, -0.18682,
-0.27457, -0.25543, -0.20603, -0.14959, -0.13139, -0.14981, -0.14408, -0.067609, 0.051191, 0.13284, 0.12098, 0.032544, -0.061257, -0.10767, -0.12013, -0.14511, -0.19696, -0.24556, -0.2621, -0.25597,
-0.2446, -0.22997, -0.18449, -0.12992, -0.11599, -0.14341, -0.14523, -0.067329, 0.0608, 0.15634, 0.16843, 0.11763, 0.057567, 0.01248, -0.037905, -0.11624, -0.20776, -0.27193, -0.28772, -0.27698,
-0.16729, -0.15678, -0.11736, -0.06928, -0.062884, -0.099238, -0.10827, -0.032908, 0.095355, 0.19359, 0.22069, 0.20407, 0.18092, 0.14503, 0.06556, -0.059921, -0.19069, -0.27792, -0.30605, -0.30151,
-0.054711, -0.050497, -0.025085, 0.0046447, -0.0035468, -0.046036, -0.0547, 0.021801, 0.14673, 0.23785, 0.26259, 0.2553, 0.24615, 0.21379, 0.12325, -0.018345, -0.16529, -0.27539, -0.33509, -0.35645,
0.034994, 0.036462, 0.048347, 0.056296, 0.032901, -0.0093711, -0.0095422, 0.070367, 0.18789, 0.26475, 0.27413, 0.25213, 0.22919, 0.18939, 0.10805, -0.0067164, -0.1297, -0.24648, -0.34607, -0.40786,
0.068048, 0.072331, 0.077682, 0.070599, 0.039079, 0.0033901, 0.013575, 0.092521, 0.1964, 0.2564, 0.24906, 0.20406, 0.15227, 0.09682, 0.036871, -0.019303, -0.080291, -0.17344, -0.29637, -0.39302,
0.07136, 0.075279, 0.073873, 0.057959, 0.02718, 0.002534, 0.020181, 0.091413, 0.17711, 0.22124, 0.20156, 0.13636, 0.055299, -0.01601, -0.051084, -0.039508, -0.019914, -0.058458, -0.16778, -0.27242,
0.078048, 0.071157, 0.055727, 0.034603, 0.014006, 0.0062015, 0.031006, 0.092879, 0.16125, 0.19171, 0.16138, 0.079066, -0.023945, -0.10267, -0.11236, -0.043395, 0.048787, 0.077999, 0.017183, -0.062986,
0.075165, 0.05514, 0.028758, 0.013166, 0.015212, 0.032345, 0.068352, 0.12492, 0.18096, 0.2001, 0.15742, 0.058118, -0.058969, -0.13502, -0.12233, -0.019957, 0.11273, 0.19318, 0.18893, 0.14956,
0.026771, 0.0096372, -0.0034165, 0.0090794, 0.047634, 0.095455, 0.14513, 0.19902, 0.24529, 0.25315, 0.19594, 0.080214, -0.044534, -0.11244, -0.085909, 0.020611, 0.15107, 0.24232, 0.26847, 0.25877,
-0.076308, -0.065515, -0.032856, 0.031307, 0.11604, 0.19582, 0.25835, 0.30791, 0.34033, 0.33073, 0.25456, 0.12398, -0.0023073, -0.059186, -0.02618, 0.065667, 0.165, 0.22993, 0.24771, 0.24065,
-0.20493, -0.15272, -0.059553, 0.063574, 0.19503, 0.30663, 0.38391, 0.42973, 0.4429, 0.40505, 0.30045, 0.15282, 0.028254, -0.015113, 0.024966, 0.10479, 0.17632, 0.20993, 0.2023, 0.18041,
-0.31625, -0.22986, -0.088796, 0.079267, 0.24776, 0.38927, 0.48606, 0.53422, 0.52936, 0.45787, 0.31725, 0.14764, 0.022572, -0.0082872, 0.042648, 0.12518, 0.19339, 0.21901, 0.19852, 0.16476,
-0.38016, -0.27693, -0.1128, 0.077106, 0.26604, 0.42791, 0.54141, 0.59537, 0.58027, 0.4844, 0.31563, 0.12751, -0.00044445, -0.024103, 0.035895, 0.12757, 0.20644, 0.24108, 0.22282, 0.18655 };

const Eigen::Matrix<double, datasize, datasize, Eigen::RowMajor> m1(m1data);

using State = std::pair<int, int>;

//permet de vérifier si une action est réalisable à partir d'un état donné (bords)
bool reachable(State state, State action, int dimension)
{
	int f = state.first + action.first;
	int s = state.second + action.second;
	return (f < dimension && f >= 0 && s < dimension && s >= 0);
}

/*convertit les valeurs 0,1,2 et 3 en une pair correspondant à une action
** 0 = vers le haut --> (-1,0)
** 1 = vers la gauche --> (0,-1)
** 2 = vers le bas --> (1,0)
** 3 = vers la droite --> (0,1)
*/
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

//Génère une action aléatoire pour l'exploration (tout en vérifiant la faisabilité de celle-ci)
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

//évalution en un point du polynome qui définit la carte 
double value(const Eigen::Matrix<double, datasize, datasize>& poly, int x, int y)
{
	return poly(x, y);
	double value = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			value += poly(i, j)*pow(x, i)*pow(y, j);
		}
	}
	return value;
}

//recherche de la valeur maximale de récompense accessible depuis un état donné (permet de récupérer la valeur est l'action à réaliser pour l'obtenir.
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

//vérification de l'arrivée sur la case du trésor
bool isTreasure(const State& state, int dimension)
{
	return state.first == dimension - 1 && state.second == dimension - 1;
}

//Attribue le gain/coût pour se déplacer d'un état à un autre
// s0 : état de départ
// s1 : état d'arrivée
double getReward(const State& s0, const State& s1, int dimension)
{
	if (isTreasure(s1, dimension))
		return treasureReward;
	return -std::abs(value(m1, s0.first, s0.second) - value(m1, s1.first, s1.second));//coût pour se déplacer (fonction de la pente)
}

//calcule du coût sur le chemin optimal
double getCost(const Eigen::MatrixXd& Q, int dimension)
{

	State state = std::make_pair(0, 0);
	int maxIter = 200;
	double cost = 0;
	do
	{
		State s1;
		maxReward(Q, state, dimension, &s1);
		cost += getReward(state, s1, dimension);
		state = s1;
	} while (!isTreasure(state, dimension) && maxIter--);

	return cost;
}

int main()
{
	
	//creation matrices de qualité
		//exploration pure
	Eigen::MatrixXd Q(qdim, qdim);
		//Exploration + exploitation (proba constante)
	Eigen::MatrixXd Q2(qdim, qdim);
	//initialisation des matrices
	Q.setConstant(init);
	Q2.setConstant(init);
	//creation de l'état (position de l'agent)
	State state [2];

	std::uniform_int_distribution<> dis(0, dimension - 1);
	//initialisation aléatoire de la position
	state[0].first = dis(gen);	
	state[0].second = dis(gen);
	state[1].first = dis(gen);
	state[1].second = dis(gen);

	//creation de la variable action pour les 2 politiques
	std::pair<int, int> action[2];

	//creation du fichier de log des coûts durant l'apprentissage
	std::ofstream costFile("cost.csv");
	for (int i = 0; i < niter; i++)
	{
		//choix de l'action au hasard
		action[0] = randomAction(state[0], dimension);

		//choix de  l'action (exploration + exploitation)
		std::uniform_int_distribution<> expl(0, 999);
		double temp = expl(gen);
		if (temp >= 5) 
		{
			// choix de l'action au hasard
				action[1] = randomAction(state[1], dimension);
		}
		else //choix de l'action optimale
		{
			State newState;
			maxReward(Q2, state[1], dimension, &newState);
			action[1] = std::make_pair(newState.first - state[1].first, newState.second - state[1].second);
		}


		//récompense
		for (int j = 0; j < 2 ; j++)
		{
			int s = dimension * state[j].first + state[j].second;
			std::pair<int, int> S;
			S.first = state[j].first + action[j].first;
			S.second = state[j].second + action[j].second;
			int nextState = dimension * S.first + S.second;
			double r = getReward(state[j], S, dimension);

			if (j == 0) //si agent d'exploration pure
			{
				Q(s, nextState) = (1 - alpha)*Q(s, nextState) + gamma*(r + maxReward(Q, S, dimension));
			}
			else //si agent combinant exploration et exploitation
			{
				Q2(s, nextState) = (1 - alpha)*Q2(s, nextState) + gamma*(r + maxReward(Q2, S, dimension));
			}

			//mise à jour de la position de l'agent
			state[j] = S;

			if (isTreasure(state[j], dimension))//vérification de l'arrivée sur la case du trésor
			{
				std::cout << "finished" << std::endl;
				//l'agent est repositionné aléaoirement sur la carte
				state[j] = std::make_pair(dis(gen), dis(gen));
			}
		}
		if (!(i % 10))
		{
			//log toutes les 10 itérations du coût sur le chemin optimal découvert pour les 2 agents
			costFile << getCost(Q, dimension) << "," << getCost(Q2, dimension) << std::endl;
		}
	}
	//creation des fichiers de log pour le chemin optimal
	std::ofstream file("log_exploration.csv");
	std::ofstream file2("log_combinaison.csv");

	//après les itérations, on log le chemin otpimal obtenu pour le traçer dans MATLAB
	for (int i = 0; i < 2; i++)
	{
		double cost = 0;
		state[i] = std::make_pair(0, 0);
		int maxIter = 200;
		do
		{
			if (i == 0)//agent d'exploration
			{
				file << state[i].first << "," << state[i].second << std::endl;
				cost += maxReward(Q, state[i], dimension, &state[i]);
				std::cout << cost << std::endl;
			}
			if (i == 1)//agent d'exploration/exploitation
			{
				file2 << state[i].first << "," << state[i].second << std::endl;
				cost += maxReward(Q2, state[i], dimension, &state[i]);
				std::cout << cost << std::endl;
			}

			
		} while (!isTreasure(state[i], dimension) && maxIter--);

		
	}
	file.close();
	file2.close();

	
	system("pause");
	return 0;
}