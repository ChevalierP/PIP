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
	Eigen::Matrix<int, m1size, m1size> coutsCumules;
	Eigen::Matrix<int, m1size, m1size> temporaire;
	Eigen::Matrix<int, m1size,m1size> m = m1;

	coutsCumules.setConstant(-1);
	coutsCumules(m1size - 1, m1size - 1) = 1;
	temporaire = coutsCumules;
	politiqueOptimale.setConstant('-');

	std::vector<int> vect;

	for (int i = 0; i < 2; i++)
	{
		temporaire = coutsCumules;
		for (int j = 0; j < m.rows() ; j++)
		{
			for (int k = 0; k < m.cols() ; k++)
			{
				if (j - 1 > 0)
				{ 
					if (coutsCumules(j - 1, k) > 0) 
					{ 
						int haut = m(j - 1, k) + coutsCumules(j - 1, k);
						vect.push_back(haut);
					}
				}
				if (k - 1 > 0)
				{
					if (coutsCumules(j, k - 1) > 0) 
					{
						int gauche = m(j, k - 1) + coutsCumules(j, k - 1);
						vect.push_back(gauche);
					}
				}
				if (j + 1 < m.rows())
				{
					if (coutsCumules(j + 1, k) > 0) 
					{ 
						int bas = m(j + 1, k) + coutsCumules(j + 1, k);
						vect.push_back(bas);
					}
				}
				if (k + 1 < m.cols() )
				{
					if (coutsCumules(j , k + 1) > 0) 
					{ 
						int droite = m(j, k + 1) + coutsCumules(j, k + 1);
						vect.push_back(droite);
					}
				}
				if (vect.size() > 0)
				{
					vect.push_back(coutsCumules(j, k));
					temporaire(j, k) = *std::min_element(vect.begin(), vect.end(), myfn);
					std::cout << temporaire << '\n' << std::endl;
				}
				vect.clear();
			}
		}
		coutsCumules = temporaire;
	}

	std::cout << coutsCumules << std::endl;

	return 0;
}