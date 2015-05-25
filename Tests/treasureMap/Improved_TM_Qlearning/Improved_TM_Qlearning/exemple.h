#ifndef EXEMPLE_H
#define EXEMPLE_H

#include <Eigen\Eigen>

static const int m1size = 10;
static const int dim = 100;
static const double gamma = 0.9;
static const int m1data[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
							 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
							 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
							 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
							 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
							 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
							 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
							 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };

Eigen::Matrix<int, m1size, m1size, Eigen::RowMajor> m1(m1data);

double value(Eigen::Matrix<double, m1size, m1size> M, int x, int y);
Eigen::Matrix<int, m1size, m1size> polyDerive(Eigen::Matrix<double, m1size, m1size> M, int dir); // dir = 0 --> x dir = 1 --> y
std::pair<int, int> randomAction(std::pair<int, int> state);
bool reachable(int state, int action);
std::pair<std::pair<int, int>, double> maxOfRow(Eigen::Matrix<double, m1size, m1size> Q, std::pair<int, int> state, Eigen::Matrix<double, m1size, m1size> dX, Eigen::Matrix<double, m1size, m1size> dY);
#endif