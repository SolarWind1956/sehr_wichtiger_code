// 	Chapter_2_cont_9.hpp 
#ifndef INCLUDE_GUARDS_Chapter_2_cont_9_HPP
#define INCLUDE_GUARDS_Chapter_2_cont_9_HPP
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

#include "coreinfo_seh.hpp"
#include "..\include\type_name_as_str.h"
#include "..\include\type_name_as_str_chat_gpt.hpp"
#include "..\include\value_category_of_expression.hpp"

/*
	*******************************************************************************************************************
	2.4. 	Синтаксис `инициализации с использованием фигурных скобок`: {}	-	 Braced-Initialization Syntax: {}
 
	Инициализация с использованием {фигурных скобок} – 
	`обобщение синтаксиса инициализации` по стандарту C++03 – 
	была спроектирована с намерением обеспечить 
	`безопасное и универсальное` его применение 
	в `любом контексте инициализации`.	
	-----------------------------------------------------------------------------------------------------------------------------------------------------------------
	2.4.1. 	Описание
	
	'Списковая инициализация` (`list initialization`), 
	изначально названная `унифицированной инициализацией` (`uniform initialization`), 
	использует для этой цели {фигурные скобки}, 
	поэтому неформально упоминается как `инициализация с применением фигурных скобок` (`braced-Initialization`)
	и иногда используется в тесном взаимодействии 
	с шаблоном стандартной библиотеки C++ 
	`std::initializer_list`. 
	
	`Инициализация с применением фигурных скобок`  (`braced-Initialization`)
	предназначена для предоставления `универсального синтаксиса инициализации объектов`
	независимо 
	
	(1) 	от контекста, в котором используется этот синтаксис, 
	или 
	(2) 	от типа инициализируемого объекта. 
	
	Как мы увидим в дальнейшем, эта цель проектирования в общем и в целом была достигнута, 
	хотя и с некоторыми особенностями и недоработками.
 
	-------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/	
int example_44() {
	cout << "------- I'm " <<  __PRETTY_FUNCTION__ << " в потоке с ID == " << std::this_thread::get_id() << endl;

	
	return 0;
}	
//	--------------------------------------------------------------------------------------------------------
#endif	//	#ifndef INCLUDE_GUARDS_Chapter_2_cont_9_HPP
