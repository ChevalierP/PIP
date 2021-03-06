#ifndef EXEMPLE_H
#define EXEMPLE_H

#include <Eigen\Eigen>

static const int m1data[] = { 1, 20, 1, 1, 1,
							  1, 20, 1, 20, 1,
							  1, 20, 1, 20, 1,
							  1, 20, 1, 20, 1,
							  1, 1, 1, 20, 1 };
static const int m1size = 5;
static const Eigen::Matrix<int, m1size, m1size, Eigen::RowMajor> m1(m1data);


#endif