	//		#pragma once -> SF.8: Use #include guards for all header files
#ifndef INCLUDE_GUARDS_NUMERICS_HPP
#define INCLUDE_GUARDS_NUMERICS_HPP
/*
	Numerics.hpp
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

#include "..\type_name_as_str.h"
/*
	***************************************************************************************************
	Заголовочный файл #include <valarray>
 
	Массивы значений объявлены в заголовочном файле <valarray>.
*/	
#include <valarray>
//	В частности, в заголовочном файле <valarray> определены следующие классы:
#if 0	

// массив значений типа 'T'
template	<typename T> 
class 	valarray;							

// 	срез массива значений 
class 	slice;								

template	<typename T> 
class 	slice_array;
 						
// обобщенный срез 
class 	gslice;								

template <typename T> 
class 	gslice_array;

// массив значений с маской
template	<typename T> 
class 	mask_array; 							

// перечисляемый массив значений
template	<typename T> 
class 	indirect_array; 

#endif	 
/*
	Классы имеют следующий смысл:
 
	• 	valarray — основной класс, управляющий массивом значений;
	
	• 	slice и gslice определяют срез как подмножество массива значений по аналогии с библиотекой BLAS;
 
	• 	slice_array, gslice_array, mask_array и indirect_array — это внутренние вспомогательные классы,
		используемые для хранения временных значений или данных. 
		
		Эти классы невозможно использовать для непосредственного программирования интерфейса. 
		
		Они создаются косвенно определенными операциями над массивами значений.
		
		Все классы параметризованы шаблонным типом элементов. 
		
		В принципе в качестве шаблонного параметра может использоваться любой тип данных. 
		
		Однако по природе массивов значений это должен быть числовой тип.
*/	
//	-----------------------------------------------------------------------------------------------------

namespace SEH 
{
//	-----------------------------------------------------------------------------------------------------
	class slice
	{
	  public:	
		/*
			Конструктор по умолчанию. 
			Эквивалент slice(0, 0, 0). 
			Этот конструктор существует только для того, чтобы позволить создавать массивы срезов.
		*/	
		slice	();
		/*
			Создает новый срез с параметрами start, size, stride. 
			Этот срез будет относиться к количеству элементов size, 
			каждый из которых имеет позицию:
			старт + 0 * шаг
			старт + 1 * шаг
			...
			старт + (размер - 1) * шаг
		*/
		slice	( 	std::size_t 		start
				, 	std::size_t 		size
				, 	std::size_t 		stride 
				);
		//	Создает копию other.
		slice	( 	const slice& 	other );
		/*
			Parameters
			start	-	положение первого элемента
			size	-	количество элементов в срезе
			stride	-	количество позиций между последовательными элементами в срезе
			other	-	еще один фрагмент для копирования
		*/
		size_t 			start 		() 	const; 		// 	индекс первого элемента
		size_t 			size 		() 	const; 		// 	число элементов
		size_t 			stride 		() 	const; 		// 	п-ый элемент находится no адресу: start() + n * stride()
		/*
			Здесь 'шаг' (stride) 	— 	это расстояние (в элементах) 
										между последовательными элементами среза. 
										
			Таким образом, срез определяется 'последовательностью целых чисел'.
		*/
		
		friend bool 		operator==	( const slice& lhs, const slice& rhs )
		{
			return		lhs.start() 		== 	rhs.start() 
					&& 	lhs.size() 		== 	rhs.size() 
					&& 	lhs.stride() 	== 	rhs.stride();
		}			
	};
	
	template		<class T> 
	class slice_array	{
							/*	std::slice_array 	— 	это вспомогательный шаблон, 
														используемый valarray subscript operator 
														с аргументом std::slice. 
														
								Он имеет семантику ссылки на подмножество массива, 
								заданное объектом std::slice.
							*/
	  public:						
		//	Member types
		using	value_type		=	T;							
		//	Constructors & destructor
		//slice_array	(const slice_array& 	other);
		
		~slice_array();					//	Уничтожает индексы в массиве. 
										//	Элементы, на которые ссылается объект, не изменяются.
										
		slice_array	() = delete;										//	Конструктор по умолчанию 
																	//	явно определен как удаленный: 
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
	//	******************************************************************************************
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
	//	******************************************************************************************
	template		<class T> 
	class valarray
	{
	 // внутреннее представление
	 public:
		typedef T value_type;
		//	1) 	Конструктор по умолчанию. Создает пустой числовой массив.
		valarray();									//	valarray c size() == 0				
		//	2) 	Создает числовой массив из копий count элементов value-initialized .
		explicit valarray(std::size_t count);			//	count элементов co значением T{}
		//	3) 	Создает числовой массив с копиями count val .
		valarray(const T& val, std::size_t count);		//	count элементов co значением 'val'
		//	4) 	Создает числовой массив с count-копиями значений из массива, на который указывает vals. 
		//		Если этот массив содержит меньше значений count, поведение не определено.
		valarray(const T* vals, std::size_t count);	//	count элементов co значениями p[0], p[1], ...
		//	5) Копировать конструктор. Создает числовой массив с копией содержимого other.
		valarray(const valarray& other);
		//	6) Переместить конструктор. Создает контейнер с содержимым other, используя семантику перемещения.
		valarray(valarray&& other) noexcept;
		//	7-10) Converting constructor . Преобразуйте соответствующую структуру данных в valarray .
		valarray(const std::slice_array<T>& 	sa);	//	(7)	
		valarray(const std::gslice_array<T>& 	gsa);	//	(8)	
		valarray(const std::mask_array<T>& ma);		//	(9)	
		valarray(const std::indirect_array<T>& ia);	//	(10)	
		//	11) Создает числовой массив с содержимым списка инициализаторов il.
		valarray(std::initializer_list<T> il);		//	Создает новый числовой массив из различных источников.
		/*
			Parameters
			
			count		-	количество элементов, которые необходимо построить
			val			-	значение для инициализации элементов с помощью
			vals		-	указатель на массив C, 
							который будет использоваться в качестве источника для инициализации содержимого
			other		-	другой числовой массив, который будет использоваться 
							в качестве источника для инициализации содержимого
			sa			-	массив фрагментов для инициализации элементов с помощью std::slice_array<T>
			gsa			-	универсальный массив срезов для инициализации элементов с помощью std::gslice_array<T>
			ma			-	массив маски для инициализации элементов с помощью std::mask_array<T>
			ia			-	косвенный массив для инициализации элементов с помощью std::indirect_array<T>
			il			-	список инициализаторов для инициализации элементов с помощью std::initializer_list<T>
		*/	
		/*
			 Данный набор конструкторов позволяет нам инициализировать std::valarray, 
			 используя числовые массивы вспомогательных типов или отдельные значения. 
			 .cpp  <---------------------------   Например:			
		*/
		
		~valarray();
		/*
			22.4.2. Индексирование и присваивание в классе valarray
			
			В классе valarray операция индексирования применяется 
			как для доступа к отдельным элементам, так и для того, чтобы получать подмассивы:
		*/
		//	std::valarray<T>::operator[]
		const T&              	operator[]( std::size_t pos ) const;
		T&                    	operator[]( std::size_t pos );
		
		// 	срез константного массива значений возвращает новый массив значений 	std::valarray<T>
		std::valarray<T>       	operator[]( std::slice slice_arr ) const;
		// 	срез переменного числового массива возвращает объект класса 			std::slice_array<T>
		std::slice_array<T>    	operator[]( std::slice slice_arr );
		
		std::valarray<T>       	operator[]( const std::gslice& gslicearr ) const;
		std::gslice_array<T>   	operator[]( const std::gslice& gslicearr );
		std::valarray<T>       	operator[]( const std::valarray<bool>& boolarr ) const;
		std::mask_array<T>     	operator[]( const std::valarray<bool>& boolarr );
		std::valarray<T>       	operator[]( const std::valarray<std::size_t>& indarr ) const;
		std::indirect_array<T> 	operator[]( const std::valarray<std::size_t>& indarr );
		/*
			Parameters
			pos			-	позиция возвращаемого элемента
			slicearr	-	slice элементов для возврата
			gslicearr	-	gslice элементов для возврата
			boolarr		-	маска элементов для возврата
			indarr		-	индексы возвращаемых элементов
		*/
		//	std::valarray<T>::operator=
		valarray<T>& operator=( const valarray<T>& other );
		valarray<T>& operator=( valarray<T>&& other ) noexcept;
		valarray<T>& operator=( const T& val );
		valarray<T>& operator=( const std::slice_array<T>& other );
		valarray<T>& operator=( const std::gslice_array<T>& other );
		valarray<T>& operator=( const std::mask_array<T>& other );
		valarray<T>& operator=( const std::indirect_array<T>& other );
		valarray<T>& operator=( std::initializer_list<T> il );
		/*
			Заменяет содержимое числового массива.
			
			1) 	Oператор копирующего присваивания. 
			
				Если size() != other.size(), 
				сначала измените размер *this, 
				как если бы это было resize(other.size()). 
				
				Каждому элементу *this присваивается значение соответствующего элемента other.
				
			2) 	Oператор перемещающего присваивания. 
			
				Заменяет содержимое *this на содержимое other. 
				
				Значение other после этой операции не определено. 
				
				Сложность этой операции может быть линейной, 
				если T имеет нетривиальные деструкторы, 
				но в противном случае она обычно постоянна.
				
			3) 	Заменяет каждое значение в *this копией val.
			
			4-7) Заменяет содержимое *this результатом обобщенной операции индексации. 
			
			Поведение не определено, если длина size() не равна длине other 
			или если какое-либо значение слева зависит от значения справа 
			(например, v = v[v > 2]).

			8) 	Назначает содержимое списка инициализаторов il. 
				Эквивалент *this = valarray(il).
		*/
		/*
			----------------------------------------------------------------------------------------------------
			22.4.3. Функции-члены
			Для класса valarray определено некоторое количество совершенно очевидных 
			и несколько менее очевидных функций-членов:
		*/
		valarray<T> 		operator+() const;
		valarray<T> 		operator-() const;
		valarray<T> 		operator~() const;
		valarray<bool> 	operator!() const;
		
		// составное присваивание
		valarray& operator*= (const T&);
		valarray& operator/= (const T&);
		valarray& operator%= (const T&);
		valarray& operator+= (const T&);
		valarray& operator-= (const T&);
		valarray& operator^= (const T&);
		valarray& operator&= (const T&);
		valarray& operator|= (const T&);
		valarray& operator<<=(const T&);
		valarray& operator>>=(const T&);
	 
		valarray& operator*= (const valarray&);
		valarray& operator/= (const valarray&);
		valarray& operator%= (const valarray&);
		valarray& operator+= (const valarray&);
		valarray& operator-= (const valarray&);
		valarray& operator^= (const valarray&);
		valarray& operator|= (const valarray&);
		valarray& operator&= (const valarray&);
		/*
			Для массивов valarray операции '>>' и '<<'
			являются операциями 'битовых сдвигов', 
			но не операциями сдвига в элементах массива и не операциями ввода/вывода.
		*/
		valarray& operator<<(int);
		valarray& operator>>(int);
		/*
			Для выполнения битовых сдвигов в элементах интегрального типа массива valarray 
			используются операторы <<= и >>=. 
		*/
		valarray& operator<<=(const valarray&);
		valarray& operator>>=(const valarray&);
		
		//	функции-члены
		void 			swap		(valarray&) 			noexcept;
	 
		size_t 			size		() 					const;
	 
		T 				sum		() 					const;
		T 				min		() 					const;
		T 				max		() 					const;
	 
		valarray 		shift 	(int) 				const;
			
		valarray 		cshift	(int) 				const;
		valarray 		apply	(T func(T)) 			const;
		valarray 		apply	(T func(const T&)) 	const;
		/*
			Размеры массивов valarray изменять можно. 
			
			Однако resize() — это не та операция, что превращает valarray в структуру, 
			способную динамически расти как vector или string. 
			
			Вместо этого, resize() реинициализирует массив, 
			заменяя текущее содержимое умолчательными значениями или значениями,
			заданными вторым параметром. 
			
			Старое содержимое при этом теряется.
	 
			Часто массив valarray становится после выполнения операции resize() тем, 
			что создается как 'пустой вектор'. 
		*/
		void 			resize	(size_t sz, T c = T());
	};	
	/*
		Если size() == 0, то возвраты sum(), min() и max() не определены.
		
		Eсли v — это массив типа valarray, 
		то его можно масштабировать следующим образом: v *= .2, или v /= 1.3. 
		
		То есть 'применение к вектору скаляра'
		означает его 'применение к каждому элементу вектора'. 
		
		Обычно легче оптимизировать операцию '*=', 
		чем комбинацию отдельных операций '*' и '='.

		----------------------------------------------------------------------------------------
		22.4.4. Внешние операции и функции
		
		Обычные бинарные операции и математические функции определены для массивов valarray.
		
		Бинарные операции определяются для двух операндов valarray, 
		а также для valarray и его скалярного типа.
	*/
	template<class T> valarray<T> operator* 	(const valarray<T>&, const valarray<T>&) ; 
	template<class T> valarray<T> operator* 	(const valarray<T>&, const T&) ;
	template<class T> valarray<T> operator* 	(const T&, const valarray<T>&) ;
	//	аналогично: /, %, +, -, ^, &, |, <<, >>, &&, ||, ==, |=<, >, <=, >=, atan2(), иpow()
	template<class T> valarray<T> abs 		(const valarray<T>&);
	// аналогично: acos, asin, atan, cos, cosh, exp, log, loglO, sin, sinh,s qrt, tan и tanh

//	-----------------------------------------------------------------------------------------------------
	//	----------------------------------------------------------------------------------------------------
}	//	End of namespace SEH
/*
	****************************************************************************************************
*/
#endif 	// INCLUDE_GUARDS_NUMERICS_HPP
