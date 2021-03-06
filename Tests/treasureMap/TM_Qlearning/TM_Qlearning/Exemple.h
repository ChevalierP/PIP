#ifndef EXEMPLE_H
#define EXEMPLE_H

#include <Eigen\Eigen>

//paramètres modifiables
static const int size = 5;
static const double gamma = 0.9;
static const int m1size = size*size;
static const int min = 0;
static const int max = m1size - 1;
static const int wallsReward = -58;
static const int caseReward = -1;
static const int treasureReward = 100;
static const char m1data[] = { 'c', 'w', '-', '-', '-', 'c', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
					/*2*/	   '-', 'w', 'c', '-', '-', '-', 'w', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', 
					/*3*/	   '-', 'w', 'c', 'c', '-', '-', '-', 'c', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
					/*4*/	   '-', '-', 'c', 'c', 'c', '-', '-', '-', 'w', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
					/*5*/	   '-', '-', '-', 'c', 'c', '-', '-', '-', '-', 'c', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
					/*6*/	   'c', '-', '-', '-', '-', 'c', 'w', '-', '-', '-', 'c', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
					/*7*/	   '-', 'w', '-', '-', '-', 'c', 'w', 'c', '-', '-', '-', 'w', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
					/*8*/	   '-', '-', 'c', '-', '-', '-', 'w', 'c', 'w', '-', '-', '-', 'c', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
					/*9*/	   '-', '-', '-', 'c', '-', '-', '-', 'c', 'w', 'c', '-', '-', '-', 'w', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
					/*10*/	   '-', '-', '-', '-', 'c', '-', '-', '-', 'w', 'c', '-', '-', '-', '-', 'c', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
					/*11*/	   '-', '-', '-', '-', '-', 'c', '-', '-', '-', '-', 'c', 'w', '-', '-', '-', 'c', '-', '-', '-', '-', '-', '-', '-', '-', '-',
					/*12*/	   '-', '-', '-', '-', '-', '-', 'w', '-', '-', '-', 'c', 'w', 'c', '-', '-', '-', 'w', '-', '-', '-', '-', '-', '-', '-', '-',
					/*13*/	   '-', '-', '-', '-', '-', '-', '-', 'c', '-', '-', '-', 'w', 'c', 'w', '-', '-', '-', 'c', '-', '-', '-', '-', '-', '-', '-',
					/*14*/	   '-', '-', '-', '-', '-', '-', '-', '-', 'w', '-', '-', '-', 'c', 'w', 'c', '-', '-', '-', 'w', '-', '-', '-', '-', '-', '-',
					/*15*/	   '-', '-', '-', '-', '-', '-', '-', '-', '-', 'c', '-', '-', '-', 'w', 'c', '-', '-', '-', '-', 'c', '-', '-', '-', '-', '-',
					/*16*/	   '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', 'c', '-', '-', '-', '-', 'c', 'w', '-', '-', '-', 'c', '-', '-', '-', '-',
					/*17*/	   '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', 'w', '-', '-', '-', 'c', 'w', 'c', '-', '-', '-', 'c', '-', '-', '-',
					/*18*/	   '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', 'c', '-', '-', '-', 'w', 'c', 'w', '-', '-', '-', 'c', '-', '-',
					/*19*/	   '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', 'w', '-', '-', '-', 'c', 'w', 'c', '-', '-', '-', 'w', '-',
					/*20*/	   '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', 'c', '-', '-', '-', 'w', 'c', '-', '-', '-', '-', 'T',
					/*21*/	   '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', 'c', '-', '-', '-', '-', 'c', 'c', '-', '-', '-',
					/*22*/	   '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', 'w', '-', '-', '-', 'c', 'c', 'c', '-', '-',
					/*23*/	   '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', 'c', '-', '-', '-', 'c', 'c', 'w', '-',
					/*24*/	   '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', 'w', '-', '-', '-', 'c', 'w', 'T',
					/*25*/	   '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', 'c', '-', '-', '-', 'w', 'c' };

Eigen::Matrix<char, m1size, m1size, Eigen::RowMajor> m1(m1data);


bool reachable(int i, int j);
int randomAction(int state);
int rewardCharToInt(int state, int action);
std::pair<double, int> maxOfRow(Eigen::Matrix<double, m1size, m1size> M, int state);
#endif