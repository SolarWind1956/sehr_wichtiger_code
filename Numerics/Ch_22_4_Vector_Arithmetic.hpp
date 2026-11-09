	//		#pragma once -> SF.8: Use #include guards for all header files
#ifndef INCLUDE_GUARDS_ERRORS_CH_22_4_VECTOR_ARITHMETIC_HPP
#define INCLUDE_GUARDS_ERRORS_CH_22_4_VECTOR_ARITHMETIC_HPP
/*
	Ch_22_4_Vector_Arithmetic.hpp
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
#include <valarray>

#include <random>
#include <string_view>
#include <sstream>
#include <stdexcept>

#include "Ch_22_4_Vector_Arithmetic_cont.hpp"
#include "..\type_name_as_str.h"
/*
	***************************************************************************************************
	22.4. Векторная арифметика
	
	Cтандартная библиотека языка C++ 
	также предоставляет специальный векторный класс — std::valarray — 
	специально разработанный для быстрых численных операций над векторами.
	
	Глядя на предоставляемые классом std::valarray средства, 
	нужно помнить, что они представляют собой довольно низкоуровневые строительные блоки 
	для высокопроизводительных вычислений. 
	
	В итоге, главным критерием проектирования была не простота их использования, 
	а возможность эффективного применения на высокопроизводительных компьютерах 
	в условиях агрессивной оптимизации. 
	
	Если цель — это гибкость и универсальность, 
	а вовсе не особая эффективность, 
	то лучше строить вычисления на основе контейнеров, 
	и не пытаться втиснуться в жесткие рамки простого, 
	эффективного и намеренно традиционного вектора std::valarray.
	
	Кто-нибудь может сказать, что std::valarray следовало бы назвать vector, 
	поскольку он близок традиционному математическому вектору, 
	a std::vector следовало бы назвать array, так как это скорее массив в обычном смысле. 
	
	Но терминология развивалась в ином направлении: 
	std::valarray — это специальный вектор, оптимизированный для серьезных математических вычислений; 
	vector — это гибкий контейнер для хранения и манипулирования объектами различных типов, 
	а массивами (arrays) принято называть низкоуровневые встроенные типы данных.
	
	Тип std::valarray поддержан четырьмя подтипами, 
	специфицирующими подмножества std::valarray:
	
	■ 	std::slice и std::slice_array, std::gslice и std::gslice_array представляют понятие 'срезов',
 
	■ 	std::mask_array специфицирует подмножество путем маркировки элементов,
	
	■ 	std::indirect_array содержит индексы необходимых элементов.

	https://runebook.dev/ru/docs/cpp/-index-?q=valarray
	--------------------------------------------------------------------------------------------------------
	22.4.1. Конструкторы класса valarray
	
	Сам класс std::valarray и его вспомогательные средства 
	определены в пространстве имен std и представлены в заголовочном файле <valarray>;
	
	https://runebook.dev/ru/docs/cpp/numeric/valarray/valarray
	https://runebook.dev/ru/docs/cpp/header/valarray
*/	
namespace SEH 
{
//	-----------------------------------------------------------------------------------------------------
template		<class T> 
class valarray
{
 // внутреннее представление
 public:
	typedef T 	value_type;
	//	1) 	Конструктор по умолчанию. Создает пустой числовой массив.
	valarray();									//	valarray c size() == 0				
	//	2) 	Создает числовой массив из копий count элементов value-initialized .
	explicit valarray(std::size_t 			count);	//	count элементов co значением T{}
	//	3) 	Создает числовой массив с копиями count val .
	valarray(const T& val, std::size_t count);			//	count элементов co значением 'val'
	//	4) 	Создает числовой массив с count-копиями значений из массива, на который указывает vals. 
	//		Если этот массив содержит меньше значений count, поведение не определено.
	valarray(const T* vals, std::size_t count);		//	count элементов co значениями p[0], p[1], ...
	//	5) Копировать конструктор. Создает числовой массив с копией содержимого other.
	valarray(const valarray& 					other);
	//	6) Переместить конструктор. Создает контейнер с содержимым other, используя семантику перемещения.
	valarray(valarray&& 						other) noexcept;
	//	7-10) Converting constructor . Преобразуйте соответствующую структуру данных в valarray .
	valarray(const std::slice_array<T>& 		sa);	//	(7)	
	valarray(const std::gslice_array<T>& 		gsa);	//	(8)	
	valarray(const std::mask_array<T>& 		ma);	//	(9)	
	valarray(const std::indirect_array<T>& 	ia);	//	(10)	
	//	11) Создает числовой массив с содержимым списка инициализаторов 'il'.
	valarray(std::initializer_list<T> 		il);	//	Создает новый числовой массив из различных источников.
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
	/*
		Индексация целыми числами вполне традиционна,
		и при этом никакого контроля 
		выхода за допустимые границы индексов 
		не выполняется.
	*/
	const T&              	operator[]( std::size_t 							pos ) 			const;
	T&                    	operator[]( std::size_t 							pos );
	
	std::valarray<T>       	operator[]	( std::slice 						slice_arr ) 		const;
	std::slice_array<T>    	operator[]	( std::slice 						slice_arr );
	std::valarray<T>       	operator[]	( const std::gslice& 				gslice_arr ) 	const;
	std::gslice_array<T>   	operator[]	( const std::gslice& 				gslice_arr );
	std::valarray<T>       	operator[]	( const std::valarray<bool>& 			bool_arr ) 		const;
	std::mask_array<T>     	operator[]	( const std::valarray<bool>& 			bool_arr );
	std::valarray<T>       	operator[]	( const std::valarray<std::size_t>& 	ind_arr ) 		const;
	std::indirect_array<T> 	operator[]	( const std::valarray<std::size_t>& 	ind_arr );
	/*
		Parameters
		pos			-	позиция возвращаемого элемента
		slicearr	-	slice элементов для возврата
		gslicearr	-	gslice элементов для возврата
		boolarr		-	маска элементов для возврата
		indarr		-	индексы возвращаемых элементов
	*/
	//	std::valarray<T>::operator=
	/*
		Подходящий набор операций присваивания 
		делает ненужным 
		перед выполнением присваивания 
		преобразование вспомогательных типов, 
		таких как slice_array, в тип valarray.
	*/
	valarray<T>& operator=( const valarray<T>& 			other );
	valarray<T>& operator=( valarray<T>&& 				other ) noexcept;
	valarray<T>& operator=( const T& 						val );
	valarray<T>& operator=( const std::slice_array<T>& 	other );
	valarray<T>& operator=( const std::gslice_array<T>& 	other );
	valarray<T>& operator=( const std::mask_array<T>& 		other );
	valarray<T>& operator=( const std::indirect_array<T>& 	other );
	valarray<T>& operator=( std::initializer_list<T> 		il );
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
/*
	Все эти операции применяются ко всем элементам операндов (операнда) способом, 
	применяемым в обычных математических выражениях. 
	
	Естественно, операция применима лишь в том случае, когда она имеет смысл для типа аргумента шаблона. 
	
	В противном случае компилятор выдаст ошибку в момент конкретизации шаблона.
	
	Там, где результат имеет тип valarray, длина получающегося массива та же, что и у операндов типа valarray. 
	
	В случае разной длины операндов valarray результат бинарных операций не определен.
	
	Довольно любопытно, но для valarray не предоставляется никаких операций ввода/вывода; 
	<< и >> являются операциями сдвига. 
	
	В то же время, можно легко определить версии операций << и >> для ввода/вывода.
	
	Pассматриваемые операции возвращают новый valarray, а не модифицируют свои операнды. 
	
	Это может дорого стоить, но может и не стоить, 
	если для реализации valarray применяется агрессивная оптимизация.
	
	Все эти операции и математические функции над массивами valarray 
	можно применять и к slice_array, gslice_array, mask_array, indirect_array
	и к комбинациям этих типов. 
	
	В то же время, реализации могут преобразовывать эти типы в valarray перед выполнением операций.
*/
/*
	***********************************************************************************************
	22.4.5. Срезы
 
	'Срез' (slice) 	— 	это абстракция, которая позволяет эффективно работать 
							с векторами как с матрицами произвольной размерности. 
							
	Это ключевое понятие для векторов языка Fortran и библиотеки BLAS (Basic Linear Algebra Subprograms), 
	являющейся базисом для многих численных расчетов. 
	
	В своей основе срез — это каждый n-ый элемент некоторой части valarray.

	***********		class slice		***********************************************************
	https://runebook.dev/ru/docs/cpp/numeric/valarray/slice
	
	std::slice — это класс "селектора", 
	который "идентифицирует" 'подмножество' ('срез') std::valarray, 
	аналогичное срезу BLAS 
	(Basic Linear Algebra Subprograms - Базовые подпрогораммы линейной алгебры).
	https://en.wikipedia.org/wiki/Basic_Linear_Algebra_Subprograms	
	https://ru.wikipedia.org/wiki/Basic_Linear_Algebra_Subprograms
	
	Объект типа std::slice содержит три целочисленных значения: 
	
	-	начальный индекс, 
	-	шаг (stride)
	и 
	-	общее количество значений в подмножестве. 
	
	Объекты типа std::slice 
	можно использовать в качестве индексов 
	с помощью оператора [].
*/
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
	/*
		Проверяет, равны ли параметры lhs и rhs — старт, размер и шаг соответственно.
		
		Эта функция не видна обычным unqualified или qualified lookup 
		и может быть найдена argument-dependent lookup только тогда, 
		когда std::slice является 'связанным классом аргументов'.
	*/
	
	/*
		*****************************************************************************************
		Еще можно рассматривать срезы как необычный вид итераторов, 
		ведь они позволяют задать последовательности индексов для valarray. 
		
		На этой основе можно построить настоящий итератор:
	*/
	template	<class T> 
	class 	Slice_iter
	{
		valarray<T>* 	v;
		slice 			slc;
		size_t 			curr; 			// 	индекс текущего элемента
		
		T& 				ref 			(size_t i) 		const 		{	
			return (*v)[slc.start() + i * slc.stride()]; 
		}
	 public:
		Slice_iter 	(valarray <T>* vv, slice ss) 
					: 	v{vv} 
					, 	slc{ss} 
					, 	curr{0} 
					{}
		Slice_iter 		end 			() 				const {
			Slice_iter 	t 	= 	*this;
			t.curr 			= 	slc.size(); 			// 	индекс эл-та, следующего за последним
			return 		t;
		}
		Slice_iter& 		operator++ 	() 		{
			curr++; 
			return 	*this; 
		}
		Slice_iter 		operator++ 	(int) 	{
			Slice_iter 	t	= 	*this; 
			curr++; 
			return 		t; 
		}
		T& 				operator[] 	(size_t i) 	{	// 	индексация в стиле С
			return 	ref(i); 
		} 
		T& 				operator () 	(size_t i) 	{	// 	индексация в стиле Fortran
			return 	ref(i); 
		} 
		T& 				operator* 	() 			{
			return ref(curr); 						//	текущий элемент
		} 
		
		template	<class U> 	friend bool 		operator== 	(const Slice_iter<U>& p, const Slice_iter<U>& q);
		template	<class U> 	friend bool 		operator!= 	(const Slice_iter<U>& p, const Slice_iter<U>& q);
		template	<class U> 	friend bool 		operator< 	(const Slice_iter<U>& p, const Slice_iter<U>& q);

	};
	/*
		Поскольку срез имеет размер, мы даже можем обеспечить проверку корректности диапазона элементов. 
		
		Я воспользовался slice::size(), чтобы ввести операцию end(), 
		предоставляющую итератор, настроенный на элемент, следующий за последним элементом среза.
		
		Поскольку срез может описывать либо строку, либо столбец, 
		Slice_iter позволяет проходить valarray как по строкам, 
		так и по столбцам.
 
		Сравнения можно определить следующим образом:
	*/
	template	<class T> bool 	operator	==	(const Slice_iter<T>& p, const Slice_iter<T>& q)	{
		return 		p.curr 			== 	q.curr 
				&& 	p.slc.stride() 	== 	q.slc.stride() 
				&& 	p.slc.start() 	==	q.slc.start();
	}
 
	template	<class T> bool 	operator	!= 	(const Slice_iter<T>& p, const Slice_iter<T>& q)	{
		return 		!(p == q);
	}
	
	template	<class T> bool 	operator	< 	(const Slice_iter<T>& p, const Slice_iter<T>& q)	{
		 return 		p.curr			<	q.curr 
				&& 	p.slc.stride() 	==	q.slc.stride() 
				&& 	p.slc.start() 	==	q.slc.start();
	}
	//	----------------------------------------------------------------------------------------------------
}	//	End of namespace SEH
/*
	****************************************************************************************************
*/
#endif 	// INCLUDE_GUARDS_ERRORS_CH_22_4_VECTOR_ARITHMETIC_HPP
