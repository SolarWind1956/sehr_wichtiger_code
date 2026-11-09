// 	cut_Chapter_25_cont_09.hpp 
#ifndef INCLUDE_GUARDS_Chapter_25_cont_09_HPP
#define INCLUDE_GUARDS_Chapter_25_cont_09_HPP

#pragma GCC diagnostic ignored "-Wunused-variable"				//	Отключить диагностику
#pragma GCC diagnostic ignored "-Wunused-parameter"				//	warning: unused parameter 'c' [-Wunused-parameter]
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#pragma GCC diagnostic ignored "-Wpessimizing-move"
#pragma GCC diagnostic ignored "-Wunused-value"
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#pragma GCC diagnostic ignored "-Wuninitialized"

//	--------------------------------------------------------------------------------------------------------
#include <iostream>
using std::cout; using std::cerr; using std::endl; using std::boolalpha;
#include <iomanip>
#include <iterator>   		// 	std::istream_iterator
#include <sstream>    		// 	std::istringstream
#include <ostream>
#include <type_traits>
#include <vector>
#include <map>
#include <stdio.h> 			//	std::err 	
#include <cstdint>			//	std::uint32_t, std::uint64_t
#include <cctype>
#include <memory>			// 	std::make_shared, std::make_unique, 	std::unique_ptr
#include <utility>   			// 	std::forward
#include <limits>             // 	std::numeric_limits
#include <initializer_list>   // 	std::initializer_list	
#include <algorithm>          // 	std::min
#include <numeric>			// 	std::accumulate
#include <cassert>
#include <thread>
#include <functional>   		// 	std::function
#include <cctype>   			// 	std::toupper
#include <random>   			// 	std::default_random_engine, 	std::uniform_int_distribution
#include <concepts>

#include "..\include\type_names_tester.hpp"
#include "..\include\value_category_tester.hpp"
#include "..\include\triviality_tester.hpp"
#include "..\include\layout_and_special_members_tester.hpp"
#include "..\include\color_output.hpp"
#include "..\include\sizeof_and_alignof.hpp"
#include "..\include\is_nothrow_tester.hpp"
#include "..\include\value_category_and_type_tester.hpp"
#include "..\include\S_AS_UDT.hpp"
#include "..\include\StringVST.hpp"
#include "..\include\PersonVST.hpp"

using namespace	chat_gpt_seh;
using namespace	color_output;
/*
	****************************************************************************************************************************
	2.21. 		Шаблоны с переменным количеством аргументов
	2.21.1. 		Описание
	2.21.1.1. 	Шаблоны классов с переменным количеством аргументов
	2.21.1.2. 	`Пакеты параметров `типов шаблона`
	2.21.1.3. 	Специализация шаблонов классов с переменным количеством аргументов
	2.21.1.4. 	`Шаблоны-псевдонимы` с переменным количеством аргументов
	2.21.1.5. 	`Шаблоны функций` с `переменным количеством аргументов`
	2.21.1.6. 	`Пакеты параметров функций` (не путать с `пакетами параметров `типов шаблона`)
	2.21.1.7. 	Функции-члены с переменным количеством аргументов
	2.21.1.8. 	Вывод аргументов шаблона		-	Template argument deduction
	2.21.1.9. 	Правило `жадного поиска` совпадения
	2.21.1.10. 	Правило `приемлемого совпадения`	-	The Rule of Fair Matching
	2.21.1.11. 	Особые случаи соответствия аргументов шаблона функции
	2.21.1.12. 	Бестиповые `пакеты параметров шаблонов`		-	Non-type template parameter packs

	-------------------------------------------------------------------------------------------------------------------------------------------------------------------
	2.21.1.13. 	Пакеты параметров шаблонов шаблона	-	Template template parameter packs	-	TTPP

	
	`Пакеты параметров шаблонов шаблона` 	– 	это `обобщение `параметров шаблонов шаблона` 
												с переменным количеством аргументов версии C++03. 
												
	Variadic `классы` и `функции` можно спроектировать так, 
	чтобы они принимали `пакеты параметров шаблонов шаблона` 
	как `параметры` в дополнение к `пакетам параметров типов` 
	и `бестиповым пакетам параметров` шаблона.
	
	`Template template parameter` - `TTP`
	`Шаблонный шаблонный параметр`
	`Параметр-шаблон шаблона` 	
	`Шаблонный параметр шаблона` 
								– 	это `параметр шаблона`, 
									именующий `аргумент`, 
									который сам по себе является `шаблоном`. 
										
*/
/*
	Можно спроектировать шаблон класса, типом `типового параметра` у которого
	является `шаблон класса`.
*/
template <typename T, template<typename> class X>   
	requires requires(X<T>& x, T t) {	x.push_back(t); }		// Проверяем, что метод push_back() существует
struct TPClsT_T 
{ 
	X<T> container;
	
	TPClsT_T()
	{
		cout << bright_white << "-------------- I'm " << __PRETTY_FUNCTION__ << reset << endl;
		// 	Предполагается, что класс `X` имеет метод push_back()
		container.push_back(T{});
	}
};
void test_1_templ_templ_param(){
	cout << bright_white << "-------------- I'm " << __PRETTY_FUNCTION__ << reset << endl;
	TPClsT_T<int, std::vector>		ttp_cls_obj;
	ttp_cls_obj.container.push_back(1);
	
	for (const auto& val : ttp_cls_obj.container) {
		// 	Предполагается, что `T` можно вывести на экран
		cout << val << " ";
	}
	cout << endl;
}
/*
	-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
int example_123() {
	cout << blue	<< "-------------- I'm " << __PRETTY_FUNCTION__ << " of Chapter_25_cont_09.hpp" << reset << endl;	

	test_1_templ_templ_param();

	return 0;
}
//	--------------------------------------------------------------------------------------------------------
#pragma GCC diagnostic pop 					// 	Восстановить прежние настройки
#endif	//	#ifndef INCLUDE_GUARDS_Chapter_25_cont_09_HPP

