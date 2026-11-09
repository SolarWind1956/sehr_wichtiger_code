/* 
	Errors_Handling_Collection.cpp
*/
#include <iostream>
using std::cout; using std::endl; using std::boolalpha; using std::cin; using std::cerr;
#include <fstream>
#include <sstream>

#include <new>			//	Как выяснилось, не обязательное явное включение заголовка
#include <cstddef>
#include <memory>
#include <iterator>
#include <map>
#include <list>
#include <exception>
#include	<bitset>
#include <vector>

#include <ranges>			//	Для gcc этот заголовок обязателен??? , clang вроде как обошелся без него

#include <numeric>
#include <limits>
#include <climits>
#include <cmath> 
#include <math.h>
#include <cerrno>

using namespace std;

#include "Errors_Handling_Collection.hpp"
#if 0	//	Многострочный комментарий для кода - меняем 0 на 1, и фрагмент будет компилироваться
#include "..\type_name_as_str.h"
#include "..\separate_number_with_apostrophe.hpp"
#include "..\integ_to_bin_str.hpp"
#endif	//	Конец фрагмента закомментированного кода
/*
	**************************************************************************************
	
*/ 
/*
	------------------------------------------------------------------------------------------------
	
*/
static void		test_1()	
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
}
/*
	----------------------------------------------------------------------------------------------------
	22.3. Стандартные математические функции
	
	Заголовочные файлы <cmath> и <math.h> объявляют то, 
	что принято называть 
	«общепринятыми математическими функциями»:

	Когда возникают ошибки, 
	errno из <cerrпо> принимает значение EDOM 
	b случае выхода аргумента из области определения функций, 
	и значение ERANGE — в случае выхода результата за возможные пределы значений типа. 
	
	Например:
*/
void 	f_w_errno ()
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	
	errno = 0; 				//	очистка старого состояния, инициирующего ошибку
	sqrt(-1.0);
	if (errno	==	EDOM) 
		cerr << "sqrt() not defined for negative argument" << endl;
 
	errno = 0; 				//	очистка старого состояния, инициирующего ошибку
	pow (std::numeric_limits<double>::max(), 2.0);
	if (errno 	== 	ERANGE) 
		cerr << "result of pow () too large to represent as a double" << endl;
}
static void		test_2()	
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	float	test_flt		=	1.0f + std::numeric_limits<float>::epsilon() - 1.0f;
	if (  test_flt > 0.0f)
		cout << "epsilon() = 1.19209290E-07F самое маленькое число такое, 	что 1 + epsilon - 1 	больше нуля." << endl;
	else
		cout << "epsilon() = ??? 1.19209290E-07F самое маленькое число такое, 	что 1 + epsilon - 1 	больше нуля." << endl;
	
	f_w_errno ();
}
/*

*/
/*
	-----------------------------------------------------------------------------------------------
*/
static void		test_3()	{	
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	
}
/*
	-------------------------------------------------------------------------------------------
*/
static void		test_4()	{	
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	
}
/*
	****************************************************************************************
*/
static void		test_5()	{	
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
}

static void		test_6()	{	
	cout << "\n-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;

}	
/*
	***************************		main()		******************************************
*/
int main()
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << " of Errors_Handling_Collection"  << endl;
	
    try {
		//test_1();
		test_2();
		//test_3();
		//test_4();
		//test_5();
		//test_6();
				
		system("pause");
    }
    catch (const std::exception& e) {
        cerr << "Exception: " << e.what() << endl;
        return  EXIT_FAILURE;
    }
	system("pause");
}
