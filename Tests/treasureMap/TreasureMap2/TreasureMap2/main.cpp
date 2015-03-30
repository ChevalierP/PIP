#include <iostream>
#include <algorithm>    // std::min_element, std::max_element
#include <Eigen\Eigen>
#include <vector>
#include <math.h>
#include "exemple.h"

bool myfn(int i, int j) 
{ 
	if (i >= 0 && j >= 0)
	{ 
		return i < j; 
	}
	else return false;
}

int main()
{
	Eigen::Matrix<char, m1size, m1size> politiqueOptimale;
	Eigen::Matrix<char, m1size, m1size> coutsCumules;
	Eigen::Matrix<char, m1size, m1size> temporaire;
	Eigen::Matrix<int, m1size,m1size> m = m1;

	coutsCumules.setConstant(-1);
	coutsCumules(m1size - 1, m1size - 1) = 0;
	temporaire = coutsCumules;
	politiqueOptimale.setConstant('-');

	int haut = 0, bas = 0, gauche = 0, droite = 0;
	std::vector<int> vect;

	for (int i = 0; i < 1000; i++)
	{
		for (int j = 0; j < m1size - 1; j++)
		{
			for (int k = 0; k < m1size - 1; k++)
			{
				if (j - 1 > 0)
				{ 
					if (coutsCumules(j - 1, k) > 0) 
					{ 
						haut = m(j - 1, k) + coutsCumules(j - 1, k);
						vect.push_back(haut);
					}
				}
				if (k - 1 > 0)
				{
					if (coutsCumules(j, k - 1) > 0) 
					{
						gauche = m(j, k - 1) + coutsCumules(j, k - 1);
						vect.push_back(gauche);
					}
				}
				if (j + 1 < m1size - 1)
				{
					if (coutsCumules(j + 1, k) > 0) 
					{ 
						bas = m(j + 1, k) + coutsCumules(j + 1, k);
						vect.push_back(bas);
					}
				}
				if (k + 1 < m1size - 1)
				{
					if (coutsCumules(j , k + 1) > 0) 
					{ 
						droite = m(j, k + 1) + coutsCumules(j, k + 1);
						vect.push_back(droite);
					}
				}
				
				temporaire(j, k) = *std::min_element(vect.begin(), vect.end(), myfn);
			}
		}
		coutsCumules = temporaire;
	}

	std::cout << politiqueOptimale << '\n' << '\n' << coutsCumules << std::endl;

	return 0;
}