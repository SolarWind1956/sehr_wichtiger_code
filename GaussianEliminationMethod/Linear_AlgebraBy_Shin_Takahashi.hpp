// Linear_AlgebraBy_Shin_Takahashi.hpp 
#ifndef INCLUDE_GUARDS_Linear_AlgebraBy_Shin_Takahashi_HPP
#define INCLUDE_GUARDS_Linear_AlgebraBy_Shin_Takahashi_HPP
//	--------------------------------------------------------------------------------------------------------
#include <iostream>
using std::cout; using std::cerr; using std::endl; using std::boolalpha;
#include <vector>
#include <stdio.h> /* stderr */
#include <cctype>

#include <Eigen/Dense>
#include <Eigen/LU> 
using namespace Eigen;
/*
	******************************************************************************************************
	Основы работы с Eigen: матрицы, векторы и выражения, тензоры

	----------------------------------------------------------------------------------------------------------------------------------------
*/
void first_example() {
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	/*
		https://ru.wikipedia.org/wiki/Единичная_матрица
		
		'Единичная' или 'тождественная' матрица -  
		квадратная матрица En = (eij) размера (порядка) n, 
		элементы главной диагонали которой равны единице поля (eii=1), 
		а остальные равны нулю (eij=0 при i≠j)
	*/
	Matrix3d 				E_3x3;
   
	E_3x3 	<< 	1, 0, 0,  
				0, 1, 0,  
				0, 0, 1;
	cout << "Единичная марица 3 х 3" << endl;
	cout << E_3x3  << endl;
	
	// 	В библиотеке есть соответствующая функция
	Matrix3d					Identity_3x3		=	MatrixXd::Identity(3, 3);
	cout << "Единичная марица 3 х 3" << endl;
	cout << Identity_3x3  << endl;
    return;
}
/*
	******************************************************************************************************
	Схема Горнера для вычисления многочлена

	P(x) = a𝑛 xⁿ + a𝑛₋₁ xⁿ⁻¹ + ... + a₁ x¹ + a₀ = a₀ + x(a₁ +  x(a₂ + ... x( a𝑛₋₁ + x a𝑛 )))))
*/
double Horner_s_scheme(Vector4d polynom_coeff, double polynom_item){
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	double 	PolynomValue{0};
	auto 	polynom_coeff_size	=	polynom_coeff.size();
	for (auto i{0}; i < polynom_coeff_size - 1; i++){
		PolynomValue = polynom_item * (PolynomValue + polynom_coeff[i]);
	}
	return PolynomValue + polynom_coeff[polynom_coeff_size - 1];
}
double Horner_s_scheme(std::vector<double> polynom_coeff, double polynom_item){
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	double 	PolynomValue{0};
	size_t 	polynom_coeff_size	=	polynom_coeff.size();
	for (size_t i{0}; i < polynom_coeff_size - 1; i++){
		PolynomValue = polynom_item * (PolynomValue + polynom_coeff[i]);
		/*
			р = ad 
			for i = d - 1 down to 0 
				p = x * p  + ai
			end for 
		
		*/
	}
	return PolynomValue + polynom_coeff[polynom_coeff_size - 1];
}
void example_2() {
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	Vector4d		Ai_4;
	Ai_4		<< 2.0, 3.0, 4.0, 2.0;
	cout << Ai_4	 << endl;
	double X{2};
	double PolynomValue{0};
	for (auto i{0}; i < 3; i++){
		PolynomValue = X * (PolynomValue + Ai_4[i]);
	}
	PolynomValue += Ai_4[3];
	cout << "Значение полинома: " << PolynomValue	 << endl;
	
	cout << "Значение полинома: " << Horner_s_scheme(Ai_4, 2)	 << endl;
	
	//std::vector		Vector10d{1.0, -18.0, 144.0, -672.0, 2016.0, -4032.0, 5376.0, -4608.0, 2304.0, -512.0}; 
	std::vector		Vector00d{2.0}; 
	std::vector		Vector01d{2.0, 2.0}; 
	std::vector		Vector02d{2.0, 3.0, 2.0}; 
	std::vector		Vector03d{2.0, 3.0, 4.0, 2.0}; 
	cout << "Значение полинома: " << Horner_s_scheme(Vector00d, 2)	 << endl;
	cout << "Значение полинома: " << Horner_s_scheme(Vector01d, 2)	 << endl;
	cout << "Значение полинома: " << Horner_s_scheme(Vector02d, 2)	 << endl;
	cout << "Значение полинома: " << Horner_s_scheme(Vector03d, 2)	 << endl;
}
/*
*/
void example_3() {
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
}
/*
*/
void example_4() {
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
}	
/*
*/
void example_5() {
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
}
/*
*/
void example_6() {
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
}
/*
	****************************************************************************************************************
	
*/
//	--------------------------------------------------------------------------------------------------------
#endif	//	#ifndef INCLUDE_GUARDS_Linear_AlgebraBy_Shin_Takahashi_HPP
	