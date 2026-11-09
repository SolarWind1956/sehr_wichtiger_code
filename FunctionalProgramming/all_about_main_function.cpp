//	all_about_main_function.cpp
#include <iostream>
using std::cout; using std::cerr; using std::endl; using std::boolalpha;

#include <iomanip>
using std::setw;

#include <concepts>

#include <string_view>
using std::string_view;

#include <cstdint>		// 	Для uintptr_t
#include <cstddef>

#include <typeinfo>

#include <type_traits>
#include <cassert>

#include <vector>
#include <algorithm>
#include <list>
#include <new>   	// 	std::nothrow_t

#include "../../include/value_category_of_expression.hpp"
#include "../../include/type_names_tester.hpp"
#include "../../include\value_category_and_type_tester.hpp"
using namespace	chat_gpt_seh;
 
#include "../../include/color_output.hpp"
using namespace	color_output;

void demo_1_param_of_main()
{
	cout << bright_white << "-------------- I'm " << __PRETTY_FUNCTION__ << reset << endl;
	const		int			argc{4};
	
				char 		program_name[]("program_name.exe");
				char 		arg1[]		=	"first_arg";
				char 		arg2[] 		= 	"second_arg";
				char 		arg3[]		=	"third_arg";
				
	cout << "char arg1[] =	'first_arg';  Тип arg1 == " << type_names_tester<decltype(arg1)>() << endl;
	
	const 		char* 		arg_2 		= 	"second_arg";
	cout << "const char* arg_2 'second_arg';  Тип arg_2 == " << type_names_tester<decltype(arg_2)>() << endl;
	
				char *		argv[argc]	=	{	program_name
											,	arg1
											,	arg2
											,	arg3
											};
	for (auto elem : argv){
		cout << elem << "  ";
	}
	cout << endl;
}
void demo_2_param_of_main()
{
	cout << bright_white << "-------------- I'm " << __PRETTY_FUNCTION__ << reset << endl;
	// 	1. 	Объявляем строковые литералы (константные строки)
    const 	char* 	arg1 	= 	"program_name";
    const 	char* 	arg2 	= 	"first_arg";
    const 	char* 	arg3 	= 	"second_arg";

			char 	arg_1[]	=	"program_name";
			char 	arg_2[] 	= 	"first_arg";
	const	char 	arg_3[]	=	"second_arg";
	
    // 	2. 	Создаем массивы указателей на эти строки
    const char* argv_1[] = {arg1, arg_2, arg3}; 	
	
	const char* argv_2[] = {arg1, arg_2, arg_3}; 	
	
	char* argv_1nc[] = {const_cast<char*>(arg1), arg_2, const_cast<char*>(arg3)}; 	
	
	char* argv_2nc[] = {arg_1, arg_2, const_cast<char*>(arg_3)}; 	
	
	//	3.	Определяем размеры массивов указателей на строки
	const		int			argc_1{sizeof(argv_1) / sizeof(argv_1[0])};
	const		int			argс_2{sizeof(argv_2) / sizeof(argv_2[0])};
	
	for (auto elem : argv_1){
		cout << elem << "  ";
	}
	cout << endl;
	
	for (auto elem : argv_2){
		cout << elem << "  ";
	}
	cout << endl;
}
void demo_3_param_of_main()
{
	cout << bright_white << "-------------- I'm " << __PRETTY_FUNCTION__ << reset << endl;
	const		int			argc{4};
	
	const		char 		program_name[]("program_name.exe");
	const		char 		arg1[]		=	"first_arg";
	const		char 		arg2[] 		= 	"second_arg";
	const		char 		arg3[]		=	"third_arg";
				
	cout << "char arg1[] =	'first_arg';  Тип arg1 == " << type_names_tester<decltype(arg1)>() << endl;
	
	const 		char* 		arg_2 		= 	"second_arg";
	cout << "const char* arg_2 'second_arg';  Тип arg_2 == " << type_names_tester<decltype(arg_2)>() << endl;
	
	const		char *		argv[argc]	=	{	program_name
											,	arg1
											,	arg_2
											,	arg3
											};
	for (auto elem : argv){
		cout << elem << "  ";
	}
	cout << endl;}

/*
	-------------------------------------------
	int main()
	-------------------------------------------
*/
int main() {
	color_output::init();
	cout << bright_white << "-------------- I'm " << __PRETTY_FUNCTION__ << reset << endl;
	
	//demo_1_param_of_main();
	demo_2_param_of_main();
	
	//system("pause");
}
