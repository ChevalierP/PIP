#include <iostream>
#include <vector>

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
	std::vector<int> vect;
	vect.push_back(1);
	vect.push_back(3);
	vect.push_back(-1);
	vect.push_back(9);

	*std::min_element(vect, vect + vect.size, myfn);
}