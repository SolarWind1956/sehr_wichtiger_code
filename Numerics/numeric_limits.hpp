	//		#pragma once -> SF.8: Use #include guards for all header files
#ifndef INCLUDE_GUARDS_NUMERIC_LIMITS_HPP
#define INCLUDE_GUARDS_NUMERIC_LIMITS_HPP
/*
	numeric_limits.hpp
*/	
#include	<iostream>
using std::cout; using std::endl; using std::boolalpha; using std::cin; using std::cerr;
	
#include	<set>
#include	<bitset>
#include	<vector>
#include	<string>
#include	<cassert>
//#include <stdio.h>
#include <errno.h>
#include <cstdio>
#include <cstdlib>
#include <cstddef>
#include <memory>
#include <iterator>
#include <algorithm>
#include <functional>

#include <random>
#include <string_view>
#include <stdexcept>

#include "..\type_name_as_str.h"
/*
	***************************************************************************************************
	https://www.chem.kuleuven.be/peeters/manuals/pgi/doc/pgC++_lib/stdlibcr/num_5679.htm
*/	
//	-----------------------------------------------------------------------------------------------------
namespace SEH 
{
	enum 	float_round_style 	{	round_indeterminate       	= 	-1
								,	round_toward_zero         	=  	0
								,   round_to_nearest          	=  	1
								,	round_toward_infinity     	=  	2
								,	round_toward_neg_infinity 	=  	3
								};
}	
//	-----------------------------------------------------------------------------------------------------
/*
*/
namespace SEH
{
	//	-------------------------------------------------------------------------------------------------
	template <class T>
	class 	numeric_limits {

 public:

 // General -- meaningful for all specializations.

    static const bool 	is_specialized;
    static T 			min 					();
    static T 			max 					();
    static const int 	radix;
    static const int 	digits;
    static const int 	digits10;
    static const bool 	is_signed;
    static const bool 	is_integer;
    static const bool 	is_exact;
    static const bool 	traps;
    static const bool 	is_modulo;
    static const bool 	is_bounded;

	// Floating point specific.

    static T 			epsilon 				();
    static T 			round_error 			();
    static const int 	min_exponent10;
    static const int 	max_exponent10;
    static const int 	min_exponent;

    static const int 	max_exponent;
    static const bool 	has_infinity;
    static const bool 	has_quiet_NaN;
    static const bool 	has_signaling_NaN;
    static const bool 	is_iec559;
    static const bool 	has_denorm;
    static const bool 	tinyness_before;
	
    static const 
	float_round_style 	round_style;
    
	static T 			denorm_min 			();
    static T 			infinity 			();
    static T 			quiet_NaN 			();
    static T 			signaling_NaN 		();
};

//	Specializations
#if 0	
numeric_limits<float>
numeric_limits<double>
numeric_limits<long double>
numeric_limits<short>
numeric_limits<unsigned short>
numeric_limits<int>
numeric_limits<unsigned int>
numeric_limits<long>
numeric_limits<unsigned long>
numeric_limits<char>
numeric_limits<wchar_t>
numeric_limits<unsigned char>
numeric_limits<signed char>
numeric_limits<bool>	
#endif	
/*
	Для интегральных типов, таких как char, интерес представляет небольшая часть информации. 
	
	Вот специализация numeric_limits<char> для реализации, в которой тип char знаковый и содержит 8 бит:
*/
template<> 
class 	numeric_limits<char>
{
 public:
	static const bool 		is_specialized 	= 	true;		// 	да, информация есть
	static const int 		digits 			= 	7;			// 	число бит (исключая знак)
	static const bool 		is_signed 		= 	true;		// 	в данной реализации char знаковый	
	static const bool 		is_integer 		= 	true;		// 	char - это интегральный тип
	
	static char 				min 			() 		throw() 		{	return -128; } 	// 	наименьшее значение
	static char 				max 			() 		throw() 		{	return 127; }	// 	наибольшее значение
 
	// 	масса объявлений, не имеющих отношение к char 
};
/*
	Большинство членов numeric limits предназначены для описания чисел с плавающей запятой. 
	
	Например, вот одна из возможных реализаций для float.
*/
template<> 
class 	numeric_limits<float>
{
 public:
	static const bool 		is_specialized 	= 	true;
	static const int 		radix 			= 	2; 		// 	основание степени (в данном случае, двойки)
	static const int 		digits 			= 	24; 	// 	количество (двоичных) цифр в мантиссе	
	static const int 		digits10 		= 	6; 		//	количество десятичных цифр в мантиссе
	static const bool 		is_signed 		= 	true;
	static const bool 		is_integer 		= 	false;
	static const bool 		is_exact 		= 	false;
	static float 			min 			() throw() 		{	return 1.17549435E-38F; }
	static float 			max 			() throw() 		{	return 3.40282347E+38F; }
	static float 			epsilon 		() throw() 		{	return 1.19209290E-07F; }
	static float 			round_error 	() throw() 		{	return 0.5F; }
	static float 			infinity 	() throw() 		{	return float{}/* некоторое значение */;	}
	static float 			quiet_NaN 	() throw() 		{	return float{}/* некоторое значение */;	}
	static float 			signaling_NaN() throw() 		{	return float{}/* некоторое значение */; 	}
	static float 			denorm_min 	() throw() 		{	return min(); }
	static const int 		min_exponent 	= 	-125;
	static const int 		min_exponent10 	= 	-37;
	static const int 		max_exponent 	= 	+128;
	static const int 		max_exponent10 	= 	+38;
	static const bool 		has_infinity 	= 	true;
	static const bool 		has_quiet_NaN 	= 	true;
	static const bool 		has_signaling_NaN	= 	true;

	static const bool 		has_denorm_loss 	= 	false;
	static const bool 		is_iec559 		= 	true;								// 	отвечает IEC-559
	static const bool 		is_bounded 		= 	true;
	static const bool 		is_modulo 		= 	false;
	static const bool 		traps 			= 	true;
	static const bool 		tinyness_before 	= 	true;

	static const 			float_denorm_style_has_denorm 	= 	denorm_absent; 		//	enum из <limits> 	
	static const 			float_round_style_round_style 	= 	round_to_nearest; 	//	enum из <limits> 
}; 
/*
	min() 	— 	это самое маленькое положительное нормализованное число, 
	a epsilon — самое маленькое положительное число с плавающей запятой такое, 
	что 
	1 + epsilon - 1 
	больше нуля.
 
	Определяя некоторый скалярный тип в духе встроенных типов, 
	неплохо определить для него и подходящую специализацию numeric_limits. 
	
	Например, если бы я написал тип 'Quad' для чисел с учетверенной точностью, 
	или если бы в реализации оказался тип 'long long', 
	пользователь мог бы ожидать наличия специализации 
	numeric_limits<Quad> или numeric_limits<long long>.
	
	Теоретически можно представить себе специализацию numeric_limits для пользовательского типа, 
	имеющего мало общего с числами с плавающей запятой. 
	
	Но в таких случаях лучше использовать обычные способы 
	для представления свойств типа, 
	а не городить специализацию numeric_limits со свойствами, 
	отличными от стандартных.
 
	В numeric_limits свойства чисел с плавающей запятой 
	представляются с помощью встраиваемых функций. 
	
	Для интегральных же типов свойства должны представляться в форме, 
	допускающей их использование в константных выражениях. 
	
	Это значит, что они должны иметь инициализаторы внутри класса. 
	
	Если вы для этого используете статические константные члены, 
	а не перечисления, 
	не забудьте определить эти статические члены.
*/
	//	------------------------------------------------------------------------------------------------------
}	//	End of SEH

/*
	****************************************************************************************************
*/
#endif 	// INCLUDE_GUARDS_NUMERIC_LIMITS_HPP
