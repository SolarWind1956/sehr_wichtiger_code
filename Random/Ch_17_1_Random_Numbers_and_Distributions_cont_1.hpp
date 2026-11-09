#ifndef INCLUDE_GUARDS_Ch_17_1_Random_Numbers_and_Distributions_cont_1_HPP
#define INCLUDE_GUARDS_Ch_17_1_Random_Numbers_and_Distributions_cont_1_HPP
#pragma once
// 	Ch_17_1_Random_Numbers_and_Distributions_cont_1.hpp
/*
	Nicolai M. Josuttis 			'The C++ Standard Library Second Edition'
	НИКОЛАИ М. ДЖОСАТТИС		'СТАНДАРТНАЯ БИБЛИОТЕКА C++. Справочное руководство.  Второе издание' 

	Глава 	17. 					'Работа с числами' 
	Chapter 	17. 					'Numerics' 
	  	
	Параграф 	17.1. 			'Случайные числа и распределения'
	Paragraph	17.1. 			'Random Numbers and Distributions'
*/
#include <random>
#include <limits>
#include <cstddef>
#include <type_traits>
#include <algorithm>
#include <vector>
#include <map>
#include <sstream>
#include <iostream>
#include <iomanip>
using std::cout; using std::endl; using std::boolalpha;
#include "..\..\include\type_name_as_str.h"
#include "..\..\include\Random_Number_s_Auxiliaries.hpp"

/*
	*************************************************************************************************************************
	17.1.2 		Engines			-	17.1.2. Генераторы
	https://en.wikipedia.org/wiki/Pseudorandom_number_generator
	https://ru.wikipedia.org/wiki/Генератор_псевдослучайных_чисел
		
	Стандартная библиотека C++ содержит 16 генераторов случайных чисел, 
	которые можно использовать для вычисления случайных чисел, 
	комбинируя их с распределениями или перетасовками.
	
	Как указывалось выше, 'генератор случайных чисел' - 	это источник случайности, 
														имеющий 'состояние'. 
														
	Это 'состояние' определяет 
	последовательность генерируемых 'случайных значений', 
	но не 'случайных чисел'.
	
	?????????????????????????????????	-	Перевод с английского дословный:
	
	'Его состояние определяет какая последовательность случайных значений, 
	которые не являются случайными числами, 
	генерируются'.
	
	При каждом вызове функции 
	с помощью  'оператора вызова функции' 
	operator()() 
	возникает 'новое' 'случайное значение' 'без знака', 
	а 'внутреннее состояние' генератора 'изменяется', 
	чтобы 'в следующий раз'
	появилось 'новое случайное значение'.
	
	'Переходы между состояниями' и 'генерируемые значения'
	обычно точно определены. 
	
	Таким образом, 
	при 'одном и том' же состоянии 
	конкретный генератор на любой платформе 
	создаст 'одно и то же' случайное значение. 
	
	Единственным исключением является генератор 
	std::default_random_engine, 
	зависящий от реализации. 
	
	Он вычисляет предсказуемые значения, 
	но алгоритмы, лежащие в его основе, 
	на разных платформах могут быть разными.
 
	Можно, конечно, сказать, что каждое значение, 
	созданное генератором, всегда должно быть случайным, 
	но на компьютере ни одно число не бывает истинно случайным. 
	
	Если требуется действительно непредсказуемое значение, 
	состояние генератора следует обрабатывать случайным образом, 
	не зависящим от программы, 
	например, определяя количество миллисекунд 
	между двумя последовательными щелчками мышью.
	
	С другой стороны, предсказуемость случайных величин дает преимущество:
	можно создавать одинаковые случайные сценарии 
	для тестирования на основе случайных чисел. 
	
	Эту возможность демонстрирует следующая программа:
*/
//	------------------------------------------------------------------------------------------------------------------------------------------
static 	void 	printNumbers (std::default_random_engine& 		dflt_random_engine){
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
    for (int i = 0; i < 6; ++i) {
        cout << dflt_random_engine() << " ";
    }
    cout << endl;
}
//	------------------------------------------------------------------------------------------------------------------------------------------
static 	void 	random_numbers_engines()
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	
    // 	create engine and generate numbers
	cout << "\n Cоздаем генератор и генерируем случайные числа" << endl;
    std::default_random_engine 	dflt_random_engine;
    printNumbers(dflt_random_engine);

    // 	create equal engine and generate numbers
    cout << "\n Cоздаем аналогичный генератор и генерируем случайные числа" << endl;
	std::default_random_engine 	dflt_random_engine_2;  	// same 'initial state' as dflt_random_engine
    printNumbers(dflt_random_engine_2);
    
    //	create engine with initial state specified by a seed
    cout << "\n Cоздаем генератор с начальным состоянием, заданным начальным значением" << endl;
	std::default_random_engine 	dflt_random_engine_3	{42};
    printNumbers(dflt_random_engine_3);

    // 	save state of engine 'dflt_random_engine	'	-
    cout << "\n Cохраняем состояние генератора в объекте std::stringstream" << endl;
	std::stringstream 	engineState;
    engineState << dflt_random_engine;
	
	cout << "std::stringstream engineState" << endl;
	std::string 	curr_str{};
	while (engineState)	{
		engineState >> curr_str;
		cout << "next engineState " << curr_str << endl;
	}	
	
    // 	generate numbers according to current state of 'dflt_random_engine'
	cout << "\n Генерируем числа в соответствии с текущим состоянием генератора" << endl;
    printNumbers(dflt_random_engine);
    
    // 	restore saved state of engine 'dflt_random_engine' and create numbers again
	cout << "\n Восстанавливаем сохраненное состояние генератора dflt_random_engine и снова создаем числа" << endl;
    engineState >> dflt_random_engine;
    printNumbers(dflt_random_engine);

    // 	restore saved state of engine 'dflt_random_engine', 
	cout << "\n Восстанавливаем сохраненное состояние посредством std::stringstream::clear(); .seekg(0);" << endl;
    engineState.clear();   	// 		clear EOF bit
    engineState.seekg(0);  	// 		and move read position to the beginning
    engineState >> dflt_random_engine;
	
	//	skip 2 numbers and create numbers again
	cout << "\n Пропускаем два числа и снова создаем числа" << endl;
    dflt_random_engine.discard(2);
    printNumbers(dflt_random_engine);
}
/*
	После объявления генератора случайных чисел 
	(в нашем случае — заданного по умолчанию) 
	для создания случайных значений можно выполнить оператор вызова фунции
	
	operator().
*/	
static	void  random_number_engine_as_funtor()	{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	
	std::default_random_engine 	dflt_random_engine;
	print_Engines_traits<std::default_random_engine>	(dflt_random_engine);
		
	std::mt19937_64 				mersenne_twister_64_engine;
	print_Engines_traits			(mersenne_twister_64_engine);
	
	std::knuth_b 				knuth_b_engine;
	print_Engines_traits			(knuth_b_engine);
	
	std::ranlux24 				ranlux24_engine;
	print_Engines_traits			(ranlux24_engine);
	 
	
	std::minstd_rand0  			minstd_rand0_engine;
	print_Engines_traits			(minstd_rand0_engine);
	
	dflt_random_engine();
} 
/* 
	Таким образом, генератор случайных чисел - это 'функциональный объект' 
	(т.е. объект, который ведет себя как функция).
*/
/*
	Генерируются целые значения без знака. 
	
	Обычно можно задать точный тип, за исключением 
	std::default_random_engine, 
	который зависит от реализации. 
	
	Для каждого генератора операция result_type определяет тип, 
	а статические функции-члены min() и max () 
	возвращают минимальные и максимальные генерируемые значения 
	(в интервал включаются оба).
 
	Если снова объявить такой же генератор, 
	он будет иметь такое же начальное состояние 
	и будет создавать те же самые случайные значения. 
	
	Для того чтобы получить то же самое значение, 
	можно дважды передать генератор функции printNumbers () по значению. 
	
	Поскольку функция printNumbers () при каждом вызове 
	будет работать с временной копией генератора 'dflt_random_engine', 
	будут дважды созданы одни и те же случайные числа.
	
	Следующий оператор показывает, как передать начальное значение, 
	чтобы изменить начальное состояние генератора:
	
	std::default_random_engine 		dflt_random_engine(42);

	Обычно, чтобы получить действительно случайные числа, 
	в качестве начального значения необходимо передать значение, 
	поступающее извне программы 
	(например, интервал времени между двумя щелчками мышью или значение, выданное таймером).
	
	Следующий код демонстрирует другие возможности генераторов. 
	
	Например, для сохранения и восстановления состояния генератора 
	можно использовать операторы 
	
	std::ostringstream 	engineState;
	engineState << dflt_random_engine;
	
	...
	
	engineState >> dflt_random_engine;

	Следующие инструкции записывают состояние в строку с помощью строкового потока
 
	std::ostringstream 	engineState; 
	engineState << dflt_random_engine;
 
	Это 'состояние' представляет собой последовательность значений, 
	разделенных пробелами. 
	
	(Но она не является последовательностью случайных чисел!) 
	
	Если изменить состояние генератора dflt_random_engine - 
	например, генерируя числа, - 
	а затем восстановить его, 
	то генератор перейдет в состояние, 
	которое было сохранено, 
	и сгенерирует те же числа снова. 

	Следующий оператор пропускает два состояния:
	
	dflt_random_engine.discard(2);
 
	Таким образом, генератор будет находиться в таком состоянии,
	будто он уже сгенерировал два значения. 
	
	Однако зачастую функция discard() при переборе состояний 
	с помощью оператора operator() может работать быстрее.
*/
/*
	*******************************************************************************************************************************
	17.1.3. 	Подробное описание генераторов		-		 17.1.3 		Engines in Detail
	
	Генераторы в стандартной библиотеке шаблонов разделяются на несколько категорий.
	
	- 	Основные генераторы, реализующие основные алгоритмы генерации случайных значений.
 
		• 	Класс std::linear_congruential_engine
		
		• 	Класс std::mersenne_twister_engine
		
		• 	Класс std::subtract_with_carry_engine
		
	-	Адаптеры генераторов, которые можно инициализировать (основными) генераторами.
	
		• 	Класс std::discard_block_engine, 		адаптирующий генератор, 
												каждый раз отбрасывает заданное количество сгенерированных значений.
											
		• 	Класс std::independent_jbits_engine, 	адаптирующий генератор для создания случайных значений 
												с заданным количеством битов.
												
		• 	Класс std::shuffle_order_engine, 		адаптирующий генератор 
												с помощью изменения порядка генерируемых им значений.
												
	-	Адаптеры с предопределенными параметрами.
		
		• 	std::minstd_rand0
		
		• 	std::minstd_rand
		
		• 	std::mt19937
 
		• 	std::mt19937_64
 
		• 	std::ranlux24_base 
		
		• 	std::ranlux48_base
		
		• 	std::ranlux24
 
		• 	std::raniux48
 
		• 	std::knuth_b	
	
	Например, тип std::knuth_b - это сокращение для выражения
 
	std::shuffle_order_engine	<	std::linear_congruential_engine	<	uint_fast32_t
															,	16807
															,	0
															,	2147483647
															>
							,	256
							>	
							
	Кроме того, 'по умолчанию' задается тип 
	
	std::default_random_engine, 
	
	зависящий от реализации. 
	
	Это единственный тип генератора, 
	не гарантирующий создания одной и той же последовательности 
	на разных платформах.
	
	В табл. 17.1 перечислены операции над генераторами случайных чисел. 
	
	Все стандартные генераторы одного и того же типа 
	изначально имеют одно и то же состояние, 
	т.е. оператор == возвращает значение 'true', 
	и оба генератора создают одну и ту же последовательность случайных значений. 
	
	Таким образом, для создания разных случайных значений, 
	необходимо задавать разные параметры 'seed'.						
		
	Таблица 17.1. 	Операции над генераторами случайных чисел 
	------------------------------------------------------------------------------------------------------------------------------------------------------------
	Операция 					Описание
	------------------------------------------------------------------------------------------------------------------------------------------------------------
	engine 		е 				Конструктор по умолчанию; 
								создает генератор с начальным состоянием, заданным по умолчанию
								
	engine 		е (seed) 			Создает генератор, состояние которого 
								соответствует параметру 'seed'
								
	engine 		е (е2) 			Копирующий конструктор; 
								копирует генератор ('е' и 'е2' будут иметь одинаковые состояния)
								
	е.seed() 						Переводит генератор 'е' в начальное состояние
	
	е.seed (seed) 				Переводит генератор 'е' в состояние, соответствующее параметру 'seed'

	е()							Возвращает следующее случайное значение и переводит генератор в следующее состояние
	
	е.discard(n)					Переводит генератор в n-е состояние 
								(эквивалентно 'n' вызовам е(), но может работать быстрее)
	
	e1 == е2 					Проверяет 'равенство состояний' генераторов e1 и е2 
	
	el != е2 						Проверяет 'неравенство состояний' генераторов e1 и е2 
	
	ostrm << е 					Записывает состояние генератора 'е' в поток вывода 'ostrm' 

	istrm >> е					Считывает новое состояние из потока ввода в генератор 'е'
 	------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
static	void		Operations_of_Random_Number_Engines()	{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	
	std:default_random_engine		dflt_random_engine	{42};
	
	cout << "ostrm << е; Записывает состояние генератора 'е' в поток вывода 'ostrm' " << dflt_random_engine << endl;
	dflt_random_engine();
	cout << "Cостояние генератора после вот такой генерации: dflt_random_engine();  " << dflt_random_engine << endl;
}
/*
	Состояние, считанное оператором <<, 
	представляет собой список десятичных значений, разделенных пробелом.

	Несмотря на то, что этот список может выглядеть, как 
	последовательность 'случайных' значений, 
	на самом деле это не так.
	
	Стандарт гарантирует, 
	что при чтении записанного состояния,
	генератор того же типа перейдет в то же состояние 
	(оператор == возвращает 'true', 
	и генерируются одинаковые случайные значения).
*/
/*
	*********************************************************************************************************************
	17.1.4. 		Распределения		-		 17.1.4 	Distributions
	
	Pаспределения преобразуют 'случайные значения', 
	созданные генератором, 
	в реальные и полезные 'случайные числа'. 
	
	Вероятность этих случайных чисел 
	зависит от используемого распределения, 
	параметры которого задаются программистом. 
	
	В табл. 17.2 приведен обзор всех распределений, существующих в стандартной библиотеке C++.
	
	Таблица 17.2. Распределения, предусмотренные стандартной библиотекой C++
	-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	Категория распределений									Имя распределения				Тип данных
	-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	Uniform distributions		-	равномерные распределения		uniform_int_distribution			IntType
																uniform_real_distribution			RealType
  
	Bernoulli distributions		-	распределения Бернулли			bernoulli_distribution				bool
																binomial_distribution				IntType	
																geometric_distribution				IntType
																negative_binomial_distribution		IntType
	
	Poisson distributions		-	распределения Пуассона			poisson_distribution				IntType
																exponential_distribution			RealType
																gamma_distribution				RealType
																weibull_distribution				RealType
																extreme_value_distribution			RealType

	Normal distributions		-	нормальные распределения		normal_distribution				RealType
																lognormal_distribution				RealType
																chi_squared_distribution			RealType
																cauchy_distribution				RealType
																fisher_f_distribution				RealType
																student_t_distribution				RealType

	 Sampling distributions	-	выборочные распределения		discrete_distribution				IntType
																piecewise_constant_distribution		RealType
																piecewise_linear_distribution		RealType
	-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	
	Почти все распределения представляют собой шаблонные классы, 
	параметризованные типами генерируемых значений. 
	
	Единственным исключением является класс bernoulli_distribution, 
	представляющий собой обычный класс, 
	потому что он генерирует только значения типа bool.
	
	По умолчанию используются следующие типы:
	
	• 	int 		- 	для 	IntTуре
	• 	double 	-	для 	RealType;
	
	Стандартная библиотека C++ содержит следующие специализации шаблонов:
 
	• 	для IntType		- 	short, int, long, long long 
							и соответствующие им типы без знаков;
						
	• 	для RealType	-	float, double, long double.
 
	В табл. 17.3 	приведены операции над распределениями.
	
	Таблица 17.3. Типы и операции над распределениями
	------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	Операция 					Описание
	------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	distr::result_type 				Арифметический тип генерируемых значений
	
	distr 	d					Конструктор по умолчанию; создает распределение с параметрами, заданными но умолчанию
 	
	distr 	d		(args)		Создает распределение, параметризованное списком аргументов 'args'
 	
	d				(e)			Возвращает следующее значение с помощью генератора 'е' 
								и переводит генератор в состояние 'е'
 	
	d.min			()			Возвращает минимальное значение
 
	d.max			()			Возвращает максимальное значение

	d1 == d2					 Проверяет равенство состояний d1 и 62
 
	d1 != d2						Проверяет неравенство состояний d1 и 62
 
	ostrm << d					Записывает состояние 'd' в поток вывода ostrm
	
	istrm >> d					Считывает новое состояние из потока ввода istrm в 'd'
 
 	distr::param_type				Тип параметров 'distr'

	distr 	d		(pt)			Создает распределение с параметром 'param_type' 	'pt'
	
	d.param			(pt)			Задает текущий параметр равным 'param_type' 	'pt'
 
	d.param			()			Возвращает текущий параметр как значение типа param_type
 
	d				(e, pt)		Возвращает следующее значение с помощью генератора 'е' 
								и параметра param_type 'pt' и переводит генератор в состояние 'е'
								
 	d.param			()			Возвращает значение параметра 'param'
	------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	
	Распределение можно параметризовать, 
	поэтому параметры 'args' и 'pt' зависят от конкретного распределения. 
	
	Для параметризации можно:

	• 	передать список аргументов args конструктору;
	
	• 	использовать член, соответствующий параметру, чтобы запросить его значение;
	
	• 	использовать тип param_type, чтобы
	
		• 	передать эти параметры конструктору в виде одного аргумента;
		• 	передать эти параметры для генерации следующего значения;
		• 	запросить значения этих параметров.
		
	Класс distr::param_type должен иметь конструктор, 
	которому можно передавать значения параметров, 
	или именованные члены, возвращающие эти значения.
	
	Например, равномерные распределения имеют два параметра, 
	'а' и 'b', соответствующие минимуму и максимуму. 
	
	В результате эти аргументы можно передавать по отдельности.
*/
static	void		Operations_of_Random_Number_Distributions()	{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;

	std::uniform_int_distribution<> 	d(0, 20); 	// 	инициализируем параметры "a" и "b"
	
	d.a();										// 	выясняем значение параметра "а"
	d.b();										// 	выясняем значение параметра "b"
	d.param().a();								// 	выясняем значение параметра "a"
	d.param().b();								// 	выясняем значение параметра "b"

	cout		<< 	"d.param().a(); " << d.param().a() << "  " << d.a()	<< endl;
	cout		<< 	"d.param().b(); " << d.param().b() << "  " << d.b()	<< endl;
	
	std::default_random_engine					dflt_random_engine	{42};	
	//	Кроме того, можно просто передать объект класса param_type:
	// 	другая параметризация 
	std::uniform_int_distribution<>::param_type 	pt					{100, 200}; 	
	d(dflt_random_engine,	pt); 		//	генерирует одно значение в соответствии с параметризацией 'pt'
	d.param(pt); 					// 	позволяет всем сгенерированным значениям использовать параметризацию 'pt'
	cout << "" << type_name_as_str<decltype(d.param())>() << endl;	
	cout << "d.param().a() " << d.param().a()	<< endl;
	cout << "d.param().b() " << d.param().b()	<< endl;
}
/*
	'Выборочные распределения' используют специальные конструкторы 
	для передачи векторов значений или генератора значений.
	
	Отметим, что максимальное значение, 
	передаваемое распределениям как параметр, 
	иногда может включаться в интервал, а иногда нет.
	
	Для генераторов случайных чисел состояние, записанное с помощью оператора <<, 
	представляет собой десятичные значения, разделенные пробелами. 
	
	Несмотря на то что они могут выглядеть как список случайных чисел, 
	это не так.

	Следующая программа демонстрирует использование распределений:
*/
template 	<	typename 	Distribution
			, 	typename 	Engine
			>
void 	distribution_in_details	(	Distribution 		distribution
								, 	Engine 				engine
								, 	const std::string& 	name
								)
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	
    // 	print min, max and four example values
    cout << name << ":" << endl;
    cout << "- min():  " << distribution.min() 	<< endl; 
    cout << "- max():  " << distribution.max() 	<< endl; 
    cout << "- values: " << distribution(engine) 	<< ' ' 		//	Каждый раз новое случайное число
						<< distribution(engine) 	<< ' '
						<< distribution(engine) 	<< ' ' 
						<< distribution(engine) 	<< endl; 

    // 	count the generated values (converted to integral values)
    std::map<long long, int> 		value_counter;
    for (int i = 0; i < 200000; ++i) {
        value_counter[distribution(engine)]++;
    }

    // 	and print the resulting distribution
    cout << "====" << endl;
    for (auto elem : value_counter) {
        cout << std::setw(3) 	<< elem.first 	<< ": "
                                << elem.second 	<< endl;
    }
    cout << "====" << endl;
    cout << endl;
}
static 	void  distribution_in_details_test()
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << " of distribution_in_details.cpp" << endl;
    std::knuth_b 	knuth_b_engine;

    std::uniform_real_distribution<> 	uniform_distrib(0, 10);
    distribution_in_details	(	uniform_distrib
							,	knuth_b_engine
							,	"uniform_real_distribution"
							);

    std::normal_distribution<> 		normal_distrib;
    distribution_in_details	(	normal_distrib
							,	knuth_b_engine
							,	"normal_distribution"
							);

    std::exponential_distribution<> 	exponential_distrib;
    distribution_in_details	(	exponential_distrib
							,	knuth_b_engine
							,	"exponential_distribution"
							);

    std::gamma_distribution<> 		gamma_distrib;
    distribution_in_details	(	gamma_distrib
							,	knuth_b_engine
							,	"gamma_distribution"
							);
}
/*
	
*/
/*
	
*/
static	void  random_number_distributions_check()	{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	
	std::uniform_int_distribution<int> 	uniform_int_distrib;
	print_Distributions_traits(uniform_int_distrib);
	
	std::uniform_int_distribution<int> 	uniform_int_distrib_2(0, 255);
	print_Distributions_traits(uniform_int_distrib_2);
	
	std::normal_distribution<> 			normal_distrib;
	print_Distributions_traits(normal_distrib);
}

/*
	*********************************************************************************************************************
*/
//	------------------------------------------------------------------------------------------------------------------------------------------
//	-----------------------------------------------------------------------------------------------------------------------------------------------------------------
#endif	//	#ifndef INCLUDE_GUARDS_Ch_17_1_Random_Numbers_and_Distributions_cont_1_HPP
