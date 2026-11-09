#ifndef INCLUDE_GUARDS_Random_Number_s_Auxiliaries_HPP
#define INCLUDE_GUARDS_Random_Number_s_Auxiliaries_HPP
#pragma once
// 	Random_Number_s_Auxiliaries.hpp
/*
	https://en.cppreference.com/w/cpp/numeric/random
	
*/
#include <random>
#include <limits>
#include <cstddef>
#include <type_traits>
#include <algorithm>
#include <vector>
#include <sstream>
#include <iostream>
using std::cout; using std::endl; using std::boolalpha;
//#include "..\..\include\type_name_as_str.h"
#include "type_name_as_str.h"


/*
	*************************************************************************************************************************
	Распечатка по возможности всех свойств генератора случайных чисел
*/	
template <typename EngineType>
static 	void 	print_Engines_traits (EngineType& 		random_number_engine){
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	
	//	Информация, общая для всех генераторов случайных чисел
	cout << "Тип генератора случайных чисел == " << type_name_as_str<EngineType>() << endl;
	cout << "Тип генератора случайных чисел == " << type_name_as_str<decltype(random_number_engine)>() << endl;
	cout << "max() генератора случайных чисел == " << random_number_engine.max() << endl;
	cout << "min() генератора случайных чисел == " << random_number_engine.min() << endl;
	
	//	Информация, специфическая для различных генераторов случайных чисел
	
	if constexpr (typeid(EngineType) 		== typeid(std::default_random_engine)){
		cout << "std::default_random_engine" << endl;
	}
	
	if constexpr (typeid(EngineType) 		== typeid(std::mt19937_64)){
		cout << "std::mt19937_64" << endl;
	}

	if constexpr (typeid(EngineType) 		== typeid(std::knuth_b))	{
		cout << "std::knuth_b" << endl;
	}	
	
		if constexpr (typeid(EngineType) 	== typeid(std::ranlux24)){
		cout << "std::ranlux24" << endl;
	}

	if constexpr (typeid(EngineType) 		== typeid(std::minstd_rand0 ))	{
		cout << "std::minstd_rand0 " << endl;
	}
}
/*
	*********************************************************************************************************************
*/
/*
	*************************************************************************************************************************
	Распечатка по возможности всех свойств распределения случайных чисел
*/	
template <typename DistributionType>
static 	void 	print_Distributions_traits (DistributionType& 		random_number_distribution){
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	
	//	Информация, общая для всех распределений случайных чисел
	cout << "Тип распределения случайных чисел == " << type_name_as_str<DistributionType>() << endl;
	cout << "Тип распределения случайных чисел == " << type_name_as_str<decltype(random_number_distribution)>() << endl;
	
	using result_s_type = DistributionType::result_type;
	cout << "Тип данных генерируемых чисел == " << type_name_as_str<result_s_type>() << endl;

	//	Информация, специфическая для различных распределений случайных чисел
	
	if constexpr (std::is_same<DistributionType, std::uniform_int_distribution<>>::value){
		cout << "std::uniform_int_distribution" << endl;
		
		using parameters_type = DistributionType::param_type;
		cout << "Тип параметров 'a' и 'b' == " << type_name_as_str<parameters_type>() << endl;

		cout << "Параметр 'a' == " << random_number_distribution.a() << endl;
		cout << "Параметр 'b' == " << random_number_distribution.b() << endl;
		
	}
	if constexpr (std::is_same<DistributionType, std::normal_distribution<>>::value){
		cout << "std::normal_distribution" << endl;
	}
	
	#if 0	
	if constexpr (typeid(DistributionType) 		== typeid(std::mt19937_64)){
		cout << "std::mt19937_64" << endl;
		
		cout << "Параметр 'a' == " << random_number_distribution.a() << endl;
		cout << "Параметр 'b' == " << random_number_distribution.b() << endl;
	}

	if constexpr (typeid(DistributionType) 		== typeid(std::knuth_b))	{
		cout << "std::knuth_b" << endl;
	}	
	
		if constexpr (typeid(DistributionType) 	== typeid(std::ranlux24)){
		cout << "std::ranlux24" << endl;
	}

	if constexpr (typeid(DistributionType) 		== typeid(std::minstd_rand0 ))	{
		cout << "std::minstd_rand0 " << endl;
	}
	#endif	
}

//	------------------------------------------------------------------------------------------------------------------------------------------
//	-----------------------------------------------------------------------------------------------------------------------------------------------------------------
#endif	//	#ifndef INCLUDE_GUARDS_Random_Number_s_Auxiliaries_HPP
