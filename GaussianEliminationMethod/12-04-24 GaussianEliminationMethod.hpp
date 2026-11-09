#pragma once
/*
	GaussianEliminationMethod.hpp
	
	Solving of Linear Equations Systems with the Gauss Method
*/
#include	<iostream>
using std::cout; using std::endl; using std::boolalpha;
#include <iomanip>
#include <limits>
using std::string;
using std::numeric_limits;
#include <cassert>
#include <cstring>

double CalcPolynom3Order	(	std::vector<double> coefficients
							, 	double x) {
	// cout << "-------------- I'm " << __func__ << "(std::vector<vector> coefficients, double x)" << endl;
		
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
std::vector<double>	GaussianEliminationMethodFor3Order	(	std::vector<double>	x_arr
														, 	std::vector<double> y_arr
														)	{
	cout << "-------------- I'm " << __func__ << endl;
	//	0.
	// cout << "Готовим матрицу коффициентов для системы линейных уравнений" << endl;
	std::vector<std::vector<double>>	matrix_0(4, std::vector<double>(5));
	for (auto idx_outer = 0; idx_outer != 4; ++idx_outer)
		for (auto idx_inner = 0; idx_inner != 3; ++idx_inner)		{	
			matrix_0[idx_outer][idx_inner]	=  	pow(x_arr[idx_outer], (3 - idx_inner));
			matrix_0[idx_outer][3]			=	1.0;
			matrix_0[idx_outer][4]			=	y_arr[idx_outer];
		}
	//	1.
	// cout << "Вытесним из всех четырех уравнений первые коэффициенты" << endl;
	std::vector<std::vector<double>>	matrix_1{matrix_0}; 

	for (auto idx_outer = 0; idx_outer != 3; ++idx_outer) 
		for (auto idx_inner = 4; idx_inner >= 0; --idx_inner)		{	
			matrix_1[idx_outer][idx_inner]		/=  matrix_1[0][0];
			matrix_1[idx_outer+1][idx_inner]	/= 	matrix_1[idx_outer+1][0];
			matrix_1[idx_outer+1][idx_inner]	-=	matrix_1[0][idx_inner];
		}
	//	2.
	// cout << "Вытесним из трех нижних уравнений вторые коэффициенты" << endl;
	std::vector<std::vector<double>>	matrix_2{matrix_1};

	for (auto idx_outer = 1; idx_outer != 3; ++idx_outer)
		for (auto idx_inner = 4; idx_inner >= 1; --idx_inner)		{	
			matrix_2[idx_outer][idx_inner]		/=  matrix_2[1][1];
			matrix_2[idx_outer+1][idx_inner]	/= 	matrix_2[idx_outer+1][1];
			matrix_2[idx_outer+1][idx_inner]	-=	matrix_2[1][idx_inner];
		}	
	//	3.
	// cout << "Вытесним из двух нижних уравнений третьи коэффициенты" << endl;
	std::vector<std::vector<double>>	matrix_3{matrix_2};

	for (auto idx_outer = 2; idx_outer != 3; ++idx_outer)
		for (auto idx_inner = 4; idx_inner >= 2; --idx_inner)		{	
			matrix_3[idx_outer][idx_inner]		/=  matrix_3[2][2];
			matrix_3[idx_outer+1][idx_inner]	/= 	matrix_3[idx_outer+1][2];
			matrix_3[idx_outer+1][idx_inner]	-=	matrix_3[2][idx_inner];
		}
	//	4.
	// cout << "Вытесним из нижнего уравнения четвертый коэффициент" << endl;
	std::vector<std::vector<double>>	matrix_4{matrix_3};

	matrix_4[3][4]		/=  matrix_4[3][3];
	matrix_4[3][3]		/=  matrix_4[3][3];
		
	// cout << "Обратный ход алгоритма - формирование вектора коэффициентов" << endl;
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
using namespace std;
// Вывод системы уравнений
void sysout	(	double** 	a
			, 	double* 	y
			, 	int 		n			// Размерность массива
			)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			cout << a[i][j] << "*x" << j;
			if (j < n - 1)
				cout << " + ";
		}
		cout << " = " << y[i] << endl;
	}
	return;
}

double* 	gauss(double** a, double* y, int n)
{
	double	* 	x;
	double	 	max;
	int 		k;
	int 		index;
	
	const double eps = 0.00001;  	// точность - коэффициенты не могут быть меньше этой величины?
									// Но они могут быть и совсем пропущены?
	
	x = 	new double[n];
	k = 0;
	while (k < n)	{
		// Поиск строки с максимальным a[i][k]
		max 	= abs(a[k][k]);
		index 	= k;
		for (int i = k + 1; i < n; i++)		{
			if (abs(a[i][k]) > max)	{
				max = abs(a[i][k]);
				index = i;
			}
		}
		// Перестановка строк
		if (max < eps)    {
			// нет ненулевых диагональных элементов
			cout << "Решение получить невозможно из-за нулевого столбца ";
			cout << index << " матрицы A" << endl;
			return 0;
		}
		for (int j = 0; j < n; j++)		{
			double temp = a[k][j];
		
			a[k][j] 	= a[index][j];
			a[index][j] = temp;
		}
		double 	temp 	= y[k];
		y[k] 			= y[index];
		y[index] 		= temp;
		// Нормализация уравнений
		for (int i = k; i < n; i++)		{
			double 	temp = a[i][k];
		
			if (abs(temp) < eps) 
				continue; 					// для нулевого коэффициента пропустить
			for (int j = k; j < n; j++)
				a[i][j] = a[i][j] / temp;
		
			y[i] = y[i] / temp;
		
			if (i == k)  
				continue; 					// уравнение не вычитать само из себя
			for (int j = 0; j < n; j++)
				a[i][j] = a[i][j] - a[k][j];
      
			y[i] = y[i] - y[k];
		}
		k++;
	}
	// обратная подстановка
	for (k = n - 1; k >= 0; k--)	{
		x[k] = y[k];
		for (int i = 0; i < k; i++)
			y[i] = y[i] - a[i][k] * x[k];
	}
	return 	x;
}	// End double* 	gauss(double** a, double* y, int n)

int main()
{
	double** a, * y, * x;
	int n;
	system("chcp 1251");
	system("cls");
	cout << "Введите количество уравнений: ";
	cin >> n;
	a = new double* [n];
	y = new double[n];
	for (int i = 0; i < n; i++)	{
		a[i] = new double[n];
		for (int j = 0; j < n; j++)    {
			cout << "a[" << i << "][" << j << "]= ";
			cin >> a[i][j];
		}
	}
	for (int i = 0; i < n; i++)  {
		cout << "y[" << i << "]= ";
		cin >> y[i];
	}
	sysout(a, y, n);
	
	x = gauss(a, y, n);
	
	for (int i = 0; i < n; i++)
		cout << "x[" << i << "]=" << x[i] << endl;
	
	for (int i = 0; i < n; i++)
		delete[] a[i];
	
	delete[] a;
	delete[] y;
	delete[] x;
	
	cin.get(); cin.get();
	
	return 0;
}
*/
