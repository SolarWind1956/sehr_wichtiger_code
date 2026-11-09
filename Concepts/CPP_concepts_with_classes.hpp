#pragma once
/*
	CPP_concepts_with_classes.hpp
*/
#include	<iostream> 
using std::cout; using std::endl; using std::boolalpha;
#include <limits>
using std::string;
using std::numeric_limits;
#include <cassert>
#include <cstring>
#include <concepts>
#include <iterator>
#include <optional>

#include "..\type_name_as_str.h"
/*	
	***********************************************************************************************
	Концепции C++ с классами
	Шандор Дарго 24 февраля 2021 г.
	https://www.sandordargo.com/blog/2021/02/24/cpp-concepts-with-classes
	--------------------------------------------------------------------------------------------
	В прошлый раз мы обсуждали, как использовать концепции с функциями , 
	а на этот раз мы увидим, как использовать концепции с классами. 
	
	Cуществует четыре способа использования концепций с функциями:

	1	предложения-requires​ (the requires clause);
	2	завершающее requires-предложение (the trailing requires clause);
	3	ограниченные параметры шаблона (constrained template parameters);
	4	шаблоны сокращенных функций (abbreviated function templates).

	С классами у нас меньше вариантов. 
	
	Завершающее предложение не имело бы особого смысла requires, 
	поскольку нет сигнатуры функции, за которой оно могло бы следовать…

	И сокращенные шаблоны функций тоже не подойдут.
*/
template <typename T>
concept NumberConcept = 
				std::integral<T> || std::floating_point<T>;
				
template <typename T>
class WrappedNumber {
public:
	WrappedNumber(NumberConcept auto num) : m_num(num) {
		cout << "-------------- I'm COTOR " << __func__ << "(NumberConcept auto num)" << endl;
		cout << "--- Template Parameter's Type " << type_name_as_str<T>() << endl;
		cout << "--- Template NumberConcept auto num's Type " << type_name_as_str<decltype(num)>() << endl;
	}
private:
	//NumberConcept auto m_num; 				// error: non-static data member declared with placeholder
	T	m_num{};
};
/*
	Мы не можем 'объявлять' элементы данных с помощью 'auto', 
	это запрещено стандартом .

	Если мы удалим auto, у нас появится другое сообщение об ошибке, 
	в котором будет сказано, 
	что мы должны использовать auto (или decltype(auto)) 
	после концепта NumberConcept.

	предложения-requires​ (the requires clause);
	ограниченные параметры шаблона (constrained template parameters);
	
	Для наших примеров мы будем использовать ту же неполную NumberConcept-концепцию, 
	которую использовали в прошлый раз.

template <typename T>
concept Number = std::integral<T> || std::floating_point<T>;

	-------------------------------------------------------------------------------------------------------
	Предложения-requires​ (the requires clause);

	Мы можем использовать это requires-предложение для определения ограничений класса шаблона. 
	
	Все, что нужно сделать, — это написать класс шаблона, 
	и, после списка параметров шаблона, 
	поместить предложение require со всеми ограничениями, 
	которые мы хотели бы определить.

template <typename T>
concept Number = std::integral<T> || std::floating_point<T>;
*/
template <typename T>
	requires NumberConcept<T>
class WrappedNumberAgain {
public:
	WrappedNumberAgain(T num) : m_num(num) {}
private:
	T  m_num;
};
void test_6() {
	cout << "-------------- I'm void " << __func__ << "()" << endl;
	
	WrappedNumber<double>	wrappedDblNumber(3.14);
	WrappedNumber<int>		wrappedIntNumber(314);
	// НЕ КАТИТ - КОНЦЕПТ НЕ ПРОПУСКАЕТ КОМПИЛЯЦИЮ. WrappedNumber<int>		wrappedCharNumber("Hello");
	// НЕ КАТИТ - WrappedNumber 			wn{42};
	WrappedNumber<int> 		wn{42};
	// КАТИТ - другой синтаксис у конструктора
	WrappedNumberAgain 		wnAgain{42};
	// WrappedNumber ws{"a string"}; 
	// template constraint failure for 'template<class T>  requires  Number<T> class WrappedNumber'
}
/*
	Как видно на примере, за исключением дополнительной строки requires,
	это то же самое, что и шаблонный класс.

	Если вы используете имя типа шаблона 'T' в нескольких местах, 
	заменяющие значения должны быть одного и того же типа. 
	
	Если вы берете в конструкторе два ограниченных 'T' , 
	они должны быть одного типа. 
	
	Вы не сможете вызывать с помощью int и с float, 
	несмотря на то, что они оба удовлетворяют концепции NumberConcept.

	Если это нужно, 
	для каждого потенциально различного использования параметра шаблона 
	потребуется другое объявление в списке параметров шаблона, 
	а также в ограничениях:

template <typename T>
concept Number = std::integral<T> || std::floating_point<T>;
*/
template <typename T, typename U>
	requires NumberConcept<T> && NumberConcept<U>
class WrappedTwoNumber {
public:
	WrappedTwoNumber(T num, U anotherNum) : m_num(num), m_anotherNum(anotherNum) 	{
		cout << "-------------- I'm COTOR " << __func__ << "(T num, U anotherNum)" << endl;
		cout << "--- Template T-Parameter's Type " << type_name_as_str<T>() << endl;
		cout << "--- Template U-Parameter's Type " << type_name_as_str<U>() << endl;
	}
private:
	T  m_num;
	U  m_anotherNum;
};
void test_7() {
	cout << "-------------- I'm void " << __func__ << "()" << endl;
    WrappedTwoNumber wn{42, 4.2f};
}
/*
	Этот приведенный выше пример также показывает, 
	что мы можем использовать 'составные выражения' в качестве 'ограничений'. 
	
	Это невозможно при другом способе написания ограниченных классов шаблонов.

	-------------------------------------------------------------------------------------------------------
	Ограниченные параметры шаблона
	
	Благодаря ограниченным параметрам шаблона 
	использовать концепции еще проще. 
	
	В списке параметров шаблона вместо ключевого слова typename 
	вы можете просто указать концепцию, которую хотите использовать.

template <typename T>
concept Number = std::integral<T> || std::floating_point<T>;


template <Number T>
class WrappedNumber {
public:
	WrappedNumber(T num) : m_num(num) {}
private:
	T  m_num;
};

	В этом примере вы можете увидеть, как мы ограничили 'T'-реализацию Number-концепции.

	Явным преимуществом ограниченных параметров шаблона является то, 
	что они просты в использовании, 
	легко читаются 
	и не требуют лишнего многословия.

	Обратной стороной является то, 
	что вы не можете использовать составные выражения в качестве ограничений.

	А с requires-предложением вы можете написать что-то вроде этого:

template <typename T>
	requires std::integral<T> || std::floating_point<T>
class WrappedNumber {
  // ...
};

	При ограниченных параметрах шаблона подобное было бы невозможно. 
	
	Если вам нужно использовать какие-то сложные ограничения, 
	вы должны выделить их в отдельную концепцию.

	Кроме того, это похоже на requires-предложение: 
	если у вас есть несколько параметров, 
	которым необходимо удовлетворить Number, 
	но они могут быть разными, 
	вы должны использовать несколько параметров шаблона:

template <Number T, Number U>
class WrappedNumber {
public:
	WrappedNumber(T num, U anotherNum) : m_num(num), m_anotherNum(anotherNum) {}
private:
	T  m_num;
	U  m_anotherNum;
};
	************************************************************************************************************
	Заключение

	Сегодня мы обнаружили два способа использования концепций с классами. 
	И с помощью предложения-requires , и с ограниченными параметрами шаблона 
	у нас есть простой и понятный способ использовать наши концепции для ограничения типов, 
	которые могут принимать наши классы шаблонов .

	В первом случае мы даже можем определить некоторые сложные требования 
	без необходимости выделения их в отдельные концепции, 
	а во втором мы можем использовать только одну концепцию 
	для каждого параметра шаблона, 
	но, наоборот, это очень кратко. 
	
	Выбор за вами, исходя из ваших потребностей.

*/	
//	-----------------------------------------------------------------------------------------------------------