//  	GaussianNDegree.cpp
//
//  	Sergii Edgar Hekkel (Heckel)
//  	14.02.2026
//	
//	Solving of Linear Equations Systems with the Gauss Method
//	Simple demo-fragment for a third-degree equations

#include	<iostream>
using std::cout; using std::endl; 
#include <cmath>
#include <vector>
#include <cassert>
#include <limits>

/*
	For brevity (≋100 lines requirement) the implementation does not include
	partial pivoting or zero-pivot handling. 
	In production code,  pivot selection and numerical stability checks would be required.
	
	And for the same reason functions have a narrow contracts 
	and I also used “magic numbers” instead of named constants, 
	but I know that this is not idiomatic approach in C++.
*/

double CalcPolynomNDegree	(	const std::vector<double>& 	coeffs
						, 	double 						X
						) 
{//	Y =  AₙXⁿ + Aₙ₋₁Xⁿ⁻¹ + ... + A₃X³ + A₂X² + A₁X¹ + A₀X⁰  ≡  A₀ + X ∙ (A₁ + X ∙ (A₂ + X ∙ (A₃ + ... X ∙ (Aₙ₋₁ + X ∙ Aₙ))...))
	#if 0	
	return	coeffs[3] 								//	A₀ 
			+ 	X * (coeffs[2] 						//	    + X ∙ (A₁  
					+	X * (coeffs[1] 				//	                     + X ∙ (A₂ 
							+ 	X * coeffs[0]))		// 	                                     + X ∙ A₃))
			;
	#endif			
	
	const size_t 	coeffs_size	= coeffs.size();
	double 			result{};
	for (auto idx = 0; idx != coeffs_size - 1; ++idx)	{
		result = X * (result + coeffs[idx]);
	}	
	return	result + coeffs[coeffs_size - 1];
}					
std::vector<double>	GaussianNDegree(std::vector<std::vector<double>>& matrix)	
{
	int 		degree		=	matrix.size() - 1;	// 	
	int 		rows			=	degree + 1;			//	
	int 		cols			=	degree + 2;			//	matrix ∈ Y
	int 		coeffs_numb	=	degree + 1;			// 	Polynom terms' number (respectively, coefficients)

	// 	Forward 
    for (auto row = 0; row != rows - 1; ++row )		{
		
		for (auto nxt_row = row; nxt_row != rows - 1; ++nxt_row) {
			
			for (auto col = cols - 1; col >= row; --col)	{	
				
				matrix[nxt_row][col]		/=  matrix[row][row];
				matrix[nxt_row + 1][col]	/= 	matrix[nxt_row + 1][row];
				matrix[nxt_row + 1][col]	-=	matrix[row][col];
			}	
		}	
	}
	
	// 	Last elements
	matrix[rows - 1][cols - 1]		/=  matrix[rows - 1][cols - 2];
	matrix[rows - 1][cols - 2]		/=  matrix[rows - 1][cols - 2];
	
	cout << "Algorithm's reverse" << endl;
	
	// 	The polynomial coefficients' vector
	std::vector<double>	coeffs(coeffs_numb);
	
	for (auto row = rows - 1; row >= 0; --row) {
		coeffs[row] =  matrix[row][cols - 1];
		for (auto col = cols - 2; col > row; --col) {
			coeffs[row] -= coeffs[col] * matrix[row][col];
		}
	}
	return coeffs;
}
void print_vector(std::vector<double> vect) {
	for (auto elem : vect)
		cout << elem << "   ";
	cout << endl;
}
/*
	--------------------------------------------------------------------------------------------------------------------------------
	|	main()
	--------------------------------------------------------------------------------------------------------------------------------
*/
int 	main()
{
	cout << "------------------- I'm GaussianEliminationMethod_N_degree.cpp" << endl;

	std::vector<double>		X			{1.0, 2.0, 3.0, 4.0}; 	//	We have set 			{X₁ X₂ X₃ X₄}
	std::vector<double> 		test_coeffs	{2.0, 3.0, 4.0, 5.0};	//	Let us having test set 	{A₃ A₂ A₁ A₀}
	
	std::vector<double> 		Y(4);								//	We have to build set 	{Y₁ Y₂ Y₃ Y₄}
	for (auto idx = 0; idx != 4; ++idx) {
		Y[idx]	=	CalcPolynomNDegree(test_coeffs, X[idx]);
	}	
	cout <<	"Print values of polynomial with test coefficients" << endl;
	print_vector(Y);
	
	// 	Build test matrix for Equations System with  test_coeffs
	std::vector<std::vector<double>>	test_matrix(4, std::vector<double>(5));
	/*
											0	1	2	3	4
											
		0	X₁³ 	X₁² 	X₁¹ 	X₁⁰ 	Y₁		≡		X₁³ 	X₁² 	X₁¹ 	1.	Y₁
		1	X₂³ 	X₂² 	X₂¹ 	X₂⁰ 	Y₂  		≡		X₂³ 	X₂² 	X₂¹ 	1. 	Y₂  
		2	X₃³ 	X₃² 	X₃¹ 	X₃⁰ 	Y₃ 		≡		X₃³ 	X₃² 	X₃¹ 	1. 	Y₃
		3	X₄³ 	X₄² 	X₄¹ 	X₄⁰ 	Y₄		≡		X₄³ 	X₄² 	X₄¹ 	1. 	Y₄
	*/
	for (auto row = 0; row != 4; ++row) {
		for (auto col = 0; col != 3; ++col)	{	
			test_matrix[row][col]	=  	pow(X[row], (3 - col));
		}
		test_matrix[row][3]			=	1.0;
		test_matrix[row][4]			=	Y[row];
	}	
	
	//	We need to calc test_matrix' coefficients to compare these with test set but onley visually (double) 
	auto calced_coeffs		=	GaussianNDegree(test_matrix); 	

	cout <<	"Vector of calculated polynomial coefficients" << endl;
	print_vector(calced_coeffs);
		
	cout <<	"Calc values of polynomial at control points X" << endl;
	for (auto idx = 0; idx != 4; ++idx) {
		Y[idx]	=	CalcPolynomNDegree(calced_coeffs, X[idx]);
	}	
	print_vector(Y);
}