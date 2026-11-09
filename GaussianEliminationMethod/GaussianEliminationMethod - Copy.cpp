/*
	GaussianEliminationMethod.cpp
	
	Solving of Systems of Linear Equations with the Gauss-Jordan Method
*/
#include	<iostream>
using std::cout; using std::endl; using std::boolalpha;
#include <iomanip>
#include <limits>
#include <cmath>
using std::string;
using std::numeric_limits;
#include <cassert>
#include <cstring>
#include "GaussianEliminationMethod.hpp"

#include	<iostream>
using std::cout; using std::endl; using std::boolalpha;
#include <iomanip>
#include <limits>
using std::string;
using std::numeric_limits;
#include <cassert>
#include <cstring>
#include <vector>

#include "GaussianEliminationMethod.hpp"

double CalcPolygon3Order	(	std::vector<double> coefficients
							, 	double x) {
	cout << "-------------- I'm " << __func__ << "(std::vector<vector> coefficients, double x)" << endl;
		
	return	coefficients[3] 								//	d
			+ 	x * (coefficients[2] 						//	c
					+	x * (coefficients[1] 				//	b
							+ 	x * coefficients[0]			// 	a
							)
					);
};
/*
	**************************************************************************************
*/
std::vector<double>	GaussianEliminationMethod	(	std::vector<double>	x_arr
												, 	std::vector<double> y_arr
												)	{
	cout << "-------------- I'm " << __func__ << endl;
	//	0.
	cout << "Готовим матрицу коффициентов для системы линейных уравнений" << endl;
	std::vector<std::vector<double>>	matrix_0(4, std::vector<double>(5));
	for (auto idx_outer = 0; idx_outer != 4; ++idx_outer)
		for (auto idx_inner = 0; idx_inner != 3; ++idx_inner)		{	
			matrix_0[idx_outer][idx_inner]	=  	pow(x_arr[idx_outer], (3 - idx_inner));
			matrix_0[idx_outer][3]			=	1.0;
			matrix_0[idx_outer][4]			=	y_arr[idx_outer];
		}
	for (std::vector<double>& arr_inner : matrix_0)	{
		for (double& arr_elem :  arr_inner)
			cout << arr_elem << "  ";
		cout << endl;
	}
	//	1.
	cout << "Вытесним из всех четырех уравнений первые коэффициенты" << endl;
	std::vector<std::vector<double>>	matrix_1{matrix_0}; 

	for (auto idx_outer = 0; idx_outer != 3; ++idx_outer) 
		for (auto idx_inner = 4; idx_inner >= 0; --idx_inner)		{	
			matrix_1[idx_outer][idx_inner]		/=  matrix_1[0][0];
			matrix_1[idx_outer+1][idx_inner]	/= 	matrix_1[idx_outer+1][0];
			matrix_1[idx_outer+1][idx_inner]	-=	matrix_1[0][idx_inner];
		}
	for (std::vector<double>& arr_inner : matrix_1)	{
		for (double& arr_elem :  arr_inner)
			cout << arr_elem << "  ";
		cout << endl;
	}
	//	2.
	cout << "Вытесним из трех нижних уравнений вторые коэффициенты" << endl;
	std::vector<std::vector<double>>	matrix_2{matrix_1};

	for (auto idx_outer = 1; idx_outer != 3; ++idx_outer)
		for (auto idx_inner = 4; idx_inner >= 1; --idx_inner)		{	
			matrix_2[idx_outer][idx_inner]		/=  matrix_2[1][1];
			matrix_2[idx_outer+1][idx_inner]	/= 	matrix_2[idx_outer+1][1];
			matrix_2[idx_outer+1][idx_inner]	-=	matrix_2[1][idx_inner];
		}	
	for (std::vector<double>& arr_inner : matrix_2)	{
		for (double& arr_elem :  arr_inner)
			cout << arr_elem << "  ";
		cout << endl;
	}
	//	3.
	cout << "Вытесним из двух нижних уравнений третьи коэффициенты" << endl;
	std::vector<std::vector<double>>	matrix_3{matrix_2};

	for (auto idx_outer = 2; idx_outer != 3; ++idx_outer)
		for (auto idx_inner = 4; idx_inner >= 2; --idx_inner)		{	
			matrix_3[idx_outer][idx_inner]		/=  matrix_3[2][2];
			matrix_3[idx_outer+1][idx_inner]	/= 	matrix_3[idx_outer+1][2];
			matrix_3[idx_outer+1][idx_inner]	-=	matrix_3[2][idx_inner];
		}
	for (std::vector<double>& arr_inner : matrix_3)	{
		for (double& arr_elem :  arr_inner)
			cout << arr_elem << "  ";
		cout << endl;
	}
	//	4.
	cout << "Вытесним из нижнего уравнения четвертый коэффициент" << endl;
	std::vector<std::vector<double>>	matrix_4{matrix_3};

	matrix_4[3][4]		/=  matrix_4[3][3];
	matrix_4[3][3]		/=  matrix_4[3][3];
		
	for (std::vector<double>& arr_inner : matrix_4)	{
		for (double& arr_elem :  arr_inner)
			cout << arr_elem << "  ";
		cout << endl;
	}

	cout << "Обратный ход алгоритма - формирование вектора коэффициентов" << endl;
	std::vector<double>	result(4);
	
	result[3] 	=	matrix_4[3][4];				//	d
	
	result[2] 	= 	matrix_4[2][4] 				//	c
				- 	matrix_4[2][3] * result[3];
	
	result[1] 	= 	matrix_4[1][4] 				//	b
				- 	matrix_4[1][3] * result[3] 
				- 	matrix_4[1][2] * result[2];
	
	result[0] 	= 	matrix_4[0][4] 				//	a
				- 	matrix_4[0][3] * result[3] 
				- 	matrix_4[0][2] * result[2] 
				- 	matrix_4[0][1] * result[1];
	
	return result;	
};

/*
	********************		main()		****************************************
*/
int 	main()
{
	cout << "I'm *******		GaussianEliminationMethod.cpp" << endl;
	
	std::vector<double>		x_arr{1.1, 2.2, 3.3, 4.4}; 
	std::vector<double> 	y_arr(4);
	std::vector<double> 	coefficients{2.0, 3.0, 4.0, 5.0};
	cout <<	"Определим значения фунции в котрольных точках оси абцисс OX" << endl;
	for (auto idx = 0; idx != 4; ++idx)
		y_arr[idx]	=	CalcPolygon3Order(coefficients, x_arr[idx]);
	for (auto elem : y_arr)
		cout << elem << "   ";
	cout << endl;
	//	Вектор вычисленных коффициетов
	std::vector<double> 	calced_coefficients(4);
	calced_coefficients		=	GaussianEliminationMethod(x_arr, y_arr); 	
	for (auto elem : calced_coefficients)
		cout << elem << "   ";
	cout << endl;
	cout <<	"Определим значения фунции в котрольных точках оси абцисс OX" << endl;
	for (auto idx = 0; idx != 4; ++idx)
		y_arr[idx]	=	CalcPolygon3Order(calced_coefficients, x_arr[idx]);
	for (auto elem : y_arr)
		cout << elem << "   ";
	cout << endl;
}