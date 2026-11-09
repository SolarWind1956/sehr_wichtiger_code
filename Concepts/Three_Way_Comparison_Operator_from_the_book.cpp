/*
	Three_Way_Comparison_Operator_from_the_book.cpp
*/
#include	<iostream>
using std::cout; using std::endl; using std::boolalpha;
#include <limits>
using std::string;
using std::numeric_limits;
#include <cassert>
#include <cstring>
#include <concepts>
#include "..\type_name_as_str.h"
#include "Three_Way_Comparison_Operator_from_the_book.hpp"
/*	
	***************************************************************************************************
	--------------------------------------------------------------------------------------------
	 Rainer Grimm 'C++20 Get the Details'
	--------------------------------------------------------------------------------------------
	 4.3 Оператор трехстороннего сравнения		-	 4.3 Three-Way Comparison Operator
	 
	 Оператор трехстороннего сравнения за его внешний вид <=> часто называют космическим кораблем (spaceship operator).

	 Этот оператор для двух значений 'A' и 'B' определяет, 
	 какое из следующих соотношений – (A < B), (A == B) или (A > B) – 
	имеет место. 
	
	Вы можете сами определить этот оператор, либо компилятор сгенерирует его за вас.
	
	Для того чтобы оценить все преимущества, даваемые оператором трехстороннего сравнения, 
	давайте начнем с классического способа реализации.
	
*/
/*
	********************		main()		****************************************
*/
int 	main()
{
	cout << "I'm *******	Three_Way_Comparison_Operator_from_the_book.cpp" << endl;
	//		
	test_1();
	test_2();
	test_3();
	test_4();
	test_5();
	test_6();
	test_7();
	test_8();
	test_9();
	test_10();

	return 0;
}
/*

*/
