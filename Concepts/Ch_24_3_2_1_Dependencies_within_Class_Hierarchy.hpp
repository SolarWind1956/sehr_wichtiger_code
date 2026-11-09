//		#pragma once -> SF.8: Use #include guards for all header files
#ifndef INCLUDE_GUARDS_CH_24_3_2_1_DEPENDENCIES_WITHIN_CLASS_HIERARCHY_HPP
#define INCLUDE_GUARDS_CH_24_3_2_1_DEPENDENCIES_WITHIN_CLASS_HIERARCHY_HPP
/*
	Ch_24_3_2_1_Dependencies_within_Class_Hierarchy.hpp
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

#include <limits>
#include <climits>

#include <random>
#include <string_view>
#include <sstream>
#include <stdexcept>
#include <concepts>

#include "..\type_name_as_str.h"
/*
	**************************************************************************************
	24.3.2.1. Зависимости внутри иерархии классов
*/ 
/*
	------------------------------------------------------------------------------------------------
	
*/
namespace SEH 
{
//	-----------------------------------------------------------------------------------------------------
template	<class T> 
class 	Buffer
{
	T	data{T{}};
 public:
	Buffer()	{
		cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	}	
// protected:	
	void 		put 			(T) {		
		cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
		cout << "// 	вызывает overflow(T) при переполнении буфера" << endl;
	} 
	T 			get 			()	{		
		cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
		cout << "// 	вызывает underflow(), если буфер пуст" << endl;
		return T{};
	}		
	//...
// private:
// protected:
	virtual int 	overflow 	(T)	=	0; 
	virtual int 	underflow 	()	=	0; 
	//...
};
//	-----------------------------------------------------------------------------------------------------
template	<class T> 
class Circular_buffer : public Buffer<T>
{
  public:	
	int 	virtual	overflow 	(T par) override {
		cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << "   par == " << par << endl;
		cout << "вызывается при переполнении буфера - при заполнении возвращаемся на начало буфера" << endl;
		return 0;
	}
	int 	virtual	underflow 	() override {
		cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
		cout << "// 	вызывается, если буфер пуст" << endl;
		return 0;
	}	
	//...
};
template	<class T> 
class Expanding_buffer: public Buffer<T>
{
  public:	
	int virtual	overflow 	(T par) override {
		cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << "   par == " << par << endl;
		cout << "вызывается при переполнении буфера - при заполнении увеличиваем размер буфера" << endl;
		return 0;
	}
	int virtual	underflow 	() override {
		cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
		cout << "// 	вызывается, если буфер пуст" << endl;
		return 0;
	}	
	//...
};
//	-----------------------------------------------------------------------------------------------------
class X_class{
  public:
	X_class()	{
		cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	}
	X_class(X_class& lhs) {
		cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	}
	X_class		operator= (X_class& lhs) {
		cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
		return lhs;
	}
	
	~X_class()	{
		cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	}
	void 	test_func()	{
		cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	}	
};
X_class&		f_free (const X_class& 	x_class_ref)	{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	return  	const_cast<X_class&>(x_class_ref);
}
class Y_class{
 public:	
	X_class*			x_class_own_ptr{nullptr};
	X_class*			x_class_cpy_ptr{nullptr};
	//extern X_class&	f_free (const X_class&);
 public:
	Y_class()	{
		cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
		x_class_own_ptr	=	new X_class();
	}
	~Y_class()	{
		cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
		delete 	x_class_own_ptr;	x_class_own_ptr = nullptr;
	}
	Y_class(X_class& x_class_ref){
		cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
		x_class_cpy_ptr	=	&x_class_ref;
	}
	//	-----------------------------------------------------------------------------------
	//	Оператор явного преобразования типа
	#if 1	
	operator X_class	()	{
		cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
		return *x_class_own_ptr;
	}
	#endif
};
//	-----------------------------------------------------------------------------------------------------
// 	Вспомнил про концепты
//	https://runebook.dev/ru/docs/gcc/c_002b_002b-concepts
//	https://kaidzu.com/cpp/concept
/*
	<---------------- .cpp test_3()
*/
template	<typename T>
concept has_type_def = requires
	{
		typename T::inner; 		// required nested member name
		typename T::outer; 		// required class template specialization
		typename T::data_type;   // required alias template substitution
	};

class Templ_Class_1{
  public:	
	using inner		=	int;
	using outer		=	int;
	using data_type	=	int;
};
class Templ_Class_2{
  public:	
	using inner		=	int;
	using outer		=	int;
};
//	https://en.cppreference.com/w/cpp/language/requires
#if defined (__clang__) || defined (_MSC_VER) || defined (_MSC_FULL_VER) 
	template	<typename T>	
		requires has_type_def<T>
#elif defined (__GNUC__) 	|| 	defined (__GNUG__)
	template	<has_type_def T>
#endif
//	Реально все три компилятора пропускают обе конструкции согласно описаниям стандарта
class ConceptsTest
{
	T	templ_class_obj;
};
//	-----------------------------------------------------------------------------------------------------
}	//	End of namespace SEH
/*
	****************************************************************************************************
*/
#endif 	// INCLUDE_GUARDS_CH_24_3_2_1_DEPENDENCIES_WITHIN_CLASS_HIERARCHY_HPP
