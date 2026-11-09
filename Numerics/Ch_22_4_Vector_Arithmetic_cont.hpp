	//		#pragma once -> SF.8: Use #include guards for all header files
#ifndef INCLUDE_GUARDS_ERRORS_CH_22_4_VECTOR_ARITHMETIC_CONT_HPP
#define INCLUDE_GUARDS_ERRORS_CH_22_4_VECTOR_ARITHMETIC_CONT_HPP
/*
	Ch_22_4_Vector_Arithmetic_cont.hpp
*/	
#include	<iostream>
using std::cout; using std::endl; using std::boolalpha; using std::cin; using std::cerr;
	
#include	<vector>
#include	<string>
#include	<cassert>
#include <errno.h>
#include <cstdio>
#include <cstdlib>
#include <cstddef>
#include <memory>
#include <iterator>
#include <algorithm>

#include <limits>
#include <climits>
#include <valarray>

#include "..\type_name_as_str.h"
/*
	***************************************************************************************************
	22.4. Векторная арифметика
	
	Cтандартная библиотека языка C++ 
	также предоставляет специальный векторный класс — std::valarray — 
	специально разработанный для быстрых численных операций над векторами.
*/
namespace SEH 
{
	/*
		*********************	slice_array		*********************************************************
		https://runebook.dev/ru/docs/cpp/numeric/valarray/slice_array
		
		22.4.6. Массив slice_array
		
		Из массива valarray и среза мы можем построить нечто похожее на valarray, 
		но на самом деле являющееся лишь способом обратиться к подмножеству массива, 
		адресуемого срезом. 
		
		Это slice_array, который определяется следующим образом:
	*/
	template		<class T> 
	class slice_array	{
							/*	std::slice_array 	— 	это вспомогательный шаблон, 
														используемый valarray subscript operator 
														с аргументом std::slice. 
														
								Он имеет семантику ссылки на подмножество массива, 
								заданное объектом std::slice .
							*/
	  public:						
		//	Member types
		using	value_type		=	T;							
		//	Constructors & destructor
		//slice_array	(const slice_array& 	other);
		
		~slice_array();					//	Уничтожает индексы в массиве. 
										//	Элементы, на которые ссылается объект, не изменяются.
										
		slice_array	() = delete;										//	Конструктор по явно определен как удаленный: 
																	//	slice_array не является DefaultConstructible.
																	//	предотвращаем конструирование
		slice_array (const slice_array&	other)  = delete; 			// 	предотвращаем копирование
		slice_array& operator= (const slice_array&	 other)  = delete; 	//	предотвращаем копирующее присваивание
		
		//	https://runebook.dev/ru/docs/cpp/numeric/valarray/slice_array/operator=
		// 	Назначает 'value' всем элементам
		void 				operator=	(const T& 				value) 		const;		
		//	Присваивает элементы 'val_arr' упомянутым элементам *this 
		void 				operator=	(const std::valarray<T>& 	val_arr) 	const;		
		//	Назначает выбранные элементы из other_arr указанным элементам *this	
		const slice_array& 	operator=	(const slice_array& 		other_arr) 	const;		
		/*
			value		-		значение, которое нужно присвоить всем указанным элементам
			val_arr		-		std::valarray для назначения
			other_arr	-		std::slice_array для назначения
		*/									
		
		std::valarray<T>* 	ptr; 
		std::slice 			slc;
	};
	
	/*
		****************************************************************************************************
		Стандартная библиотека не предоставляет никаких матричных классов. 
		
		Вместо этого, она предлагает использовать std::valarray и срезы 
		в качестве инструмента построения матриц, 
		оптимизированных для разных задач. 
		
		Рассмотрим, как можно при помощи valarray и slice_array 
		реализовать простую двумерную матрицу:
	*/
	#if 0	
	class Matrix
	{ 
		int 					dim; 
		std::valarray<int> 	data; 
	  public: 
		explicit Matrix 	(int dim, int init =  0) 
					: 	dim{dim} 
					, 	data(init, dim * dim)  
					{} 
		void 	clear 		(int value =  0)  					{ 	data = value ;  } 
		void 	identity 	()  								{ 	clear(); 	dialog()  =  1;  } 
		int & 	operator 	()(int x, int y)  					{  	return data[dim * y + x];  }
 
		std::slice_array<int> 	dialog				()			{ 
			return 	data[std::slice(0, dim, dim + 1)]; 
		} 
		std::slice_array<int> 	secondary_diagonal	()			{ 
			return 	data[std::slice(dim - 1, dim, dim - 1)]; 
		} 
		std::slice_array<int> 	row 					(size_t row)	{ 
			return 	data[std::slice(dim * row, dim, 1)]; 
		} 
		std::slice_array<int>column					(std::size_t col) 	{ 
			return data[std::slice(col, dim, dim)]; 
		} 
		
		template <unsigned, unsigned>  
		friend  class MatrixStack; 
	};
	#endif	
	/*
		****************************************************************************************************
		22.4.8. Обобщенные срезы
		 
		std::gslice — это класс 'селектора', 
		который идентифицирует подмножество std::slice-индексов 
		массивов std::valarray, 
		определяемых 'многоуровневым набором'
		шагов и размеров. 
		
		https://runebook.dev/ru/docs/cpp/numeric/valarray/gslice
	*/
	class gslice
	{
		//	std::gslice::gslice
		//	Конструкторы 
		gslice	();
		//	Конструктор, создающий семейсвто срезов с одним общим стартом,
		//	но с различными размерностями и шагами выборки из некоторого массива или различных массивов
		gslice	( 	std::size_t 							start
				, 	const std::valarray<std::size_t>& 	sizes
				,   const std::valarray<std::size_t>& 	strides 
				);
		//	Конструктор копирования
		gslice	( 	const gslice& 						other 
				);
		/*
			start	-	положение первого элемента
			sizes	-	массив, который определяет количество элементов в каждом измерении
			strides	-	массив, определяющий количество позиций 
						между последовательными элементами в каждом измерении
			other	-	еще один фрагмент для копирования
		*/		
		size_t 					start 		() 		const; 				// 	индекс первого элемента
		std::valarray<size_t> 	size 		() 		const; 				// 	число элементов измерения 
		std::valarray<size_t> 	stride 		() 		const; 				// 	шаг для index[0], index[1], ....		
	};
	//	----------------------------------------------------------------------------------------------------
	template <class T> 
	class 	gslice_array
	{
		using 	value_type	=	T;
		
	  public:
		gslice_array		() = delete;
		gslice_array		(const 	gslice_array& 	other);
		/*
			1) 	Конструктор по умолчанию объявлен явно определен как удаленный: 
				gslice_array не является DefaultConstructible .
				
			2) 	Создает gslice_array из другого gslice_array 	other.
		*/
		//	std::gslice_array<T>::operator+=,-=,*=,/=,%=,&=,|=,^=,<<=,>>=
		void operator+=	( const std::valarray<T>& other ) const;
		void operator-=	( const std::valarray<T>& other ) const;
		void operator*=	( const std::valarray<T>& other ) const;
		void operator/=	( const std::valarray<T>& other ) const;
		void operator%=	( const std::valarray<T>& other ) const;
		void operator&=	( const std::valarray<T>& other ) const;
		void operator|=	( const std::valarray<T>& other ) const;
		void operator^=	( const std::valarray<T>& other ) const;
		void operator<<=	( const std::valarray<T>& other ) const;
		void operator>>=	( const std::valarray<T>& other ) const;
	};
	//	----------------------------------------------------------------------------------------------------
}	//	End of namespace SEH
/*
	****************************************************************************************************
*/
#endif 	// INCLUDE_GUARDS_ERRORS_CH_22_4_VECTOR_ARITHMETIC_CONT_HPP
