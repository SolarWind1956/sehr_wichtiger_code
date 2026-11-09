#pragma once
/*
	The_concepts_from_the_book.hpp
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
#include <list>
#include <forward_list>
#include <ranges>
#include <typeinfo> 
#include <type_traits> 

// Для clang и gcc математические библиотеки в разных заголовках описаны
#if defined(__clang__)
	#include <math.h>
#else	
	#include <cmath>
#endif

#include "..\type_name_as_str.h"
/*	
	***************************************************************************************************
	Rainer Grimm 'C++20 Get the Details'
	--------------------------------------------------------------------------------------------
	
	--------------------------------------------------------------------------------------------
	Концепты добавляют семантические требования к параметрам шаблона. 

	У std::sort есть перегруженная версия, которая поддерживает оператор сравнения.
*/	
 template<class RandomIt, class Compare>
 constexpr void sort(RandomIt first, RandomIt last, Compare comp);
/*
	Ниже приводятся требования к самому продвинутому варианту std::sort.
 
	-	RandomIt 
		должен удовлетворять 
		ValueSwappable и LegacyRandomAccessIterator.
 
	-	Тип, получаемый при разыменовании RandomIt, 
		должен удовлетворять 
		MoveAssignable и MoveConstructible.
 
	-	Тип, получаемый при разыменовании RandomIt, 
		должен удовлетворять 
		Compare.
 
	Требования, такие как ValueSwappable или LagacyRandomAccessIterator, 
	являются 'именованными требованиями'. 
	
	Некоторые из этих требований формализованы в концептах С++20.
	https://en.cppreference.com/w/cpp/language/constraints
	
	В частности, std::sort() требует LegacyRandomAccessIterator. 
	
	Давайте внимательно посмотрим 
	на именованное требование 
	LegacyRandomAccessIterator, 
	называемое 
	random_access_iterator (часть <iterator>) в С++20:
	std::random_access_iterator
*/	
					
template<class I>
concept random_access_iterator =
				std::bidirectional_iterator<I> 
			&&	std::derived_from</*ITER_CONCEPT(I)  ???*/std::iterator_traits<I>, std::random_access_iterator_tag> 
			&&	std::totally_ordered<I> 
			&&	std::sized_sentinel_for<I, I> 
			&&	requires(I i, const I j, const std::iter_difference_t<I> n) {
					{ i += n }	-> std::same_as<I&>;
					{ j + n }	-> std::same_as<I>;
					{ n + j }	-> std::same_as<I>;
					{ i-= n }	-> std::same_as<I&>;
					{ j- n }	-> std::same_as<I>;
					{ j[n] }	-> std::same_as<std::iter_reference_t<I>>;
				};
/* 
	 Тип 'I' поддерживает концепт std::random_access_iterator, 
	 если он поддерживает концепт std::bidirectional_iterator 
	 и все из перечисленных требований. 
	 
	 Например, требование 
	 
	 { i += n }	->	same_as<I&> 		//	выражение { I +=n } валидно и возвращает значение типа I&. 
	 
	как часть выражения requires значит, 
	что для значения типа 'I' 
	
	выражение { I +=n } валидно и возвращает значение типа I&. 
	
	Чтобы завершить историю с сортировкой, 
	std::list поддерживает 
	std::bidirectional_iterator, 
	но не 
	std::random_access_iterator, 
	как требует std::sort().
	
	Теперь, когда вы используете алгоритм, 
	который требует std::random_access_iterator, 
	но поддерживается только std::bidirectional_iterator, 
	вы получите четкое и понятное сообщение, 
	говорящее о том, 
	что итератор не поддерживает концепт 
	std::random_access_iterator.
*/	
/*
	 4.1.2 Преимущества концептов
	 
	-	Требования к параметрам шаблона являются частью интерфейса.
	
	-	Перегрузка функций и специализация шаблонов классов также может быть основана на концептах.
	
	-	Концепты могут быть использованы для: 
			-	шаблонных функций, 
			-	шаблонных классов 
			и 
			-	универсальных (generic) функций классов или шаблонных классов.
			
	-	Вы получаете более понятные сообщения об ошибках, 
		поскольку компилятор сравнивает требования к шаблонным параметрам 
		с заданными аргументами шаблона.
		
	-	Вы можете использовать предопределенные концепты и создавать свои собственные.
	
	-	Единое использование 'auto' и концептов. 
		
		Вместо 'auto' вы можете использовать концепты.
		
	-	Если объявление функции использует концепты, 
		то она автоматически становится 'шаблонной функцией'. 
		
		Таким образом, написание шаблонной функции 
		становится таким же простым, как и написание обычной функции.
*/
/*
	 4.1.4 Использования концептов
 
	На самом деле есть четыре способа использования концептов.
 
	4.1.4.1 Четыре пути использования концептов
 
	Я использую предопределенный концепт std::integral 
	в программе  conceptsIntegralVariations.cpp всеми четырьмя способами.
	
	Определение наибольшего общего делителя (НОД) - 	GCD	-	Greatest Common Devisor.
*/
//	1.
template<typename T>                                  
requires std::integral<T>
T gcd0(T a, T b) {
    if( b == 0 ) 	
		return a;
    else 			
		return gcd0(b, a % b);
}
//	2.
template<typename T>                                  
T gcd1(T a, T b) requires std::integral<T> {
    if( b == 0 ) 	
		return a; 
    else 			
		return gcd1(b, a % b);
}
//	3.
template<std::integral T>                                  
T gcd2(T a, T b) {
    if( b == 0 ) 	
		return a; 
    else 			
		return gcd2(b, a % b);
}
//	4.
std::integral auto gcd3(std::integral auto a, std::integral auto b) { 
    if( b == 0 ) 	
		return a; 
    else 			
		return gcd3(b, a % b);
}
//	5.
template<typename T>
concept	integral	=	
						std::integral<T>;

integral auto gcd4(integral auto a, integral auto b) { 
    if( b == 0 ) 	
		return a; 
    else 			
		return gcd4(b, a % b);
}

//	-------------------------------------------------------------------------------
void test_1(){
	cout << "-------------- I'm void " << __func__ << "()" << endl;

    cout << "gcd0(100, 10)= " <<  gcd0(100, 10)  << '\n';
    cout << "gcd1(101, 10)= " <<  gcd1(101, 10)  << '\n';
    cout << "gcd2(102, 10)= " <<  gcd2(102, 10)  << '\n';
    cout << "gcd3(103, 10)= " <<  gcd3(103, 10)  << '\n';
	cout << "gcd4(104, 10)= " <<  gcd4(104, 10)  << '\n';
    cout << '\n';
}
/*
	Пример выше демонстрирует четыре способа использования концептов:
	
	-	директива requires  								(Requires clause);
	-	директива requires, идущая за объявлением функции 	(Trailing requires clause);
	-	параметр шаблона с ограничением 					(Constrained template parameter);
	-	сокращенный шаблон функции 						(Abbreviated function template).
*/
/*
	 4.1.4.2 Директива requires (Requires clause)
 
	В предыдущей программе conceptsIntegralVariations.cpp показано, как можно использовать концепт 
	для определения обычной или шаблонной функции. 
	
	На самом деле есть гораздо больше различных случаев. 
	
	Для полноты нужно добавить, что при помощи концептов вы можете задавать также возвращаемое функцией значение.
	
	Ключевое слово 'requires' вводит 'директиву requires', 
	которая задает условия на аргумент шаблона (gcd) или объявления функции (gcd1). 
	
	За requires должен идти 'предикат времени компиляции', 
	такой как 
	-	именованный концепт 								(named concept), 
	-	конъюнкция или дизъюнкция именованных концептов 	(conjunction/disjunction of named concepts)
	и 
	-	requires-выражение								(requires expression).
		
	Предикат времени компиляции (compile-time predicate) также может быть выражением.
	
	Использование 'предиката времени компиляции' в 'директиве requires'.
*/
template <unsigned int i>
	requires 
				(i <= 20) 	//	предикат времени компиляции
int sum(int j) {
    return i + j;
}
void test_2(){
	cout << "-------------- I'm void " << __func__ << "()" << endl;

    cout << "sum<20>(2000): " << sum<20>(2000) << '\n',
	cout << "sum<10>(2000): " << sum<10>(2000) << '\n',
    //	cout << "sum<23>(2000): " << sum<23>(2000) << '\n',  // ERROR requires (i <= 20)
    cout << '\n';
}
/*
	4.1.4.3 Концепты как возвращаемое значение функции
	
	Использование концепта для задания возвращаемого типа значения функции
*/ 
template<typename T>
	requires std::integral<T>
std::integral auto 		gcd(T a, T b) {
	if( b == 0 ) 
		return a;
	else 
		return gcd(b, a % b);
}
/* 
	4.1.4.4 Примеры использования концептов
	
	Самое важное – это то, что концепты являются предикатами времени компиляции. 
	
	Предикат времени компиляции 	– 	это функция, которая выполняется во время компиляции 
									и возвращает логическое значение. 
									
	Концепты	-	это просто функции, возвращающие логические значения во время компиляции. 
	
	4.1.4.4.1 Предикаты времени компиляции
	
	Концепт может быть использован в управляющей структуре.
 
	Концепты как предикаты времени компиляции
*/
struct Test{};

void test_3(){
	cout << "-------------- I'm void " << __func__ << "()" << endl;
    cout << std::boolalpha;
	cout << "std::three_way_comparable<int>: " << std::three_way_comparable<int> << "\n";

    cout << "std::three_way_comparable<double>: ";
    if (std::three_way_comparable<double>) 
		cout << "True";
    else 
		cout << "False";
    cout << "\n\n";

    static_assert(std::three_way_comparable<std::string>);

    cout << "std::three_way_comparable<Test>: ";
    if constexpr(std::three_way_comparable<Test>) 
		cout << "True";
    else 
		cout << "False";
    cout << '\n';

    cout << "std::three_way_comparable<std::vector<int>>: ";
    if constexpr(std::three_way_comparable<std::vector<int>>) 
		cout << "True";
    else 
		cout << "False";
    cout << '\n';
}
/*
	В приведенной выше программе концепт std::three_way_comparable<T>, 
	который во время компиляции проверяет различные типы 'Т' на оператор сравнения. 
	
	Поскольку это предикат времени компиляции, то он может быть использован как во время выполнения, 
	так и во время компиляции. 
	
	Конструкции static_assert и constexpr if вычисляются во время компиляции.
	https://en.cppreference.com/w/cpp/language/if
	
	4.1.4.4.2 Шаблоны классов
	
	Шаблонный класс MyVector требует, чтобы его параметр шаблона 'Т' был regular, 
	т.е. вел себя примерно как int. 
	
	Формальное определение regular будет приведено позже.
	
	Использование концепта при определении класса
*/
template <std::regular T>
class MyVector
{};
void test_4(){
	cout << "-------------- I'm void " << __func__ << "()" << endl;

    MyVector<int> 	myVec1;
    //	MyVector<int&> myVec2;  // ERROR because a reference 'int&' is not regular
}
/*
	4.1.4.4.3 Функции-члены, не являющиеся шаблонными
 
	В этом примере функция-член push_back() класса MyVector. 
	
	Этот метод требует, чтобы аргумент можно было скопировать.
 
	Использование концепта в функции-члене
*/
struct NotCopyable {
    NotCopyable() 					= default;
	
	NotCopyable(const NotCopyable&) 		= delete;
};
template <typename T>
struct MyVector_requires_copiable{
    void push_back(const T&) requires std::copyable<T> 
	{}
};
void test_5(){
	cout << "-------------- I'm void " << __func__ << "()" << endl;

    MyVector_requires_copiable<int> 			myVec1;
    myVec1 .push_back(2020);

    MyVector_requires_copiable<NotCopyable> 	myVec2;
    //myVec2 .push_back(NotCopyable());   // ERROR because not copyable - NotCopyable(const NotCopyable&) = delete;
}
/*
	4.1.4.4.4 Шаблоны с переменным количеством аргументов (variadic templates)
	
	Вы также можете использовать концепты в шаблонах с переменным количеством аргументов.
*/
template<std::integral... Args>
bool all(Args... args) 				{ return (... && args); }

template<std::integral... Args>
bool any(Args... args) 				{ return (... || args); }

template<std::integral... Args>
bool none(Args... args) 			{ return not(... || args); }

void test_6(){
	cout << "-------------- I'm void " << __func__ << "()" << endl;
    cout << std::boolalpha << '\n';
               
    cout << "all(5, true, false): " << all(5, true, false) << '\n';  

    cout << "any(5, true, false): " << any(5, true, false) << '\n'; 
              
    cout << "none(5, true, false): " << none(5, true, false) << '\n';     
}
/*
	 Определение приведенных выше шаблонов основано на 'свертке параметров шаблона' (fold expression). 
	 
	 С++11 поддерживает шаблоны с произвольным переменным количеством аргументов. 
	 
	 Произвольное количество шаблонных параметров содержится в так называемом parameter pack – 
	 совокупности нулевого или произвольного количества типов. 
	 
	 Кроме того, в С++ вы можете применить 'операцию редукции' к этой совокупности 
	 при помощи произвольного бинарного оператора. 
	 
	 Подобная редукция называется "сверткой параметров шаблона". 
	 https://www.modernescpp.com/index.php/fold-expressions/
	 
	 В этом примере логическое И &&, логическое ИЛИ ||  и отрицание логического ИЛИ 
	 применяются как 'бинарные операторы'. 
	 
	Кроме того, all, any и none требуют от своих параметров поддержки концепта std::integral.
	 
	4.1.4.4.5 Перегрузка функций в зависимости от типа параметра, ограниченного концептом
 
	std::advance – это алгоритм в стандартной библиотеке шаблонов. 
	https://en.cppreference.com/w/cpp/iterator/advance
	
	Он сдвигает заданный итератор iter на n элементов. 
	
	Исходя из возможностей, итераторы могут применять различные стратегии (policy, strategy). 
	
	Например, std::forward_list поддерживает итератор, который может передвигаться только в одном направлении, 
	в то время как std::list поддерживает итератор, который может передвигаться в обоих направлениях, 
	а std::vector поддерживает итератор с произвольным доступом. 
	
	Соответственно, для итератора, предоставленного std::forward_list или std::list, 
	вызов std::advance(iter, n) должен выполнить n операций инкремента (см. структуру std::list). 
	
	Эта сложность по времени отличается от сложности по времени для std::random_access_iterator, 
	предоставляемого std::vector. 
	
	В последнем случае мы просто прибавляем к итератору число n. 
	
	Таким образом, линейная сложность по времени O(n) превращается в константную сложность O(1). 
	
	Мы можем использовать концепты для того, чтобы различать различные типы итераторов. 
	
	Перегрузка шаблонных функций с использованием концептов
*/
template<std::forward_iterator 	I>
void advance(I& iter, int n){
    cout << "forward_iterator" << '\n';
}

template<std::bidirectional_iterator 	I>
void advance(I& iter, int n){
    cout << "bidirectional_iterator" << '\n';
}

template<std::random_access_iterator 	I>
void advance(I& iter, int n){
    cout << "random_access_iterator" << '\n';
}

void test_7(){
	cout << "-------------- I'm void " << __func__ << "()" << endl;

    std::forward_list 					forwList{1, 2, 3};
    std::forward_list<int>::iterator 	itForw = forwList.begin();	//	forward_iterator
    advance(itForw, 2);

    std::list 							li{1, 2, 3};
    std::list<int>::iterator 			itBi = li.begin();			//	bidirectional_iterator
    advance(itBi, 2);

    std::vector 						vec{1, 2, 3};
    std::vector<int>::iterator 			itRa = vec.begin();			//	random_access_iterator
    advance(itRa, 2);

    cout << '\n';
}
/*
	Три варианта функции advance перегружены, исходя из концептов 
	std::forward_iterator, std::bidirectional_iterator и std::random_access_iterator. 
	
	Компилятор сам выберет "наиболее подходящий" вариант. 
	
	Это значит, что для tsd::forward_list будет выбран вариант, удовлетворяющий концепту std::forward_iterator, 
	для std::list будет выбран перегруженный вариант, исходя из концепта std::bidirectional_iterator, 
	и для std::vector будет выбран перегруженный вариант, основанный на концепте std::random_access_iterator. 
	
	Это происходит, поскольку std::random_access_iterator является std::bidirectional_iterator, 
	а std::bidirectional_iterator, в свою очередь, является std::forward_iterator.
	
	4.1.4.4.6 Специализация шаблонов
 
	При помощи концептов также можно специализировать шаблоны.
*/
template <typename T>
struct Vector {
    Vector() {
        cout << "Vector<T>" << '\n';
    }
};
template <std::regular Reg>
struct Vector<Reg> {
    Vector() {
        cout << "Vector<std::regular>" << '\n';
    }
};

void test_8(){
	cout << "-------------- I'm void " << __func__ << "()" << endl;

    Vector<int> 	myVec1;		// 	template <std::regular Reg>
    Vector<int&> 	myVec2;		//	template <typename T>
    cout << '\n';
}
/*
	 При инстанциации шаблона класса компилятор выбирает наиболее подходящий вариант. 
	 
	 Это значит, что для Vector<int> myVec будет выбрана 'частичная специализация', 
	 основанная на std::regular. 
	 
	 А вот для ссылочного Vector<int&> myVec2 тип int& не является регулярным, 
	 и поэтому будет выбрана 'базовая версия'.
	 
	 4.1.4.4.7 Использование нескольких концептов
 
	До сих пор применение концептов было крайне простым и одновременно использовалось не более одного концепта. 
	
	Использование более чем одного концепта
*/	
template	<	typename	Iter
			,	typename	Val
			>
	requires	std::input_iterator<Iter>
			&&	std::equality_comparable<Iter>
			&&	std::equality_comparable<Val>
Iter	find(Iter	b,	Iter	e,	Val	v)
{
	return	b	+	e;
}
/*
	Функция find() требует, чтобы итератор Iter и его сравнение с заданным значением Val 
	было выполнено при следующих условиях:
	
	-	Iter должен быть входным итератором;
	
	-	значение, соответствующее итератору Iter, должно быть сравнимо на равенство с Val.
	
	Это же требование к итератору может быть выражено 
	при помощи 'ограничивающего шаблонного параметра'.
*/
template	<	std::input_iterator		Iter
			, 	typename				Val
			>
	requires	std::equality_comparable<Iter>
			&&	std::equality_comparable<Val>
Iter	find(Iter	b,	Iter	e,	Val	v)
{
	return	b	+	e;
}
/*
	4.1.5 Ограниченные или неограниченные заполнители
 
	Сначала давайте поговорим об асимметрии в С++14.
 
	4.1.5.1 Большая асимметрия в С++14
 
	В стандарте С++14 у нас есть обобщенные лямбда-функции (generic lambda). 

	'Обобщенные лямбды' используют 'auto' вместо указания конкретного типа.
	
	Сравнение обобщенных лямбд и шаблонных функций
*/

auto addLambda = [](auto fir, auto sec)	{ return fir + sec; }; 

template <typename T, typename T2>                            
auto addTemplate(T fir, T2 sec)			{ return fir + sec; }

void test_9(){
	cout << "-------------- I'm void " << __func__ << "()" << endl;
    cout << std::boolalpha << '\n';

    cout << addLambda(1, 5) 	<< " " << addTemplate(1, 5) 	<< '\n';
    cout << addLambda(true, 5) 	<< " " << addTemplate(true, 5) 	<< '\n';
    cout << addLambda(1, 5.5) 	<< " " << addTemplate(1, 5.5) 	<< '\n';
    
    const std::string fir{"ge"};
    const std::string sec{"neric"};
    cout << addLambda(fir, sec) << " " << addTemplate(fir, sec) << '\n';
    // 	'Обобщенная ламбда' и 'шаблонная функция' дают 'одинаковые результаты'.
}
/*
	Обобщенная лямбда дает новый способ задания шаблонных функций. 
	
	Mожем ли мы использовать auto для получения шаблонов функций? 
	
	Для С++14 ответ – нет, но для С++20 ответ – да.
	
	В С++20 вы можете использовать 
	'заполнитель без ограничений' (unconstrained placeholders)
	(auto) 
	или 
	'заполнитель c ограничением' (constrained placeholder) 
	(концепт) 
	в описании функции для автоматического получения шаблонной функции. 
	
	Правило применения очень простое. 
	-----------------------------------------------------
	В каждом месте, где можно использовать 'auto', 
	можно использовать 'концепт'. 
	-----------------------------------------------------
	
	Более подробно рассмотрим это в разделе, 
	посвященном 'сокращенным шаблонным функциям' (abbreviated function templates).
	
	4.1.5.2 	Заполнители
	
	Использование 'заполнителя с ограничением' (constrained placeholder) 
	вместо 'заполнителя без ограничений' (Unconstrained Placeholder)
*/
std::integral auto 	getIntegral(int val){                    
    return val;
}

void test_10(){
	cout << "-------------- I'm void " << __func__ << "()" << endl;
    cout << std::boolalpha << '\n';
 
    std::vector<int> 	vec{1, 2, 3, 4, 5};
    for (std::integral auto i_elem: vec) 			//	'заполнитель с ограничением' (constrained placeholder) 
		cout << i_elem << " ";  
	cout << '\n';
    std::integral auto 	b = true;        			//	'заполнитель с ограничением' (constrained placeholder) 
    cout << b << '\n';

    std::integral auto 	integ = getIntegral(10);	//	'заполнитель с ограничением' (constrained placeholder)              
    cout << integ << '\n';

    auto 				integ1 = getIntegral(10);   // 	'заполнитель без ограничений' (Unconstrained Placeholder)
    cout << integ1 << '\n';
}
/*
	Концепт std::integral может использоваться 
	как возвращаемый тип в цикле по контейнеру 
	или как тип переменной b 
	либо как тип переменной integ. 
	
	Для того чтобы увидеть симметрию между auto и концептами, 
	используется auto вместо std::integral auto. 
	
	Поэтому integ1 может принять значение любого типа.
	
	 4.1.6 Сокращенные шаблонные функции
	
	В C++20 можно использовать заполнитель без ограничений (auto) 
	или заполнитель с ограничением (концепт) в описании функции, 
	и тогда описание этой функции 
	автоматически становится шаблоном. 

//	1.
template<typename T>                                  
requires std::integral<T>
T gcd0(T a, T b) {
    if( b == 0 ) 	
		return a;
    else 			
		return gcd0(b, a % b);
}
//	2.
template<typename T>                                  
T gcd1(T a, T b) requires std::integral<T> {
    if( b == 0 ) 	
		return a; 
    else 			
		return gcd1(b, a % b);
}
//	3.
template<std::integral T>                                  
T gcd2(T a, T b) {
    if( b == 0 ) 	
		return a; 
    else 			
		return gcd2(b, a % b);
}
//	4.	-	описание этой функции автоматически становится шаблоном. 
std::integral auto gcd3(std::integral auto a, std::integral auto b) { 
    if( b == 0 ) 	
		return a; 
    else 			
		return gcd3(b, a % b);
}
//	5.	-	описание этой функции автоматически становится шаблоном. 
template<typename T>
concept	integral	=	
						std::integral<T>;
integral auto gcd4(integral auto a, integral auto b) { 
    if( b == 0 ) 	
		return a; 
    else 			
		return gcd4(b, a % b);
}
//	-------------------------------------------------------------------------------
void test_1(){
	cout << "-------------- I'm void " << __func__ << "()" << endl;

	cout << "gcd0(100, 10)	= " <<  gcd0(100, 10)  << '\n';
	cout << "gcd1(101, 10)	= " <<  gcd1(101, 10)  << '\n';
	cout << "gcd2(102, 10)	= " <<  gcd2(102, 10)  << '\n';
	cout << "gcd3(103, 10)	= " <<  gcd3(103, 10)  << '\n';
	cout << "gcd4(104, 10)	= " <<  gcd4(104, 10)  << '\n';
	cout << '\n';
}

	Позвольте мне подчеркнуть эту симметрию демонстрацией еще одного примера.
	
	Используя 'auto' как параметр типа, 'обычная идли лямбда-функция' add() становится 'шаблонной функцией' 
	и полностью эквивалентна одноименной 'шаблонной функции' add().
*/
template<typename T, typename T2>
auto add_templ(T fir, T2 sec) {
	return fir + sec;
}
auto add_templ_(auto fir, auto sec) {
	return fir + sec;
}
/*
	Соответственно, в связи с использованием концепта std::integral функция sub() 
	эквивалентна шаблонной функции sub().
	
	Эквивалентные функция и шаблонная функция
*/
template<std::integral T, std::integral T2>
std::integral auto sub_templ(T fir, T2 sec) {
	return fir - sec;
}
std::integral auto sub_templ_(std::integral auto fir, std::integral auto sec) {
	return fir - sec;
}
/*
	Приведенные выше обе пары функций компилятор считает эквивалентными и останавливает компиляцю.
	
	И простая функция, и шаблонная функция могут иметь произвольные типы. 
	 
	Это значит, что оба типа могут быть разными, но оба обязаны быть целочисленными. 
	 
	Например, вызовы sub(100, 10) и sub(100, true) будут корректными.
	
	Есть еще одна интересная возможность, 
	пока отсутствующая в синтаксисе сокращенных шаблонных функций: 
	вы можете 'перегрузить функции', 
	используя 'auto' или 'концепты'.
*/
