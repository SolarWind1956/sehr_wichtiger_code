//		#pragma once -> SF.8: Use #include guards for all header files
#ifndef INCLUDE_GUARDS_LINEAR_ALGEBRA_MATRIX_FIRST_STEPS_HPP
#define INCLUDE_GUARDS_LINEAR_ALGEBRA_MATRIX_FIRST_STEPS_HPP
/*
	Matrix_First_Steps.hpp
*/	
#include	<iostream>
using std::cout; using std::endl; using std::boolalpha; using std::cin; using std::cerr;
	
#include	<vector>
#include	<cassert>
#include <memory>
#include <iterator>
#include <algorithm>
#include <functional>

#include <limits>
#include <climits>

#include <random>
#include <string_view>
#include <sstream>
//#include "..\type_name_as_str.h"
//#include "..\shapes.hpp"

//namespace SEH {
	
//	-----------------------------------------------------------------------------------------------------
/*
	------------------------------------------------------------------------------------------------
	Дополнительный минор элемента матрицы n-го порядка (Additional minor of a matrix element) 
	есть определитель порядка n − 1, 
	соответствующий матрице, которая получается из матрицы 
	путём вычёркивания i-й строки и j-го столбца. 
	
	Например, для матрицы:

	M	=	1  8 7 
			3  0 5
			−1 9 11
			
	дополнительный минор второго порядка для элемента второй строки третьего столбца
	
	M(2, 3)		=	 1  8
					-1  9
					
	получается путём вычёркивания второй строки и третьего столбца:
*/
void		print_matrix	(	double*  	Matrix
					,	const int 	MatrixRank
					)
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	
	for (int i = 0; i < MatrixRank; ++i) {
		for (int j = 0; j < MatrixRank; ++j) 
			cout << *(Matrix + MatrixRank * i + j)	<< "  ";
		cout << endl;
	}		
}
double*		additional_minor		(	double*  	Matrix
								,	const int 	MatrixRank
								,	const int 	col
								,	double *		add_minor_ptr
								)
								{
	//cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	int displ{0};
	for (int i = 1; i < MatrixRank; ++i) 
	{
		for (int j = 0; j < MatrixRank; ++j) {
			if (j == col - 1)
				continue;
			double 		matrix_elem 	=	*(Matrix + MatrixRank * i + j);
			*(add_minor_ptr + displ)	=	matrix_elem;
			#if 0
			cout 	<< " i == " << i << " j == " << j << "  matrix_elem = " << matrix_elem;
			cout  	<< " displ == " << displ  << endl;
			cout  	<< " *(add_minor_ptr + displ) == " << *(add_minor_ptr + displ) << endl;
			#endif
			++displ;	 
		}	 
		//cout << endl;
	}	
	print_matrix	(add_minor_ptr,	MatrixRank - 1);
	return	add_minor_ptr;
}	
static	double 	matrix_2_determinant	(double*  	Matrix)
							{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__   << endl;
	print_matrix	(Matrix,	2);
	double	part_determ		=	(*Matrix) * (*(Matrix + 3)) - (*(Matrix + 1)) * (*(Matrix + 2));
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << " Частичный детерминант матрицы 2 го ранга " << part_determ << endl;
	return part_determ;
}

static	double 	matrix_3_determinant	(double*  	Matrix)
							{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__   << endl;
	print_matrix	(Matrix,	3);
	double *		add_minor_ptr	=	new double[9];
	double	part_determ{};
	
	for ( int j = 1; j <= 3; ++j) {
		part_determ	+=	(*(Matrix + j - 1)) 
						* 	pow((double)(-1.0), (double)(j + 1)) 	
						*	matrix_2_determinant	(	additional_minor	(	Matrix
																	,	3
																	,	j
																	,	add_minor_ptr
																	)
												);					
	}	
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  <<  " Частичный детерминант минора 2 го ранга " << part_determ << endl;
	delete[] add_minor_ptr;		add_minor_ptr = nullptr;	
	
	return part_determ;
}

static	double 	matrix_4_determinant	(	double*  	Matrix
									,	const int 	MatrixRank
									)
									{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__   << endl;
	print_matrix	(Matrix,	4);
	double *		add_minor_ptr	=	new double[16];		
	double	part_determ{};
	//	Цикл разложения по элементам строки
	for ( int j = 1; j <= 4; ++j) {
		part_determ	+=		(*(Matrix + j - 1)) 
						* 	pow((double)(-1.0), (double)(j + 1)) 	
						*	matrix_3_determinant	(	additional_minor	(	Matrix
																	,	4
																	,	j
																	,	add_minor_ptr
																	)
												);					
	}	
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  <<  " Частичный детерминант минора 3 го ранга " << part_determ << endl;
	delete[] add_minor_ptr;		add_minor_ptr = nullptr;	
	
	return part_determ;
}	


//	-----------------------------------------------------------------------------------------------------
//}	//	End of namespace SEH
/*
	****************************************************************************************************
*/
#endif // INCLUDE_GUARDS_LINEAR_ALGEBRA_MATRIX_FIRST_STEPS_HPP