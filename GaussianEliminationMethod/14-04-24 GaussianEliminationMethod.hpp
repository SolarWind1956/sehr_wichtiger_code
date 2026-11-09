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
#include <vector>
/*
	****************	CalcPolynom3Order() ***********************************************
*/
double CalcPolynom3Order	(	std::vector<double> coefficients
							, 	double 				x) 
{
	// cout << "-------------- I'm " << __func__ << "(std::vector<vector> coefficients, double x)" << endl;
		
	return	coefficients[3] 								//	d
			+ 	x * (coefficients[2] 						//	c
					+	x * (coefficients[1] 				//	b
							+ 	x * coefficients[0]			// 	a
							)
					);
};
/*
	****************	CalcPolynomAnyOrder() ***********************************************
*/
double CalcPolynomAnyOrder	(	int					polynom_order
							,	std::vector<double> coefficients
							, 	double 				x) 
{
	// cout << "-------------- I'm " << __func__ << "(std::vector<vector> coefficients, double x)" << endl;
	double result	=	0.0; 								
	for(auto idx = 0; idx != polynom_order; ++idx)
		result 	=	x *  (coefficients[idx] + result);
	
	return	result	+	coefficients[polynom_order];
};
/*
	****************	GaussianEliminationMethodForAnyOrder() ***********************************************
*/
std::vector<double>	GaussianEliminationMethodForAnyOrder	(	int					polynom_order
															,	std::vector<double>	x_arr
															, 	std::vector<double> y_arr
															)	
{
	cout << "-------------- I'm " << __func__ << endl;
	
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
	
	int order					=	polynom_order;	// Polynom's order
	int coefficient_quantity	=	order + 1;		// Polynom terms' number (respectively, coefficients)
	
	cout << "Cycle by the number of equations in the linear equations system" << endl;
	for (	auto 	idx_equation = 0
		;  			idx_equation != order
		;			++idx_equation
		)
		{
			//	1.
			/*
				Let us first eliminate the first coefficients from all (n) equations,
				then from the last (n - 1) equations the second coefficients,
				then from (n - 2) last equations the third coefficients,
				and so on, 
				but not including the last equation
			*/	
			for (	auto 	idx_outer = idx_equation
				; 			idx_outer != order
				; 			++idx_outer
				) 
				{
					auto	the1stCoeff	=	matrix[idx_equation][idx_equation];
					
					for (	auto 	idx_inner = coefficient_quantity
						; 			idx_inner >= idx_equation
						; 			--idx_inner
						)		
						{	
							matrix[idx_outer][idx_inner]		/=  the1stCoeff;
							matrix[idx_outer + 1][idx_inner]	/= 	matrix[idx_outer + 1][idx_equation];
							matrix[idx_outer + 1][idx_inner]	-=	matrix[idx_equation][idx_inner];
						}
					cout << "idx_outer  " << idx_outer << endl;	
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
/*
	****************	GaussianEliminationMethodFor3Order() ***********************************************
*/
std::vector<double>	GaussianEliminationMethodFor3Order	(	std::vector<double>		x_arr
														, 	std::vector<double> 	y_arr
														)	
{
	cout << "-------------- I'm " << __func__ << endl;
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
	for (	auto 	idx_outer = 	2
		; 			idx_outer != 	3
		; 			++idx_outer
		)
		for (	auto 	idx_inner = 	3 + 1
			; 			idx_inner >= 	2
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
				- 	matrix[2][3] * result[3];
	
	result[1] 	= 	matrix[1][3 + 1] 				//	b
				- 	matrix[1][3] * result[3] 
				- 	matrix[1][2] * result[2];
	
	result[0] 	= 	matrix[0][3 + 1] 				//	a
				- 	matrix[0][3] * result[3] 
				- 	matrix[0][2] * result[2] 
				- 	matrix[0][1] * result[1];
	
	return result;	
};

