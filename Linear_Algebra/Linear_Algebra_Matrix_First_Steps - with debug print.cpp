/* 
	Linear_Algebra_Matrix_First_Steps.cpp
*/
#include <iostream>
using std::cout; using std::endl; using std::boolalpha; using std::cin; using std::cerr;

#include <iterator>
#include <vector>

#include <ranges>			//	Для gcc этот заголовок обязателен??? , clang вроде как обошелся без него

#include <numeric>
#include <limits>
#include <climits>
#include <cmath> 
#include <math.h>
using namespace std;

#include "Linear_Algebra_Matrix_First_Steps.hpp"

//#include "..\type_name_as_str.h"
//#include "..\separate_number_with_apostrophe.hpp"
//#include "..\integ_to_bin_str.hpp"
//using namespace SEH;
/*
	**************************************************************************************
	Определение 1.1.1. 	'Матрицей' размера m x n называется упорядоченная прямоугольная таблица 
						(или массив) чисел, содержащая 'm' строк и  'n' столбцов. 
 
	Числа, входящие в описание матрицы, называемые ее 'элементами' (или 'компонентами'), 
	характеризуются как своим значением, так и 'номерами строк и столбцов'. 
	
	Условимся обозначать элемент матрицы, 
	расположенный в i-й строке и  j-м столбце, 
	как a[j][i]. 
  
	---------------------------------------------------------------------------------------------
	Определение 1.1.2. Числа 'm', 'n' 	и 'm x n' называются 'размерами матрицы'. 	
	
	Матрицы принято классифицировать по количеству их строк и столбцов. 
	
	Определение 1.1.3. 	Если m == n, то матрица называется квадратной, порядка 'n'.
	
						Матрица размера m x 1 называется m-мерным (или m-компонентным) столбцом. 
						
						Матрица размера 1 x n называется n-мерной (или n-компонентной) строкой.
*/ 
/*

*/
double 	matrix_determinant	(	double*  	Matrix
							,	const int 	MatrixRank
							)
							{
	//cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	double *		add_minor_ptr	=	new double[MatrixRank * MatrixRank];
	double 	matr_determ{0.0};
	if (MatrixRank == 2){
		matr_determ	=	(*Matrix) * (*(Matrix + 3)) - (*(Matrix + 1)) * (*(Matrix + 2));
		//cout << "Выход из рекурсии: matr_determ == " << matr_determ << endl;
		return matr_determ;
	}	
	// cout << " matrix_determinant - нет ли оплошноти с MatrixRank " << MatrixRank << endl;
	for (int j = 1; j <= MatrixRank; ++j) {
		matr_determ	+=		(*(Matrix + j - 1)) 
						* 	pow((double)(-1.0), (double)(j + 1)) 
						* 	matrix_determinant	(	additional_minor	(	Matrix
																	,	MatrixRank
																	,	j
																	,	add_minor_ptr
																	)
												,	MatrixRank - 1
												);				
		//cout << "j == " << j << 	"  matr_determ  " << matr_determ << endl;
		//cout << " matrix_determinant - нет ли оплошноти с matr_determ  " << matr_determ  << endl;
	}
	//cout << "matr_determ  " <<  matr_determ <<  endl;
	delete[] add_minor_ptr;		add_minor_ptr = nullptr;	
	
	return matr_determ;
}

static void		test_1()	{
	cout << "-------------- I'm " <<	__PRETTY_FUNCTION__	<< endl;
	
	double *		add_minor_ptr	=	new double[16];
	
	double	Matrix2x2[2][2]	{ 	{3.14, 2.71} 
							,	{9.81, 6.01}  
							};
	print_matrix(&Matrix2x2[0][0], 2);
	
	additional_minor		(	&Matrix2x2[0][0]
						,	2
						,	2
						,	add_minor_ptr
						);
	print_matrix(add_minor_ptr, 1);					
	
	double	Matrix3x3[3][3]	{ 	{3.14, 2.71, 5.82} 
							,	{9.81, 6.01, 7.14}  
							,	{4.11, 8.07, 1.18}  
							 };
	print_matrix(&Matrix3x3[0][0], 3);
	
	additional_minor		(	&Matrix3x3[0][0]
						,	3
						,	3
						,	add_minor_ptr
						);
	print_matrix(add_minor_ptr, 2);					

	double	Matrix4x4[4][4]	{ 	{3.14, 2.71, 5.82, 3.14} 
							,	{9.81, 6.01, 7.14, 2.77}  
							,	{4.11, 8.07, 1.18, 5.76}  
							,	{2.88, 3.29, 5.21, 6.22}  
							};
	print_matrix(&Matrix3x3[0][0], 4);
	
	additional_minor		(	&Matrix4x4[0][0]
						,	4
						,	4
						,	add_minor_ptr
						);
	print_matrix(add_minor_ptr, 3);					
	
	delete[] add_minor_ptr;		add_minor_ptr = nullptr;
}  
static void		test_2()	
{
	cout << "-------------- I'm " <<	__PRETTY_FUNCTION__	<< endl;
	double matr_determ{};
	#if 0
	//	---------------------------------------------------------------------------------------
	double	Matrix2x2[2][2]	{ 	{3.14, 2.71} 
							,	{9.81, 6.01}  
							};
	print_matrix(&Matrix2x2[0][0], 2);
	
	matr_determ 	=	matrix_determinant	(	&Matrix2x2[0][0]
										,	2
										);
	cout << "matr_determ 2 x 2 == " <<  matr_determ << endl;											
	
	//	---------------------------------------------------------------------------------------
	double	Matrix3x3[3][3]	{ 	{3.14, 2.71, 5.82} 
							,	{9.81, 6.01, 7.14}  
							,	{4.11, 8.07, 1.18}  
							};
	print_matrix(&Matrix3x3[0][0], 3);					
	
	matr_determ 	=	matrix_determinant	(	&Matrix3x3[0][0]
										,	3
										);
	cout << "matr_determ 3 x 3 == " <<  matr_determ << endl;		
	#endif
	//	---------------------------------------------------------------------------------------
	double	Matrix4x4[4][4]	{ 	{3.14, 2.71, 5.82, 3.14} 
							,	{9.81, 6.01, 7.14, 2.77}  
							,	{4.11, 8.07, 1.18, 5.76}  
							,	{2.88, 3.29, 5.21, 6.22}  
							};
	print_matrix(&Matrix4x4[0][0], 4);
	
	matr_determ 	=	matrix_determinant	(	&Matrix4x4[0][0]
										,	4
										);
	cout << "matr_determ 4 x 4 == " <<  matr_determ << endl;		
	
	matr_determ 	=	matrix_4_determinant	(	&Matrix4x4[0][0]
						,	4
						); 
	cout << "matr_determ 4 x 4 == " <<  matr_determ << endl;		
}
static void		test_3()	{	
	cout << "-------------- I'm " <<	__PRETTY_FUNCTION__	<< endl;
}
static void		test_4()	{	
	cout << "-------------- I'm " <<	__PRETTY_FUNCTION__	<< endl;
}
static void		test_5()	{	
	cout << "-------------- I'm " <<	__PRETTY_FUNCTION__	<< endl;
}
static void		test_6()	{	
	cout << "\n-------------- I'm " <<	__PRETTY_FUNCTION__	<< endl;

}	
/*
	**************************************************************************************************
		
*/
static void		test_7()	{	
	cout << "\n-------------- I'm " <<	__PRETTY_FUNCTION__	<< endl;
}	
/*
	***************************		main()		******************************************
*/
int main()
{
	cout << "-------------- I'm " <<	__PRETTY_FUNCTION__	<< " of Linear_Algebra_Matrix_First_Steps"	<< endl;
	
	try {
		//test_1();
		test_2();
		//test_3();
		//test_4();
		//test_5();
		//test_6();
		//test_7();	
		system("pause");
		}
		catch (const std::exception& e) {
				cerr << "Exception: " << e.what() << endl;
				return	EXIT_FAILURE;
		}
	system("pause");
}
/*

*/