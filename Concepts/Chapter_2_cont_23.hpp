// 	Chapter_2_cont_23.hpp 
#ifndef INCLUDE_GUARDS_Chapter_2_cont_23_HPP
#define INCLUDE_GUARDS_Chapter_2_cont_23_HPP
//	--------------------------------------------------------------------------------------------------------
#include <iostream>
using std::cout; using std::cerr; using std::endl; using std::boolalpha;
#include <iterator>   		// 	std::istream_iterator
#include <sstream>    		// 	std::istringstream

#include <vector>
#include <deque>
#include <map>        
#include <unordered_map> 
#include <unordered_set>   	// 	std::unordered_set
#include <list>
#include <stdio.h> 			//	std::err 	
#include <cstdint>			//	std::uint32_t, std::uint64_t
#include <cctype>
#include <memory>			// 	std::make_shared, std::make_unique, 	std::unique_ptr
#include <random>   			// 	std::default_random_engine, 	std::uniform_int_distribution
#include <utility>   
#include <tuple> 
#include <limits>             // 	std::numeric_limits
#include <initializer_list>   // 	std::initializer_list	
#include <cassert>
#include <thread>
#include <functional>   		// 	std::function
#include <cctype>   			// 	std::toupper

#include "..\include\type_name_as_str.h"
#include "..\include\type_name_as_str_chat_gpt.hpp"
#include "..\include\value_category_of_expression.hpp"

/*
	****************************************************************************************************************************
	2.10. 	 Перенаправляющие ссылки (T&&)		-	 Forwarding References (T&&)

	`Перенаправляющая ссылка` (`Forwarding Reference`) (T&&), 
	отличающаяся от `ссылки на r-value` (&&), основанной только на контексте, 
	– это особый, `специализированный вид ссылки`, который 
	
	(1) 		устанавливает `универсальную связь` 
			с результатом `выражения любой категории значения` (`value category`) 
	и 
	
	(2) 		сохраняет особенности этой `категории значения`, 
			так что `связанный объект` может быть `перемещен`, 
			если такая операция применима.
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	2.10.1.	Описание												-	Description
	
	Иногда необходима `одна и та же` 'ссылка' для связывания либо с `l-value`, либо с `r-value`, 
	которая `в дальнейшем` должна `отличаться от собственно самой ссылки` 
	вне зависимости от `допустимости перемещения` результата `исходного выражения`. 
	
	`Перенаправляющая ссылка` (forwarding reference), например `forRef` в приведенном ниже примере, 
	используемая в `интерфейсе шаблона функции`, например `myFunc`, см. ниже, 
	предоставляет именно эту возможность 
	и оказывается чрезвычайно полезной 
	для целей `условного перемещения` или `копирования` объекта `из тела шаблона функции`:
*/
template <typename T>
void myFunc(T&& 	forRef)
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	// 	Здесь есть возможность проверить, `разрешено ли перемещение` `forRef` из тела `myFunc()`
	// 	или 	`не из тела` myFunc().
}
/*
	В приведенном примере в определении шаблона функции myFunc() 
	параметр `forRef` синтаксически выглядит 
	как `неконстантная ссылка` на `r-value` типа `T`. 
	
	Но именно в этом конкретном контексте 
	тот же синтаксис `T&&` обозначает `перенаправляющую ссылку` 
	с `эффектом сохранения исходной категории значения` объекта, 
	связанного с `forRef`. 
	
	Синтаксис `T&&` представляет `перенаправляющую ссылку` 
	в противоположность `ссылке на r-value`, 
	когда отдельный `независимый шаблон функции` 
	имеет `параметр типа`, например `T`, 
	а `неквалифицированный` параметр функции 
	типа, который точно указан как `T&&`, например `const T&&`, 
	должен быть `ссылкой на r-value`, 
	а `не перенаправляющей ссылкой`.
	
	Допустим, рассмотрим шаблон функции f_forwards_to_overloads(), который принимает один аргумент по ссылке, 
	а затем пытается использовать его для вызова одной из двух перегружаемых версий функции overloaded_g() 
	в зависимости от того, являлся ли исходный аргумент l-value или r-value:
*/
//	Некоторый тип, который может извлечь преимущество из возможности перемещения.
struct 	S_can_be_moved {  };

// 	Целевая функция; перегрузка для значений `const S_can_be_moved` l-value.
void overloaded_g(const S_can_be_moved& forRef){
	cout << "----const S&---------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	cout_test_result(value_category<decltype(forRef)>());
}

// 	Целевая функция; перегрузка только для значений `S_can_be_moved` r-value.
void overloaded_g(S_can_be_moved&& forRef){
	cout << "----S&&---------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	cout_test_result(value_category<decltype(forRef)>());
}  

// 	Перенаправление в целевую перегруженную функцию overloaded_f() на основе `категории значения`.

template <typename T>
void f_forwards_to_overloads(T&& forRef);
/*
	Следует отметить, 
	что функция может быть перегружена 
	`исключительно для единственного ссылочного типа`, 
	но на практике чаще всего встречается перегрузка для `константных ссылок` на l-value и r-value. 
	
	В рассматриваемом здесь конкретном случае, где `f_forwards_to_overloads` – шаблон функции, 
	`T` – `шаблон типа параметра`, а `сам тип параметра` точно указан как `T&&`, 
	параметр функции `forRef` в приведенном выше фрагменте кода 
	обозначает `перенаправляющую ссылку` (`forwarding reference`). 
	
	Если функция `f_forwards_to_overloads()` вызывается с l-value, 
	то `forRef` является `ссылкой на l-value`, 
	иначе `forRef` – `ссылка на r-value`.
	
	С учетом такой двойственной сущности `forRef` 
	одним из достаточно подробных способов 
	определения `категории исходного значения` переданного аргумента 
	могло бы стать использование типажа `std::is_lvalue_reference` 
	для самой ссылки `forRef`:
*/
#include <type_traits>   		// 	std::is_lvalue_reference
#include <utility>       		// 	std::move
template <typename T>
void f_forwards_to_overloads_1(T&& forRef)   		// 	forRef – перенаправляющая ссылка.
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	if (std::is_lvalue_reference<T>::value)   	//	Использование типажа версии C++11.
    {
		cout << "-------------- is_lvalue_reference<T> " << endl;
		cout_test_result(value_category<decltype((forRef))>());
        overloaded_g(forRef);              		// 	Передает `forRef` как l-value.
												// 	Вызывает overloaded_g(const S_can_be_moved&).
    }                           
	else
    {
		cout << "-------------- is_rvalue_reference<T> " << endl;
		cout_test_result(value_category<decltype((forRef))>());
		overloaded_g(std::move(forRef));    		// 	Передает `forRef` как r-value.
												// 	Вызывает overloaded_g(S_can_be_moved&&).
	}                           
}
/*
	Показанный выше предикат std::is_lvalue_reference<T>::value спрашивает, 
	происходит ли объект, связанный с `forRef`, от l-value-выражения, 
	и позволяет разработчику создать две ветви, зависящие от ответа. 
	
	В общем случае более предпочтительным является решение, 
	реализующее эту логику во время компиляции
*/ 
#include <utility>   			// 	std::forward
template <typename T>
void f_forwards_to_overloads_2(T&& forRef)
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	
	cout << "  Тип  == " << type_name_as_str_chat_gpt<T>() << endl;
	cout << "  Тип  == " << type_name_as_str_chat_gpt<decltype(forRef)>() << endl;
	cout << "  Тип  == " << type_name_as_str_chat_gpt<decltype((forRef))>() << endl;
	//	#include "..\include\value_category_of_expression.hpp"
	cout_test_result(value_category<decltype((forRef))>());
	cout_test_result(value_category<decltype(forRef)>());
	
    overloaded_g(std::forward<T>(forRef));	// 	То же самое, что overloaded_g(std::move(forRef)), 
											//	если и только если `forRef` является
											// 	ссылкой на r-value, иначе – 
											//	равнозначно overloaded_g(forRef).
}
/*
	Функция client_of_overloaded_func(), вызывающая f_forwards_to_overloads(), 
	в полной мере воспользуется тем же поведением любой из двух альтернативных реализаций, 
	предложенных выше:
*/ 
void client_of_overloaded_func()
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
    S_can_be_moved 	s;
	
    f_forwards_to_overloads_2(s);     // 	Создает экземпляр f<S_can_be_moved&> – 
									//	forRef – ссылка на l-value (S_can_be_moved&).
									// 	Функция f_forwards_to_overloads_2<S_can_be_moved&> 
									//	в итоге вызывает overloaded_g(S_can_be_moved&).

    f_forwards_to_overloads_2(S_can_be_moved());   // 	Создает экземпляр f<S_can_be_moved> – 
												//	forRef – ссылка на r-value (S_can_be_moved&&).
												// 	Функция f_forwards_to_overloads_2<S_can_be_moved> 
												//	в итоге вызывает overloaded_g(S_can_be_moved&&).
	cout << "--------------------------------------------------------------------------------" << endl;											
	#if 0
	f_forwards_to_overloads_1(s);
	f_forwards_to_overloads_1(S_can_be_moved());
	#endif	
}
/*
	----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	2.10.1.1. 	Краткий обзор методов `логического вывода аргументов шаблона` функции
					template argument deduction (TAD)
	
	Вызов `шаблона функции` 
	без `явно предоставленных шаблонных аргументов` в `точке вызова` 
	заставит компилятор попытаться, если возможно, 
	логически `вывести эти аргументы типов шаблона` из `аргументов функции`:
*/
template <typename T> void f_for_template_argument_deduction()
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	
	cout << "  Тип  == " << type_name_as_str_chat_gpt<T>() << endl;
	
	cout_test_result(value_category<T>());
}
template <typename T> void g_for_template_argument_deduction(T x)
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	
	cout << "  Тип  == " << type_name_as_str_chat_gpt<T>() << endl;
	cout << "  Тип  == " << type_name_as_str_chat_gpt<decltype(x)>() << endl;
	cout << "  Тип  == " << type_name_as_str_chat_gpt<decltype((x))>() << endl;
	
	cout_test_result(value_category<decltype(x)>());
}
template <typename T> void h_for_template_argument_deduction(T y, T z)
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	
	cout << "  Тип `y` == " << type_name_as_str_chat_gpt<T>() << endl;
	cout << "  Тип `y` == " << type_name_as_str_chat_gpt<decltype(y)>() << endl;
	cout << "  Тип `y` == " << type_name_as_str_chat_gpt<decltype((y))>() << endl;
	
	cout_test_result(value_category<decltype(y)>());
}
void template_argument_deduction_example_0()
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
    //f_for_template_argument_deduction();        	// 	Ошибка: невозможно вывести аргумент шаблона `T`.   
    f_for_template_argument_deduction<short>();   	// 	OK: T задан явно.
    g_for_template_argument_deduction(0);         // 	OK: `T` выводится как тип `int` из литерала `0`; `x` – тип `int`.
    //h_for_template_argument_deduction(0, 'a');    // 	Ошибка: выведены конфликтующие типы для `T` (int против `char`).
    h_for_template_argument_deduction('A', 'B');  // 	OK: оба аргумента имеют одинаковый тип.
}
/*
	Любые `cv-квалификаторы` – `const`, `volatile` или `оба` – 
	в выводимом параметре функции 
	будут применены `после завершения процедуры вывода типа`:
*/
template <typename T> void c_g_for_template_argument_deduction(const T x)
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	
	cout << "  Тип `T` == " << type_name_as_str_chat_gpt<T>() << endl;
	cout << "  Тип `const T x' == " << type_name_as_str_chat_gpt<decltype(x)>() << endl;
	cout << "  Тип `const T x` == " << type_name_as_str_chat_gpt<decltype((x))>() << endl;
	
	cout_test_result(value_category<T>());
	cout_test_result(value_category<decltype(x)>());
	cout_test_result(value_category<decltype((x))>());
}
#if 0	
template <typename T> void v_g_for_template_argument_deduction(volatile  T x)
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	
	cout << "  Тип  == " << type_name_as_str_chat_gpt<T>() << endl;
	cout << "  Тип  == " << type_name_as_str_chat_gpt<decltype(x)>() << endl;
	cout << "  Тип  == " << type_name_as_str_chat_gpt<decltype((x))>() << endl;
	
	cout_test_result(value_category<decltype(x)>());
}
template <typename T> void cv_g_for_template_argument_deduction(const volatile  T x)
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	
	cout << "  Тип  == " << type_name_as_str_chat_gpt<T>() << endl;
	cout << "  Тип  == " << type_name_as_str_chat_gpt<decltype(x)>() << endl;
	cout << "  Тип  == " << type_name_as_str_chat_gpt<decltype((x))>() << endl;
	
	cout_test_result(value_category<decltype(x)>());
}
#endif	
void template_argument_deduction_example_1()
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
    
	c_g_for_template_argument_deduction(0);		// 	OK: `T` выводится как `int`; `x` – `const int`.
	const	int		c_i_var{11};
	c_g_for_template_argument_deduction(c_i_var);	// 	OK: `T` выводится как `int`; `x` – `const int`.
	int				i_var{21};
	c_g_for_template_argument_deduction(i_var);	// 	OK: `T` выводится как `int`; `x` – `const int`.
	
	#if 0	
	v_g_for_template_argument_deduction(0);		// 	OK: `T` выводится как `int`; `y` – `volatile int`.
	cv_g_for_template_argument_deduction(0);		// 	OK: `T` выводится как `int`; `z` – `const volatile int`.
	#endif	
}
/*
	Точно так же `ref-квалификаторы`, отличающиеся от `&&`, 
	т.е. `&` или `&& вместе с любыми cv-квалификаторами`, 
	не изменяют `процесс логического вывода` 
	и также применяются после его завершения:
*/

/*
	Логический вывод типа работает по-разному для `перенаправляющих ссылок`, 
	где `единственным квалификатором параметра шаблона` является `&&`. 
	
	Для наглядности рассмотрим `объявление шаблона функции` f_accepting_forwarding_reference(), 
	принимающего `перенаправляющую ссылку` (forwarding reference) `forRef`:
*/
template <typename T> 
void 	f_accepting_forwarding_reference(T&& forRef)
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	cout << "  Тип `T` == " << type_name_as_str_chat_gpt<T>() << endl;
	cout << "  Тип `T&& forRef` == " << type_name_as_str_chat_gpt<decltype(forRef)>() << endl;
		
	cout_test_result(value_category<decltype(forRef)>());
}
/*
	В примере в начале раздела 2.10.1 мы видели, 
	что при вызове f() с l-value типа `S_can_be_moved` тип `T` выводится как S_can_be_moved&, 
	и `forRef` становится ссылкой на `l-value`. 
	
	Но если f вызывается с `x-value` типа `S_can_be_moved`, то `T` выводится как `S_can_be_moved` 
	и `forRef` становится `ссылкой на r-value`. 
	
	Внутренний процесс, который приводит к такой двойственности, 
	основан на `свертывании ссылок` (reference collapsing – см. следующий подраздел) 
	и на `специализированных правилах логического вывода типа`, 
	введенных для этого конкретного случая. 
	
	Если тип `T` `перенаправляющей ссылки` должен выводиться из некоторого выражения `E`, 
	то сам `T` будет выводиться как `ссылка на `l-value`, если `E` является `l-value`, 
	иначе применяются обычные правила вывода типов, 
	а `T` будет выведен как `нессылочный тип`:
*/
void g_accepting_forwarding_reference_func_test()
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	int i;
    f_accepting_forwarding_reference(i); 
		// 	`i` – l-value-выражение. 
		//	Следовательно, `T` выводится как int& – `особое правило`.
		// 	`T&&` становится `int& &&`, затем `свертывается` до `int&`.
		
    f_accepting_forwarding_reference(0); 
		// 	`0` – r-value-выражение. 
		//	Следовательно, `T` выводится как `int`.
		// 	`T&&` становится `int&&`, которая является `ссылкой на r-value`.
}
/*
	-------------------------------------------------------------------------------------------------------------------------------------------------------------------
	2.10.1.2. 	Свертывание ссылок		-	 Reference collapsing
 
	В предыдущем подразделе мы видели: 
	если функция с параметром - `перенаправляющей ссылкой` -
	`forRef` -
	вызывается с соответствующим `l-value-аргументом` 
	(например, с `именованной переменной`), 
	то возникает `интересное явление`: 
	после вывода типа мы `временно` получаем нечто, 
	синтаксически выглядящее как `r-value-ссылка` на `l-value-ссылку`. 
	
	Поскольку `ссылки на ссылки` "запрещены в C++", 
	компилятор использует механизм `свертывания ссылок` (`reference collapsing`) 
	для разрешения `перенаправляющей ссылки` в параметре `forRef` 
	в `одиночную ссылку`, 
	предоставляя таким образом способ вывода, 
	из самого типа `T`, 
	исходной категории значения аргумента, 
	переданного в f_accepting_forwarding_reference().
	
	Процесс `свертывания ссылок` выполняется компилятором в любой ситуации, 
	в которой существует вероятность создания `ссылки на ссылку`. 
	
	В таблице ниже показаны простые правила `свертывания` `нестабильных ссылок` в `стабильные`. 
	
	Особенно следует отметить, что `ссылка на l-value` 
	всегда имеет приоритет над `ссылкой на r-value`. 
	
	Единственный случай, в котором `две ссылки` свертываются `в одну ссылку на r-value`, – 
	обе исходные `ссылки на r-value`.
	
	Таблица. Свертывание нестабильных пар ссылок в одну стабильную
	-----------------------------------------------------------------------------------------------------------------------------------------------
	Первый ссылочный тип 		Второй ссылочный тип 		Результат свертывания ссылок
	-----------------------------------------------------------------------------------------------------------------------------------------------
		&							&							&
		& 							&&							&
		&&							&							&
		&&							&&							&&
 	-----------------------------------------------------------------------------------------------------------------------------------------------
	
	В `C++` явная запись типа `ссылка на ссылку` невозможна:
*/	
int		int_var   			= 0;    			// 	OK.
int&   	ref_to_int_var  		= i;    			// 	OK.    
//int& & 	ref_to_ref_to_int_var	= ref_to_int_var;	// 	Ошибка: `ref_to_ref_to_int_var` объявлена как `ссылка на ссылку`.   
	//	error: cannot declare reference to 'int&', which is not a typedef or a template type argument
/*
	Но сделать это легко с помощью `псевдонима типа` и `параметров шаблона`, 
	и в этом случае начинает действовать `механизм свертывания ссылок`:
*/
void		type_aliases_template_parameters_reference_collapsing()
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	
	using 	T1 	= 	int&; 	// 	OK. `int&`
	cout << "using 	T1 	= 	int&;  Тип T1 == " << type_name_as_str_chat_gpt<T1>() << endl;
	
	using 	T2 	= 	T1&; 	// 	OK, `int& &` becomes `int&`.
	cout << "using 	T2 	= 	T1&;  Тип T2 == " << type_name_as_str_chat_gpt<T2>() << endl;
	
	static_assert(std::is_same<T2, int&>::value,"");
}
/*
	Кроме того, `ссылки на ссылки` могут возникать во время вычислений, 
	включающих `метафункции`, или 
	как часть правил языка, таких как `правила логического вывода типа`:
*/
// 	Метафункция, которая преобразуется в `l-value-ссылку` на `T`.
template <typename T>
struct AddLvalueRef 
{ 
	typedef 	T& 	type; 
};
template <typename T>
void f_metafunction_test_1(T input)
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	
	cout << "  Тип `T` == " << type_name_as_str_chat_gpt<T>() << endl;
	cout << "  Тип `T input` == " << type_name_as_str_chat_gpt<decltype(input)>() << endl;
	cout << "  Тип `typename AddLvalueRef<T>::type` == " 
			<< type_name_as_str_chat_gpt<typename AddLvalueRef<T>::type>() << endl;
	/*
		Обратите внимание: 
		мы используем ключевое слово `typename` 
		во время создания экземпляра шаблона
		как `обобщенный способ указания` на то, 
		что `зависимое имя` `AddLvalueRef<T>::type` 
		является `типом`, а `не значением`.
	*/
	typename AddLvalueRef<T>::type 	ir1 = input;    	// 	OK: добавляет `&` для создания `T&`.
	cout << " `typename AddLvalueRef<T>::type ir1 = input;`  Тип ir1 == " 
			<< type_name_as_str_chat_gpt<decltype(ir1)>() << endl;
			
	typename AddLvalueRef<T&>::type 	ir2 = input;    	// 	OK: свертывается в `T&`.
	cout << " `typename AddLvalueRef<T&>::type ir2 = input;`  Тип ir2 == " 
			<< type_name_as_str_chat_gpt<decltype(ir2)>() << endl;
	
	typename AddLvalueRef<T&&>::type 	ir3 = input;    	// 	OK: свертывается в `T&`.   
	cout << " `typename AddLvalueRef<T&&>::type ir3 = input;`  Тип ir3 == " 
			<< type_name_as_str_chat_gpt<decltype(ir3)>() << endl;
	/*
		В версии `C++20` ключевое слово `typename`
		больше не требуется в `некоторых контекстах`, 
		где `зависимое квалифицируемое имя` 
		обязательно `должно быть типом`. 
		
		Например, `зависимое имя`, используемое как `тип возвращаемого значения функции`, – 
		template <typename T> T::R f(); – `не требует` ключевого слова `typename`.
	*/		
}
/*
	---------------------------------------------------------------------------------------------------------------------------------------------------------------
	2.10.1.3. 	Идентификация `перенаправляющих ссылок`
	
	`Синтаксис перенаправляющей ссылки` (&&) 
	полностью совпадает 
	с `синтаксисом ссылок на r-value`, 
	поэтому единственным способом их различения 
	является `исследование окружающего контекста`. 
	
	При использовании в ситуации, 
	в которой возможно выполнение `логического вывода типа`, 
	синтаксис `T&&` не соответствует ссылке на r-value, 
	вместо этого он представляет `перенаправляющую ссылку`. 
	
	Чтобы начал действовать вывод типа, 
	шаблон функции обязательно должен иметь параметр типа (например,`T`) 
	и параметр функции - 
	типа, точно соответствующего `имени параметра типа`, 
	за которым следуют символы `&&` (например, `T&&`):
*/
struct S0_For_type_deduction
{
	template <typename T>
	void f(T&& forRef);
		// 	Полностью пригодно для вывода типа аргумента шаблона:
		// 	`forRef` – `перенаправляющая ссылка`.
};
/*
	Обратите внимание: если `параметр функции` квалифицируется, 
	то синтаксис изменяет смысл и становится `обычной ссылкой на r-value`:
*/
struct S1_For_type_deduction
{
	template <typename T>
	void f(const T&& crRef);
		// 	Пригодно для вывода типа, но `не` для `перенаправляющей ссылки`:
		// 	из-за квалификатора `const` 
		//	`crRef` становится `ссылкой на r-value`.
};
/*
	Если `функция-член` `шаблона класса` 
	сама по себе не является также шаблоном, 
	то ее параметр шаблона типа выводиться не будет:
*/
template <typename T>
struct S2_For_type_deduction
{
	void f(T&& rRef);	// 	Непригодно для вывода типа, поскольку тип `T` фиксирован 
						//	и известен как часть создаваемого экземпляра S2_For_type_deduction: 
						//	`rRef` – `ссылка на r-value`.
};
/*
	В более общем смысле следует отметить, 
	что `синтаксис &&` никогда `не может` применять `перенаправляющую ссылку` 
	`для функции`, которая сама по себе `не является шаблоном`.
*/
/*
	------------------------------------------------------------------------------------------------------------------------------------------------------------------
	2.10.1.4. 	auto&& – `перенаправляющая ссылка` в контексте `без параметров`
	
	Помимо `параметров шаблона функции`, 
	`перенаправляющие ссылки` также могут существовать 
	в контексте `определений переменных`, 
	где используется ключевое слово `auto`, 
	поскольку они тоже являются участниками процедуры `логического вывода типа`:
*/
void f_with_auto_forw_ref_to()
{
	auto&& 	forw_ref_to_i = 42;   
			// 	`forw_ref_to_i` – `перенаправляющая ссылка`, 
			//	так как тип `forw_ref_to_i` обязательно должен быть выведен 
			//	из `выражения инициализации` `42`.
}
/*
	Точно так же, как и `параметры функции`, 
	`auto&&` разрешается либо в `ссылку на l-value`, 
	либо в `ссылку на r-value` 
	в зависимости от `категории значения` `выражения инициализации`:
*/
void g_with_auto_forw_ref_to()
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	int 		i 	= 0;
	auto&& 	lv 	= i;   			// 	`lv`– тип `int&`.
	cout << "auto&& 	lv 	= i;  Тип lv == " << type_name_as_str_chat_gpt<decltype(lv)>() << endl;
	cout_test_result(value_category<decltype(lv)>());
	
	auto&& 	rv 	= 0;    		// 	`rv` – тип `int&&`.
	cout << "auto&& 	rv 	= 0;  Тип rv == " << type_name_as_str_chat_gpt<decltype(rv)>() << endl;
	cout_test_result(value_category<decltype(rv)>());
}
/*
	Аналогично `const auto&` синтаксис `auto&&` 
	связывается `с любым объектом`. 
	
	Но в случае `auto&&` ссылка будет `константной`, 
	только если она `инициализируется константным объектом`:
*/
void h_with_auto_forw_ref_to()
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	int	       	i 	= 0;
	const int 	ci 	= 0;
 
	auto&& 		lv  = i;    		// 	`lv` – тип `int&`.
	cout << "auto&& 	lv 	= i;  Тип lv == " << type_name_as_str_chat_gpt<decltype(lv)>() << endl;
	cout_test_result(value_category<decltype(lv)>());

	auto&& 		clv 	= ci;   	// 	`clv` – тип `const int&`.
	cout << "auto&& 	clv 	= ci;  Тип clv == " << type_name_as_str_chat_gpt<decltype(clv)>() << endl;
	cout_test_result(value_category<decltype(clv)>());
}
/*
	Точно так же, как и для параметров функции, 
	`исходная категория значения` выражения, 
	используемого для `инициализации переменной` `перенаправляющей ссылки`, 
	может распространяться во время `последующего вызова функции`, 
	например при использовании `std::forward`
*/ 
#include <utility>   			// 	std::forward
// 	Создание `l-value` или `r-value` в зависимости от типа `T`.
template <typename T>
T 	get_for_forward()
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	
	cout << "  Тип `T` == " << type_name_as_str_chat_gpt<T>() << endl;
	cout_test_result(value_category<T>());
	
	return T{};
}
// 	Здесь use_for_forward() также принимает 	`как параметр` `перенаправляющую ссылку`,
// 	которую можно использовать по своему усмотрению.
template <typename T>
void 	use_for_forward(T&& t)
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	
	cout << "  Тип `T` == " << type_name_as_str_chat_gpt<T>() << endl;
	cout << "  Тип `T&& t` == " << type_name_as_str_chat_gpt<decltype(t)>() << endl;
	
	cout_test_result(value_category<T>());
	cout_test_result(value_category<decltype(t)>());
}

template <typename T>
void l_for_forward()
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	
	auto&& 	fr = get_for_forward<T>();		// 	get_for_forward<T>() может быть либо `l-value`, 
											//	либо `r-value` в зависимости от типа `T`.
											
	cout << "auto&& 	fr = get_for_forward<T>();  Тип `fr` == " << type_name_as_str_chat_gpt<decltype(fr)>() << endl;
	
	cout_test_result(value_category<decltype(fr)>());
	
    use_for_forward(std::forward<decltype(fr)>(fr));   	// 	decltype() – функциональная возможность версии C++11.
			// 	Распространяется исходная категория значения get_for_forward<T>() при использовании.
}
/*
	Обратите внимание: поскольку 
	
	(1) 	std::forward<>() требует тип объекта, 
		который будет перенаправляться 
		как предоставленный пользователем аргумент шаблона, 
	
	и 
	
	(2) 	невозможно именовать тип переменной `fr`, 
		в приведенном выше примере был использован оператор 	`decltype` 
		для извлечения типа `fr`.
*/ 
/*
	--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	2.10.1.5. 	Перенаправляющие ссылки без перенаправления	-	 Forwarding references without forwarding
 
	Иногда может оказаться полезным `преднамеренно не перенаправлять` 
	переменную `auto&&` или `параметр перенаправляющей ссылки` 
	для функции. 
	
	В таких случаях `перенаправляющие ссылки` 
	используются исключительно для их семантики `const-сохранения` и `универсального связывания`. 
	
	В качестве примера рассмотрим задачу `получения итераторов` 
	в диапазоне `неизвестной категории значения`:
*/
#include <iterator>   	// 	std::begin, std::end
template <typename T>
std::vector<T> 	getRange()
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	
	cout << "  Тип `T` == " << type_name_as_str_chat_gpt<T>() << endl;
	cout_test_result(value_category<T>());
	
	return std::vector<T>(5, T{10});
}
/*
	В `C++` `шаблон` можно использовать в качестве `параметра шаблона`, 
	что позволяет передавать `один шаблон` в `качестве аргумента` `другому`. 
	
	Такой подход называется «шаблоном шаблона» 
	и позволяет создавать `обобщенные классы`, 
	которые могут работать с различными контейнерами 
	(например, std::vector, std::set) 
	или другими шаблонными структурами. 
*/
#include <iterator> 	// 	For iterator concepts like std::input_or_output_iterator
template <class T, template <class> class CONTAINER>
	requires requires (CONTAINER<T> container) {	
		// 	Requirement that t.begin() returns an input_or_output_iterator
		requires std::input_or_output_iterator<decltype(container.begin())>; 
		// 	Requirement that t.end() returns an input_or_output_iterator
		requires std::input_or_output_iterator<decltype(container.end())>;   
	}
//void 	traverseRange(const decltype(CONTAINER<T>::begin())& begin, const decltype(CONTAINER<T>::end())& end){
void 	traverseRange(const CONTAINER<T>& container){
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	for (auto it = container.begin(); it != container.end();	it++)
		cout << *it << " ";
	cout << endl;
}
template <typename T>
void m_Forwarding_references_without_forwarding_1()
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	// 	getRange<T>() может быть либо `l-value`, либо `r-value` в зависимости от типа `T`.
	auto&& 	range = getRange<T>(); 	
	cout << "  Тип range == " << type_name_as_str_chat_gpt<decltype(range)>() << endl;
		
	auto 	begin 	= std::begin(range);
	cout << "  Тип begin == " << type_name_as_str_chat_gpt<decltype(begin)>() << endl;
	
	auto 	end 		= std::end(range);
	cout << "  Тип end == " << type_name_as_str_chat_gpt<decltype(end)>() << endl;
    
	for (auto it = begin; it != end;	it++)
		cout << *it << " ";
	cout << endl;
	
	//traverseRange<T>(range.begin(), range.end());	//	<decltype(range)>
	//traverseRange<decltype(range)>(range.begin(), range.end());	
	traverseRange(range);			//	<decltype(range)>
}
/*
	--------------------------------------------------------------------------------------------------------------------------------------------------------
*/
int example_70() {
	cout << "------- I'm " <<  __PRETTY_FUNCTION__ << " - ID потока == " << std::this_thread::get_id() << endl;
	
	//client_of_overloaded_func();
	//template_argument_deduction_example_0();
	//template_argument_deduction_example_1();
	//g_accepting_forwarding_reference_func_test();
	//type_aliases_template_parameters_reference_collapsing();
	#if 0	
	int 		int_var{42};
	int&		ref_to_int_var = int_var;
	f_metafunction_test_1(ref_to_int_var);
	//	Точно такой же результат:
	f_metafunction_test_1(223);
	#endif
	
	//g_with_auto_forw_ref_to();
	//h_with_auto_forw_ref_to();
	
	//l_for_forward<double>();
	
	m_Forwarding_references_without_forwarding_1<int>();
	
	return 0;
}
//	--------------------------------------------------------------------------------------------------------
#endif	//	#ifndef INCLUDE_GUARDS_Chapter_2_cont_23_HPP
