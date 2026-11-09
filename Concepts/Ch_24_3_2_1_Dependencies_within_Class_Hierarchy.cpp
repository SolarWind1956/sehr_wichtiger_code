/* 
	Ch_24_3_2_1_Dependencies_within_Class_Hierarchy.cpp
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
#include <cstdlib> 
#include <stdlib.h> 
using namespace std;

#include "Ch_24_3_2_1_Dependencies_within_Class_Hierarchy.hpp"

#include "..\type_name_as_str.h"
#include "..\separate_number_with_apostrophe.hpp"
#include "..\integ_to_bin_str.hpp"
/*
	**************************************************************************************
	24.3.2.1. Зависимости внутри иерархии классов
*/ 
/*
	------------------------------------------------------------------------------------------------
	
*/
using namespace SEH;
template <typename T>
void call_overflow(Buffer<T>& b_obj, T par)	{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << "   par == " << par << endl;
	b_obj.overflow(par);
}	

static void		test_1()	
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	int 			int_arg{42};
	double		dbl_arg{3.14};
	
	Circular_buffer<int> 		c_b;
	c_b.put(int_arg);
	Expanding_buffer<double> 	e_b;
	e_b.put(dbl_arg);
	
	call_overflow<int>(c_b, int_arg);
	call_overflow<double>(e_b, dbl_arg);
}
/*
	 
*/
//	---------------> .hpp
static void		test_2()	
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	{
		cout << "1	---------------------------------------------------------------" << endl;
		Y_class		y_class_obj_1;		
	}
	{
		cout << "2	---------------------------------------------------------------" << endl;
		X_class		x_class_obj;		
		
		auto			y_class_obj_2{Y_class(x_class_obj)};
		
		y_class_obj_2.x_class_cpy_ptr->test_func();
	}
	{
		cout << "3	---------------------------------------------------------------" << endl;
		Y_class		y_class_obj_3;		
		cout << "               -----------------------------------------------" << endl;
		//	Будет явно вызван оператор явного преобразования типа из Y_class в X_class
		//	y_class_obj_3.operator X_class().test_func();	
		
		//	Будет неявно вызван оператор явного преобразования типа из Y_class в X_class
		X_class		x_class_obj_3	=	y_class_obj_3;	//	Операция присваивающего копирования
		//	А затем будет неявно вызван конструктор копирования класса X_class
		//	X_class(X_class& lhs)
		cout << "               -----------------------------------------------" << endl;
		x_class_obj_3.test_func();
		cout << "               -----------------------------------------------" << endl;
	}
	cout << "---------------------------------------------------------------" << endl;
}

/*
	-----------------------------------------------------------------------------------------------
*/
static void		test_3()	{	
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	Templ_Class_1	tc_1;
	Templ_Class_2	tc_2;
	
	ConceptsTest<Templ_Class_1> ct_tc_1;
	//ConceptsTest<Templ_Class_2> ct_tc_2;	//	Не проходит требование концепта
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
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << " of Ch_24_3_2_1_Dependencies_within_Class_Hierarchy"  << endl;
	
    try {
		//test_1();
		//test_2();
		test_3();
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
