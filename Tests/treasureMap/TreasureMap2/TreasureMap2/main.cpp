#include <iostream>
#include <fstream>
#include <algorithm>    // std::min_element, std::max_element
#include <Eigen\Eigen>
#include <vector>
#include <math.h>
#include "exemple.h"

using Pair = std::pair < char, int > ;

bool myfn(Pair i, Pair j) 
{ 
	if (i.second > 0 && j.second > 0)
	{ 
		return i.second < j.second; 
	}
	else return false;
}

int main()
{
	Eigen::Matrix<char, m1size, m1size> politiqueOptimale;
	Eigen::Matrix<int, m1size, m1size> coutsCumules;
	Eigen::Matrix<int, m1size, m1size> temporaire;
	Eigen::Matrix<int, m1size, m1size> m = m1;

	std::cout << m << '\n' << std::endl;

	coutsCumules.setConstant(-1);
	coutsCumules(m1size - 1, m1size - 1) = 1;
	temporaire = coutsCumules;
	politiqueOptimale.setConstant('-');
	politiqueOptimale(politiqueOptimale.rows() - 1, politiqueOptimale.cols() - 1) = 'f';

	std::vector<std::pair<char, int>> vect;
	std::ofstream file("log.txt");


	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < m.rows(); j++)
		{
			for (int k = 0; k < m.cols(); k++)
			{
				if (politiqueOptimale(j, k) == 'f') continue;
				if (j > 0)
				{
					if (coutsCumules(j - 1, k) > 0)
					{
						int haut = m(j - 1, k) + coutsCumules(j - 1, k);
						vect.push_back(std::make_pair('h', haut));
					}
				}
				if (k > 0)
				{
					if (coutsCumules(j, k - 1) > 0)
					{
						int gauche = m(j, k - 1) + coutsCumules(j, k - 1);
						vect.push_back(std::make_pair('g', gauche));
					}
				}
				if (j + 1 < m.rows())
				{
					if (coutsCumules(j + 1, k) > 0)
					{
						int bas = m(j + 1, k) + coutsCumules(j + 1, k);
						vect.push_back(std::make_pair('b', bas));
					}
				}
				if (k + 1 < m.cols())
				{
					if (coutsCumules(j, k + 1) > 0)
					{
						int droite = m(j, k + 1) + coutsCumules(j, k + 1);
						vect.push_back(std::make_pair('d', droite));
					}
				}
				if (vect.size() > 0)
				{
					Pair pair_optimale = *std::min_element(vect.begin(), vect.end(), myfn);
					temporaire(j, k) = pair_optimale.second;
					politiqueOptimale(j, k) = pair_optimale.first;

					file << coutsCumules << std::endl << std::endl;

					for (Pair p : vect)
					{
						file << p.second << ' ' << p.first << ' ';
					}
					file << std::endl;
					file << i << ' ' << j << ' ' << k << std::endl;

					file << politiqueOptimale << std::endl << std::endl;
					//std::cout << '\n' << coutsCumules << '\n \n' << politiqueOptimale << '\n' << std::endl;

				}
				vect.clear();
			}
		}
		coutsCumules = temporaire;
	}

	file.close();
	return 0;
}