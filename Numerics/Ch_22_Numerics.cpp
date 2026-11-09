/* 
	Ch_22_Numerics.cpp
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

#include "Ch_22_Numerics.hpp"

#include "..\type_name_as_str.h"
#include "..\separate_number_with_apostrophe.hpp"
#include "..\integ_to_bin_str.hpp"
/*
	**************************************************************************************
	22.		Классы для математических вычислений
	
	https://www.chem.kuleuven.be/peeters/manuals/pgi/doc/pgC++_lib/stdlibcr/num_5679.htm
*/ 
/*
	------------------------------------------------------------------------------------------------
	22.2. Числовые пределы
 
	Чтобы уверенно работать с числами, 
	нужно хорошо знать общие свойства встроенных числовых типов, 
	а они зависят больше от реализации, чем от правил языка программирования. 
	
	Например, каково наибольшее значение для типа inf!
	
	Каково наименьшее значение для floaf! 
	
	Преобразование от double к float вызывает усечение или округление? 
	
	Сколько бит в char"!
 
	Ответы на подобного рода вопросы 
	дают специализации шаблона numeric std::limits,
	представленного в заголовочном файле <limits>. 
	
	Например:
*/
struct Quad 	{
	double 	dbl_{double{}};
	int 		int_{int{}};
};
void f_w_numeric_limits (double& 	dbl_, int int_)
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	if (std::numeric_limits<unsigned char>::digits != 8)	{
		cout << "необычные байты (число бит не равно 8)" << endl;
	}
	if (int_ < std::numeric_limits<short>::min() || std::numeric_limits<short>::max() < int_)	 {
		cout << "int_ не может храниться в short без потери точности " << endl;
	}
	if (0 < dbl_ && dbl_ < std::numeric_limits<double>::epsilon()) 
		dbl_ = 0.00;
	if (std::numeric_limits<Quad>::is_specialized)	 {
		cout << "доступна информация о числовых пределах для типа Quad " << endl;
	}	
}
static void		test_1()	
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	double pi{3.14};
	f_w_numeric_limits(pi, 42);
}
/*
	Реальная информация сосредоточена в специализациях шаблона. 
	
	Каждая реализация обязана предоставить по специализации этого шаблона 
	для каждого фунда ментального типа 
	(символьных типов, типа bool, целых чисел и чисел с плавающей запятой), 
	но не для таких типов, как void, перечисления или библиотечные типы (например, complex doublo).
	
	Для интегральных типов, таких как char, 
	интерес представляет небольшая часть информации. 
	
	Вот специализация numeric_limits<char> для реализации, 
	в которой тип char знаковый и содержит 8 бит:						-------------------> .hpp
	
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
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << " of Ch_22_Numerics"  << endl;
	
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
