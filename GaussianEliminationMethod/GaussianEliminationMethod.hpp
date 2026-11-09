#pragma once
//	GaussianEliminationMethod.hpp			
//  	*************************************************************************
//  	Sergej Edgar Heckel
//  	10.10.2024
//	
//	Solving of Linear Equations Systems with the Gauss Method
//  	*************************************************************************
//
#include	<iostream>
using std::cout; using std::endl; using std::boolalpha;
#include <limits>
using std::numeric_limits;
#include <cmath>
#include <string>
using std::string;
#include <vector>
//	****************	CalcPolynom3Order() ***********************************************
double CalcPolynom3Order	(	std::vector<double> 	coefficients
						, 	double 				x
						) 
{
	return	coefficients[3] 									//	d
			+ 	x * (coefficients[2] 						//	c
					+	x * (coefficients[1] 				//	b
							+ 	x * coefficients[0]			// 	a
							)
					);
};
//	****************	CalcPolynomAnyOrder() ***********************************************
double CalcPolynomAnyOrder	(	int					polynom_order
							,	std::vector<double> coefficients
							, 	double 				x) 
{
	double result	=	0.0; 								
	for(auto idx = 0; idx != polynom_order; ++idx)
		result 	=	x *  (coefficients[idx] + result);
	
	return	result	+	coefficients[polynom_order];
};
//	****************	GaussianEliminationMethodForAnyOrder() ***********************************************
std::vector<double>	GaussianEliminationMethodForAnyOrder	(	int					polynom_order
														,	std::vector<double>	x_arr
														, 	std::vector<double> 	y_arr
														)	
{
	cout << "Preparing a coefficient matrix for linear model systems" << endl;
	std::vector<std::vector<double>>	matrix	(	polynom_order + 1
											, 	std::vector<double>(polynom_order + 2)
											);
	for (	auto 	idx_outer = 0
		; 			idx_outer != polynom_order + 1
		; 			++idx_outer
		)	
		{
			for (	auto 	idx_inner = 0
				; 			idx_inner != polynom_order
				; 			++idx_inner
				)	
				{	
					matrix[idx_outer][idx_inner]		=  	pow(x_arr[idx_outer], (polynom_order - idx_inner));
				}
			matrix[idx_outer][polynom_order]			=	1.0;
			matrix[idx_outer][polynom_order + 1]		=	y_arr[idx_outer];
		}	
	int 		order						=	polynom_order;	// Polynom's order
	int 		coefficient_quantity			=	order + 1;		// Polynom terms' number (respectively, coefficients)
	double	ERROR_MINIMUM 				= 0.000001;
	auto  	equation_vector{matrix[0]};
	bool		bad_coeff					= false;	
	
	cout << "Cycle by the number of equations in the linear equations system" << endl;
	for (	auto 	idx_equation = 0
		;  			idx_equation != order
		;			++idx_equation
		)
		{
			// 	Цикл перемещения, если возникнет необходимость, наверх строки, первой найденной 
			// 	с ненулевым главным коэффициентом
			equation_vector	=	matrix[idx_equation];
			bad_coeff		= 	false;	
			for	(	auto 	idx_sub_equation = idx_equation
				;  			idx_sub_equation != order + 1
				;			++idx_sub_equation
				)
				{
					auto column	=	idx_sub_equation;
					if (abs(matrix[idx_sub_equation][column]) > ERROR_MINIMUM)
						break;
					cout << "В первом подуравнении главный коэффициент проблемный " << idx_sub_equation 
						<< " " << matrix[idx_sub_equation][column] << endl;
					// Переход к следующей строке
					equation_vector		=	matrix[idx_sub_equation];	// Запомним строку с нулевым главным
					
					auto	idx = idx_sub_equation + 1;
					if (abs(matrix[idx][column]) > ERROR_MINIMUM){
						matrix[idx - 1]	=	matrix[idx];
						matrix[idx]		=	equation_vector;
						cout << "Менялась строка " << idx << endl;
						break;
					};	
					cout << "В последующем подуравнении главный коэффициент проблемный " << idx 
						<< " " << matrix[idx_sub_equation][column] << endl;
					bad_coeff = true;		// Все главные коэффициенты нулевые
					//cout << "Проблема с уравнением " << idx << "  " << matrix[idx][column] << endl;
				}	
			// 	Если у всех подуравнений были нулевые главные коэффициненты, нужно переходить к следующей строке	
			if (bad_coeff == true)
				continue;	
			auto	The1stCoeff	=	matrix[idx_equation][idx_equation];
			// 	Здесь, возможно стоит проверить на нулевое (или близкое к нулю) значение будущего делителя
			// 	The1stCoeff == ERROR_MINIMUM == 0.000001; 
			// 	Или еще какое маленькое значение, которое может привести к ошибке.
			for (	auto 	idx_inner = coefficient_quantity
				; 			idx_inner >= idx_equation
				; 			--idx_inner
				)
				matrix[idx_equation][idx_inner]	/=  The1stCoeff;
				
			for (	auto 	idx_outer = idx_equation
				; 			idx_outer != order
				; 			++idx_outer
				) 
				{
					auto	TheNext1stCoeff	=	matrix[idx_outer + 1][idx_equation];
					// Здесь, возможно стоит проверить на нулевое (или близкое к нулю) значение будущего делителя
					// The1stCoeff == ERROR_MINIMUM == 0.000001; 
					// Или еще какое маленькое значение, которое может привести к ошибке.
					for (	auto 	idx_inner = coefficient_quantity
						; 			idx_inner >= idx_equation
						; 			--idx_inner
						)		
						{	
							matrix[idx_outer + 1][idx_inner]	/= 	TheNext1stCoeff;
							matrix[idx_outer + 1][idx_inner]	-=	matrix[idx_equation][idx_inner];
						}	
				}	
		}
	cout << "Let us eliminate the last coefficient from the lowest, last one in the equation system" << endl;
	matrix[order][coefficient_quantity]			/=  matrix[order][coefficient_quantity - 1];
	matrix[order][coefficient_quantity - 1]		/=  matrix[order][coefficient_quantity - 1];
	//	******************************************************************************************
	cout << "Algorithm's reverse - forming the vector of coefficients" << endl;
	// 	The polynomial coefficients' vector
	std::vector<double>	result(order + 1);
	
	for (	auto 	idx_order = order
		; 			idx_order >=0
		;			--idx_order
		)
		{
			// Calculation of the absolute term of an equation - rightmost, last, coefficient
			result[idx_order] 	= 	matrix[idx_order][coefficient_quantity];
			
			if (idx_order < order)	
			{
				double minuses	=	0.0;
				// The cycle of calculation coefficients, starting from the penultimate to the first
				for (	auto idx_coef_quant =  coefficient_quantity
					; 		 idx_coef_quant >= (idx_order + 1)
					; 		 --idx_coef_quant
					)
					{	
						minuses 	+= 	matrix[idx_order][idx_coef_quant] 
										* 	result[idx_coef_quant];		
					}
				result[idx_order] -= minuses;
			}
		}
	return result;	
};
//	****************	GaussianEliminationMethodFor3Order() ***********************************************
std::vector<double>	GaussianEliminationMethodFor3Order	(	std::vector<double>	x_arr
													, 	std::vector<double> 	y_arr
													)	
{
	//	0.
	// cout << "Preparing a coefficient matrix for linear model systems" << endl;
	std::vector<std::vector<double>>	matrix(3 + 1, std::vector<double>(3 + 2));
	for (	auto 	idx_outer = 	0
		; 			idx_outer != 	3 + 1
		; 			++idx_outer
		)		
		{
			for (	auto 	idx_inner = 	0
				; 			idx_inner != 	3
				; 			++idx_inner
				)	
				{	
					matrix[idx_outer][idx_inner]	=  	pow(x_arr[idx_outer], (3 - idx_inner));
				}
			matrix[idx_outer][3]			=	1.0;
			matrix[idx_outer][3 + 1]		=	y_arr[idx_outer];
		}	
	//	1.
	// cout << "Let us eliminate the first coefficients from all four equations" << endl;
	for (	auto 	idx_outer = 	0
		; 			idx_outer != 	3
		; 			++idx_outer
		) 
		for (	auto 	idx_inner = 	3 + 1
			; 			idx_inner >= 	0
			; 			--idx_inner
			)		
			{	
				matrix[idx_outer][idx_inner]		/=  matrix[0][0];
				matrix[idx_outer + 1][idx_inner]	/= 	matrix[idx_outer+1][0];
				matrix[idx_outer + 1][idx_inner]	-=	matrix[0][idx_inner];
			}
	//	2.
	// cout << "Let us eliminate the second coefficients from three lower equations" << endl;
	for (	auto 	idx_outer = 	1
		; 			idx_outer != 	3
		; 			++idx_outer
		)
		for (	auto 	idx_inner = 	3 + 1
			;			idx_inner >= 	1
			; 			--idx_inner
			)		
			{	
				matrix[idx_outer][idx_inner]		/=  matrix[1][1];
				matrix[idx_outer + 1][idx_inner]	/= 	matrix[idx_outer + 1][1];
				matrix[idx_outer + 1][idx_inner]	-=	matrix[1][idx_inner];
			}	
	//	3.
	// cout << "Let us eliminate the third coefficients from two lower equations" << endl;
	for (	auto 	idx_outer = 		2
		; 			idx_outer != 	3
		; 			++idx_outer
		)
		for (	auto 	idx_inner = 	3 + 1
			; 			idx_inner >= 2
			; 			--idx_inner
			)		
			{	
				matrix[idx_outer][idx_inner]		/=  matrix[2][2];
				matrix[idx_outer + 1][idx_inner]	/= 	matrix[idx_outer + 1][2];
				matrix[idx_outer + 1][idx_inner]	-=	matrix[2][idx_inner];
			}
	//	4.
	// cout << "Let us eliminate the fourth coefficient from the most lower equation" << endl;
	matrix[3][3 + 1]	/=  matrix[3][3];
	matrix[3][3]		/=  matrix[3][3];
		
	// cout << "Algorithm's reverse - forming the vector of coefficients" << endl;
	std::vector<double>	result(3 + 1);
	
	result[3] 	=	matrix[3][3 + 1];				//	d
	result[2] 	= 	matrix[2][3 + 1] 				//	c
				- 	matrix[2][3] * result[3]	
	result[1] 	= 	matrix[1][3 + 1] 				//	b
				- 	matrix[1][3] * result[3] 
				- 	matrix[1][2] * result[2];
	result[0] 	= 	matrix[0][3 + 1] 				//	a
				- 	matrix[0][3] * result[3] 
				- 	matrix[0][2] * result[2] 
				- 	matrix[0][1] * result[1];
	return result;	
};