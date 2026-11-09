#ifndef INCLUDE_GUARDS_RandomNumberGenerator_cont_3_HPP
#define INCLUDE_GUARDS_RandomNumberGenerator_cont_3_HPP
#pragma once
// 	RandomNumberGenerator_cont_3.hpp
/*
	Arthur O'Dwyer 	'Mastering the C++17 STL'
	Артур О’Двайр 	'Осваиваем C++17 STL' 

	Глава 11. 	'Случайные числа' 
	Chapter 11. 	'Random Numbers' 
*/
#include <random>
#include <limits>
#include <cstddef>
#include <type_traits>
#include <cassert>
#include <iostream>
using std::cout; using std::endl; using std::boolalpha;
#include <iomanip>
// 	#include "..\include_common\type_name_as_str.h"
#include "..\include\integ_to_bin_str.hpp"
/*
	**********************************************************************************************************************
	------------------------------------------------------------------------------------------------------------------
	Взвешенный выбор с discrete_distribution		-		Making weighted choices with discrete_distribution

	Метод std::discrete_distribution<int>(wbegin, wend) 
	конструирует 
	'дискретное', 
	или 'взвешенное', 
	распределение с полуоткрытым диапазоном [0, wend - wbegin). 
	
	Принцип его действия проще объяснить на примере:
*/
namespace ex17 {
template	<	class Values
		, 	class Weights
		, 	class Gen
		>
auto weighted_choice	(	const Values& 	v
					, 	const Weights& 	w
					, 	Gen& 			g
					)
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	
    auto dist 	= 	std::discrete_distribution<int>	(	std::begin	(w)
													, 	std::end		(w)
													);
    int index 	= 	dist(g);
	
    return 			v[index];
}

void test() {
	cout << "I'm TEST 17 of Examples 11" << endl;
	
    auto 					g 		= 	std::mt19937(std::random_device{}());
    std::vector<std::string> 	choices 	=	{ 	"quick"
										, 	"brown"
										, 	"fox" 
										};
    std::vector<int> 		weights 	= 	{ 	1
										, 	7
										, 	2 
										};
	
	// 	в 7 случаях из 10 ожидается, что word == "brown".
    std::string 				word 	= 	weighted_choice	(	choices
														, 	weights
														, 	g
														);
	// 	7/10 of the time, we expect word == "brown".
	cout << word << endl;	
}

} // namespace ex17
//	---------------------------------------------------------------------------------------------------------------------------------------------------------------
namespace ex18 {
/*
	Метод 
	std::discrete_distribution<int> 
	создает внутреннюю копию 'weights' 
	в скрытой переменной-члене типа 
	
	std::vector<double> 
	
	(и, как обычно для <random>, не поддерживает выбор диспетчера памяти). 
	
	Копию этого вектора можно получить вызовом dist.probabilities():
*/
void test() {
	cout << "I'm TEST 18 of Examples 11" << endl;
    int 								w[] = {1, 0, 2, 1};
    std::discrete_distribution<int> 	dist	(w, w + 4);
    std::vector<double> 				v 	= dist.probabilities();
	
    assert((v == std::vector{ 0.25, 0.0, 0.50, 0.25 }));
}
} // namespace ex18
//	--------------------------------------------------------------------------------------------------------------------------------------------------------------
namespace ex19 {
/*
	Вам редко придется использовать 'std::discrete_distribution' непосредственно; 
	в лучшем случае вы инкапсулируете его в нечто вроде предыдущей функции 'weighted_choice'. 
	
	Чтобы избежать использования динамической памяти 
	или операций с вещественными числами, 
	можно написать более простую функцию, 
	не использующую динамическую память, 
	как показано ниже:
*/
template	<class Values, class Gen>
auto 	weighted_choice	(	const Values& 			v
						, 	const std::vector<int>& 	w
						, 	Gen& 					g
						)
{
    int 		sum 		= 	std::accumulate(w.begin(), w.end(), 0);
	
	// 	Вызов функтора operator()(Generator& g)
    int 		cutoff 	= 	std::uniform_int_distribution<int>(0, sum - 1)(g);	
	
    auto 	vi 		= v.begin();
    auto 	wi 		= w.begin();
    
	while (cutoff > *wi) {
        cutoff -= *wi++;
        ++vi;
    }
    return *vi;
}
/*
	Однако выбор вещественной арифметики 
	для библиотечной реализации по умолчанию 'std::discrete_distribution' 
	имеет веское основание: 
	она избавляет от беспокойства о целочисленном переполнении. 
	
	Предыдущий код начнет производить ошибочные результаты, 
	если значение суммы превысит диапазон int.
*/
void test() {
	cout << "I'm TEST 19 of Examples 11" << endl;
    
	std::vector<std::string> 		choices 	= 	{ "quick", "brown", "fox" };
    std::vector<int> 			weights 	= 	{ 1, 7, 2 };
    std::mt19937 				g;
	
    std::string 					word 	= 	weighted_choice(choices, weights, g);
        // 7/10 of the time, we expect word=="brown".
}
} // namespace ex19
/*
	*****************************************************************************************************************
	------------------------------------------------------------------------------------------------------------
	Перемешивание карт с std::shuffle		-		Shuffling cards with std::shuffle

	Завершим эту главу знакомством с std::shuffle(a, b, g), 
	стандартным алгоритмом, 
	который принимает генератор случайных чисел. 
	
	Это 'перестановочный алгоритм'. 
	
	Он принимает диапазон элементов [a, b) 
	и перемешивает их, 
	сохраняя значения, 
	но не местоположения.
	
	Метод 
	
	std::shuffle(a, b, g) 
	
	появился в C++11 
	взамен более старого алгоритма 
	std::random_shuffle(a, b). 
	
	Старый алгоритм 
	«случайно» перемешивал диапазон [a, b), 
	но ему нельзя было передать источник случайных чисел. 
	
	На практике это означало 
	использование глобальной функции rand() 
	со всеми сопутствующими проблемами. 
	
	Как только стандарт C++11 добавил заголовок <random>, 
	определяющий стандартный способ представления генераторов случайных чисел, 
	появилась возможность избавиться от старого алгоритма 'std::random_shuffle(a, b) ', 
	основанного на rand(), 
	и, начиная с C++17,
	std::random_shuffle(a, b) 
	больше не является частью стандартной библиотеки C++.
	
	Вот как можно использовать C++11-версию std::shuffle 
	для перемешивания колоды игральных карт:	
*/
namespace ex20 {
void test() {
	cout << "I'm TEST 20 of Examples 11" << endl;
    
	std::vector<int> 	deck(52);		// Колода из 52 карт
	
	// теперь deck содержит целые числа от 1 до 52
    std::iota	(	deck.begin()
				, 	deck.end()
				, 	1
				);		// 	deck now contains ints from 1 to 52.
        
    std::mt19937 		g(std::random_device{}());
	
    std::shuffle	(	deck.begin()
				, 	deck.end()
				,	g
				);		// 	The deck is now randomly shuffled.
	/*
		Все генераторы в <random> 'полностью детерминированы', 
		то есть экземпляр std::mt19937, например, 
		инициализированный фиксированным значением, 
		будет производить в точности одну и ту же последовательность на любой платформе. 
	
		Но это не относится к распределениям, таким как 'std::uniform_real_distribution', 
		и не относится к алгоритму 'shuffle'. 
		
		Переключение с libc++ на libstdc++, или даже простое обновление версии компилятора, 
		может изменить поведение std::shuffle.
		
		Обратите внимание, 
		в предыдущем примере используется «простой» метод инициализации алгоритма Mersenne Twister, 
		а это значит, что он будет поддерживать только 4 × 109 разных вариантов перемешивания 
		из 8 × 1067 возможных! 
	
		Если вы собираетесь организовать перемешивание карт для игры в настоящем казино, 
		вам определенно стоит использовать «утомительный» метод инициализации, 
		описанный выше в этой главе, 
		или – еще проще, 
		если производительность не является проблемой, – 
		используйте std::random_device непосредственно:
	*/
    std::random_device 	  rd;
    std::shuffle		(	deck.begin()
					, 	deck.end()
					, 	rd
					);
					// The deck is now TRULY (ИСТИННО) randomly shuffled.
}
/*
	Вы можете использовать в std::shuffle любой генератор и метод инициализации. 
	
	В этом заключается огромное преимущество комбинируемого подхода 
	к генерации случайных чисел, 
	реализованного в стандартной библиотеке.
*/
} // namespace ex20

/*

*/
//	-----------------------------------------------------------------------------------------------------------------------------------------------------------------
#endif	//	#ifndef INCLUDE_GUARDS_RandomNumberGenerator_cont_3_HPP
