#pragma once
/*
	Concepts_shipped_with_CPP_standard_library.hpp
*/
#include	<iostream>
using std::cout; using std::endl; using std::boolalpha;
#include <limits>
using std::numeric_limits;
#include <string>
using std::string;
#include <cassert>
#include <cstring>
#include <concepts>
#include <iterator>
#include <optional>
#include <vector>
#include <ranges>
#include <typeinfo> 

#include "..\type_name_as_str.h"
/*	
	***********************************************************************************************
	Concepts shipped with the C++ standard library
	https://www.sandordargo.com/blog/2021/03/03/cpp-concepts-in-standard-library
	Концепции, поставляемые со стандартной библиотекой C++.
	Шандор Дарго 3 марта 2021 г.
	--------------------------------------------------------------------------------------------
	
	C++20 не только дал нам возможность писать мощные концепции, 
	но также включает более 50 концепций, 
	которые являются частью стандартной библиотеки 
	и используются 'в трех разных заголовках'.

	--------------------------------------------------------------------------------------------------------
	Понятия в <concepts> заголовкe
	
	В <concepts>-заголовке вы найдете наиболее общие, 
	выражающие основные концепции языка, 
	концепции сравнения 
	и 
	концепции объектов.

	Мы не собираемся здесь рассматривать их все по понятным причинам, 
	полный список вы можете найти здесь . 
	https://en.cppreference.com/w/cpp/concepts
	
	Позвольте мне выбрать три концепции, чтобы мы могли понять идею.

	-	std::convertible_to		-	для конверсий с меньшим количеством сюрпризов
	
	-	std::convertible_to		-	помогает выразить, что вы принимаете только типы, 
								которые можно преобразовать в другой тип - 
								тип, который вы указываете. 
								
								Преобразование может быть как явным, так и неявным. 
								
								Например, вы можете сказать, что принимаете только типы, 
								которые можно преобразовать в bool. 
								
								В качестве первого параметра вы передаете тип, 
								преобразование которого должно быть действительным From, 
								а вторым — тип, который вы хотите преобразовать To, 
								в нашем случае — bool.
*/
#include <concepts>
template <typename T>
void fun(T bar) requires std::convertible_to<T, bool> {
  cout << boolalpha << static_cast<bool>(bar) << '\n'; 
}
void test_8() {
	cout << "-------------- I'm void " << __func__ << "()" << endl;
	
	fun(5); 		// OK an int can be converted into a pointer
	//  fun(std::string("Not OK")); 
	// void fun(T) requires  convertible_to<T, bool> [with T = std::__cxx11::basic_string<char>]' with unsatisfied constraints
}
/*
	-	std::totally_ordered	-	для определенных сравнений

								std::totally_ordered помогает принять типы, 
								которые определяют все 6 (шесть) операторов сравнения 
								( ==, !=, <, >, <=, >=), 
								и что результаты соответствуют строгому общему порядку на 'T'.
*/
struct NonComparable {
	int a;
};
struct Comparable {
	auto operator<=>(const Comparable& rhs) const = default; 
	int a;
};
template <typename T>
void 	fun_with_totally_ordered(T t) 
				requires std::totally_ordered<T> 
{
	cout << typeid(t).name() << " can be ordered\n";
	cout << t.a << endl;
}
void test_9() {
	cout << "-------------- I'm void " << __func__ << "()" << endl;
	
	NonComparable nc{666};
	// НЕ КАТИТ!!!!!!! fun_with_totally_ordered(nc); 
	// Not OK: error: use of function 'void fun(T) requires totally_ordered<T> [with T = NonComparable]' with unsatisfied constraints
	Comparable comparableObj{42};	//	Поле 'a' будет проинициализировано констанотой 42
	
	fun_with_totally_ordered(comparableObj);
}
/*
	В приведенном выше примере вы также можете увидеть, 
	как легко использовать <=> оператор (он же spaceship - космический корабль) 
	для генерации "всех операторов сравнения".

	Если вы ищете дополнительную информацию об <=>операторе, 
	я настоятельно рекомендую прочитать эту статью из журнала Modernes C++ .
	https://www.modernescpp.com/index.php/c-20-more-details-to-the-spaceship-operator/
	
	------------------------------------------------------------------------------------------------------
	std::copyable	-	для копируемых типов
	
	std::copyable	помогает гарантировать, что принимаются только те типы, 
	экземпляры которых можно скопировать. 
	
	std::copyable-объект должен быть копируемым, назначаемым и перемещаемым.
*/
#include <typeinfo> 
class NonMovable {
public:
	NonMovable() 	= default;
	~NonMovable() 	= default;

	NonMovable(const NonMovable&) 					= default;
	NonMovable& 	operator=(const NonMovable&) 	= default;
  
	NonMovable(NonMovable&&) 				= delete;
	NonMovable& 	operator=(NonMovable&&) = delete;
};
class NonCopyable {
public:
	NonCopyable() 	= default;
	~NonCopyable() 	= default;

	NonCopyable(const NonCopyable&) 				= delete;
	NonCopyable& 	operator=(const NonCopyable&) 	= default;
  
	NonCopyable(NonCopyable&&) 					= default;
	NonCopyable& 	operator=(NonCopyable&&) 	= default;
};

class Copyable {
public:
	Copyable() 		= default;
	~Copyable() 	= default;

	Copyable(const Copyable&) 				= default;
	Copyable& 	operator=(const Copyable&) 	= default;

	Copyable(Copyable&&) 				= default;
	Copyable& 	operator=(Copyable&&) 	= default;
};
template <typename T>
void fun_for_Copyable(T t) 
	requires std::copyable<T> 
{
  cout << typeid(t).name() << " is copyable\n";
}
void test_10() {
	cout << "-------------- I'm void " << __func__ << "()" << endl;

	NonMovable 	nm;
	//	fun_for_Copyable(nm); 
	// 	error: use of function 'void fun(T) requires  copyable<T> [with T = NonMovable]' with unsatisfied constraints
	NonCopyable nc;
	//	fun_for_Copyable(nc); 
	// 	error: use of function 'void fun(T) requires  copyable<T> [with T = NonCopyable]' with unsatisfied constraints
	Copyable 	copyableObj;
	fun_for_Copyable(copyableObj);
}
/*
	Как вы можете видеть, в приведенном выше примере, 
	класс NonMovable 
	не соответствует концепции, 
	поскольку его назначение перемещения и конструктор перемещения удалены.

	Для NonCopiable, это аналогичный случай, но, 
	хотя семантика перемещения доступна, 
	в нем отсутствуют назначение копирования и конструктор копирования.

	Наконец, Copyableкласс по умолчанию использует 
	все 5 специальных функций-членов и 
	удовлетворяет концепции std::copyable.

	------------------------------------------------------------------------------------------------------------
	Понятия в <iterator>-заголовке
	
	В <iterator>-заголовке вы в основном найдете понятия, 
	которые пригодятся при работе с алгоритмами . 
	
	Если задуматься, это имеет смысл, 
	поскольку функции заголовка <algorithms> 
	работают с контейнерами через итераторы, 
	а не непосредственно с контейнерами.

	------------------------------------------------------------------------------------------------------------
	std::indirect_unary_predicate<F, I>

	Существуют концепции, 
	связанные с вызываемыми объектами, 
	например, вы можете указать, 
	что принимаете только унарные предикаты. 
	
	Во-первых, что такое предикат? 
	
	Предикат 			— 	это вызываемый объект, 
							который возвращает либо bool значение, 
							либо значение, которое можно преобразовать в bool. 
	
	Унарный предикат 	— 	это предикат, 
							который принимает в качестве входных данных 
							один параметр.
*/
#include <iterator>
#include <vector>
template <typename F, typename I>
void foo_with_unary_predicate(F fun, I iterator) 
	requires std::indirect_unary_predicate<F, I> 
{
    cout << boolalpha << fun(*iterator) << '\n';
}
void test_11() {
	cout << "-------------- I'm void " << __func__ << "()" << endl;
	
	auto 	biggerThan42 = 	[](int i){return i > 42;};
  
	std::vector numbers{15, 43, 66};
	
	for(auto it = numbers.begin(); it != numbers.end(); ++it) {
		foo_with_unary_predicate(biggerThan42, it);
	};
}
/*
	В приведенном выше примере в foo_with_unary_predicate() принимается функция и итератор, 
	и концепция std::indirect_unary_predicate гарантирует, 
	что переданная функция fun() может принять 
	значение, указанное итератором, 
	и вернуть bool.

	--------------------------------------------------------------------------------------------
	std::indirectly_comparable

	В <iterator>-заголовке вы найдете не только концепции, 
	связанные с вызываемыми объектами, 
	но и более общие. 
	
	Например, являются ли два типа косвенно сопоставимыми. 
	
	Звучит интересно, давайте возьмем простой пример:
*/
template <typename Il, typename Ir, typename F>
void 	foo	(	Il 	leftIterator
			, 	Ir 	rightIterator
			, 	F 	function
			) 
			requires std::indirectly_comparable<Il, Ir, F> 
{
    cout << boolalpha << function(*leftIterator, *rightIterator) << '\n';
}
void test_12() {
	cout << "-------------- I'm void " << __func__ << "()" << endl;
  
	using namespace std::string_literals;
   
	auto binaryLambda 	= [](int i, int j)			{ return 42; };
	auto binaryLambda2 	= [](int i, std::string j)	{ return 666;};
  
	std::vector ints{15, 42, 66};
	std::vector floats{15.1, 42.3, 66.6};
	foo(ints.begin(), floats.begin(), binaryLambda);

	//	foo(ints.begin(), floats.begin(), binaryLambda2); 
	// 	error: use of function 'void foo(Il, Ir, F) requires  indirectly_comparable<Il, Ir, F, std::identity, std::identity> 
}
/*
	В этом случае документация меня немного озадачила :

	В качестве третьего параметра шаблона обычно class 'R', который относится к диапазонам.
	
	Но тогда, согласно его определению, 
	он вызывает std::indirect_binary_predicate с R, 
	переадресованный в первой позиции.
	
	В std::indirect_binary_predicate в первой позиции вы принимаете class F, 
	а F обозначает вызываемый объект (часто функцию).
	
	Почему R не вызывает F? 
	
	Почему в текстовом описании не упоминаются бинарные предикаты?

	Вероятно, только потому, что это еще начало пути концепций. 
	
	На самом деле я собираюсь отправить запрос на изменение этого элемента.

	------------------------------------------------------------------------------------------------------
	Концепты в <ranges>-заголовке
	
	В <ranges>-заголовке вы найдете концепции, описывающие требования к разным типам диапазонов.

	Или просто параметр представляет собой ДИАПАЗОН - range. 
	
	Но вы можете утверждать концепт для любых диапазонов, например 
	
	input_range, 
	
	output_range, 
	
	forward_range 
	
	и т. д.
*/
#include <ranges>
#include <typeinfo> 

template <typename R>
void foo_range(R range) 	
	requires std::ranges::borrowed_range<R> {
	cout << typeid(range).name() << " is a borrowed range\n";
}

void test_112() {
	cout << "-------------- I'm void " << __func__ << "()" << endl;
	
	std::vector 		numbers{15, 43, 66};
	std::string_view 	stringView{"is this borrowed?"};
	//   foo_range(numbers); 
	// 	error: use of function 'void foo(R) requires borrowed_range<R> 
	//	[with R = std::vector<int, std::allocator<int> >]' with unsatisfied constraints
	foo_range(stringView);
}
/*
	В приведенном выше примере проверяется, 
	удовлетворяет ли тип концепции borrowed_range. 
	
	Мы можем наблюдать, что std::string_view делает, 
	а std::vector - нет.

	Если вам интересно, наличие заимствованного диапазона означает, 
	что функция может принять его по значению 
	и вернуть полученный из него итератор 
	без какой-либо опасности зависания. 
	
	Чтобы узнать подробности, нажмите здесь
	https://en.cppreference.com/w/cpp/ranges/borrowed_range	

	----------------------------------------------------------------------------------------------------
	Заключение

	Сегодня мы увидели несколько примеров концепций, 
	поставляемых со стандартной библиотекой C++20. 
	
	Их около 50, которые распределены по трем заголовкам ( <concepts>, <iterators>, <ranges>).
*/
/*

*/	
//	-----------------------------------------------------------------------------------------------------------