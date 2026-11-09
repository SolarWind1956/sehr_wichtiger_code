#pragma once
/*
	about_concepts.hpp
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
	Первое впечатление от концептов
	https://habr.com/ru/articles/495396/
	***********************************************************************************************

	Концепты (или концепции) — очень интересная и полезная фича, которой давно не хватало.

	По сути это 'типизация' для 'аргументов шаблонов'.

	Основная проблема шаблонов до С++20 — 
	в них в качестве аргументов можно было подставить все что угодно, 
	в том числе то, на что они совершенно не рассчитаны. 
	
	То есть система шаблонов была совершенно нетипизирована. 
	
	В результате, при передаче в шаблон неверного параметра (аргумента шаблона) 
	возникали невероятно длинные и совершенно нечитаемые сообщения об ошибках. 
	
	Концепты призваны исправить это недоразумение. 
	
	Они добавляют в шаблоны систему типизации, причем весьма мощную. 
	
	'Метапрограммирование' на шаблонах именно "открыли", 
	а не заложили при проектировании языка. 
	
	А дальше, при разработке следующих версий языка, 
	были вынуждены подстраиваться под это «открытие», 
	так как в мире было написано очень много кода. 
	
	Концепты же — принципиально новая возможность. 
	
	И, как мне кажется, в их реализации уже присутствует некоторая непрозрачность. 
	
	Возможно, это следствие необходимости учесть огромный объем унаследованных возможностей? 
	
	***********************************************************************************************
	Общие сведения
	***********************************************************************************************
	
	Концепт — новая языковая сущность на основе синтаксиса шаблонов. 
	
	У концепта есть: 
	
		-	имя, 
		
		-	параметры 
		
		и 
		
		-	тело 	— 	предикат, 
						возвращающий константное 
						(т.е. вычисляемое на этапе компиляции) 
						логическое значение, 
						зависящее от параметров концепта. 
	Вот так:
*/
template<int I> 				//	I 				-	параметр концепта
concept Even = 					//	Even 			-	имя концепта
				I % 2 == 0;  	//	I % 2 == 0; 	-	предикат концепта

template<typename T>
concept FourByte = sizeof(T) == 4;

//	Технически, концепты очень похожи на шаблонные constexpr-выражения типа bool:

template<int I>
constexpr bool EvenX = I % 2 == 0; 

template<typename T>
constexpr bool FourByteX = sizeof(T) == 4;

//	Можно даже использовать концепты в обычных выражениях:
void	test_1(){
	cout << "-------------- I'm void " << __func__ << endl;
	bool b1 = Even<2>; 
	cout << "bool b1 = Even<2>; == " << boolalpha << b1 << endl;
};
/*
	****************************************************************************************
	Использование
	****************************************************************************************
	
	Основная идея концептов — 	их можно использовать 
								вместо ключевых слов 'typename' или 'class' в шаблонах. 
								
	Как 'метатипы' («типы для типов»). 
	
	Тем самым в шаблоны привносится 'статическая типизация'.
*/
template<FourByte T>		// Тип 'T' должен иметь размер 4 байта, иначе, код не будет скомпилирован 
void foo_2(T const & t) 
{
	cout << "-------------- I'm template<FourByte T> void " << __func__ << endl;
	cout << "template<typename T> concept FourByte = sizeof(T) == 4; sizeof(T) == " << sizeof(T) << endl;
}
/*
	Теперь, если мы используем в качестве шаблонного параметра int (размер 4 байта), 
	то код в подавляющем большинстве случаев скомпилируется; 
	а если double (размер больше, чем 4 байта), 
	то будет выдано краткое и понятное сообщение об ошибке. 
*/
void	test_2(){
	cout << "-------------- I'm void " << __func__ << endl;
	char	char_val	=	'H';
	// !!!!!!! НЕ КАТИТ! - note: because 'char' does not satisfy 'FourByte'
	// foo_2(char_val);
	double	dbl_val		=	3.14;
	// !!!!!!! НЕ КАТИТ! - note: because 'double' does not satisfy 'FourByte'
	// foo_2(dbl_val);		
	int		int_val		=	314;
	foo_2(int_val);		
};
/*	
	Простая и понятная типизация шаблонов, пока все ок.

	***************************************************************************************
	requires
	***************************************************************************************
	
	Это новое «контекстное» ключевое слово С++20, 
	имеющее двойное назначение: 
	
	-	requires clause 
	
	и 
	
	-	requires expression. 
	
	Как будет показано далее, эта странная экономия на ключевых словах приводит к некоторой путанице.

	--------------------------------------------------------------------------------------------------
	requires expression
	--------------------------------------------------------------------------------------------------

	Сначала рассмотрим 'requires expression'. 
	
	Идея весьма неплоха: 	это слово имеет блок в {фигурных скобках}, 
							код внутри которого оценивается на компилируемость. 
							
	Правда, код там должен быть написан не на С++, 
	а на специальном языке, близком к С++, 
	но имеющем свои особенности 
	(это первая странность, вполне можно было сделать и просто С++ код).

	Если код корректный 	— 	'requires expression' возвращает 'true', 
								иначе 'false'. 
	
	Сам код в {фигурных скобках}  
	не попадает на кодогенерацию никогда, 
	примерно как выражения в sizeof или decltype.

	К сожалению, слово 'контекстное' 
	и работает только внутри шаблонов, 
	то есть вне шаблона вот такое не скомпилируется:
	
	!!!!!!!!
	// Однако, скомпилировалось
*/
	bool b_2 = requires { 3 >> 1; };			// Однако, скомпилировалось
	// !!!!!!!! НЕ КАТИТ!
	// bool b_2_2 = requires { 3.14 >> 1; };	// Компилятор не пропускает код в {фигурных скобках},
												// если он с ошибкой 
	// error: invalid operands to binary expression ('double' and 'int' to binary 'operator>>' )

//	И в шаблоне:
template<typename T>
constexpr bool Shiftable = requires(T i) { i >> 1; };

// И будет работать:
void	test_3(){
	cout << "-------------- I'm void " << __func__ << endl;
	bool b1 = Shiftable<int>; 		// true
	cout << "bool b1 = Shiftable<int>; == " << boolalpha << b1 << endl;
	bool b2 = Shiftable<double>; 	// false
	cout << "bool b1 = Shiftable<double>; == " << boolalpha << b2 << endl;
};
/*
	Основное применение requires expression 	— 	"создание концептов". 
	
	Например, можно 
	проверить наличие полей и методов в типе, 
	проверить, поддерживает ли тип какую-то операцию? 
	
	Весьма востребованный кейс.
*/
template <typename T>
concept comparable = 
					requires(T a){		// compound requirement		-	сложное требование
							{a < a} -> std::convertible_to<bool>;
					};
// 	Или:					
template <typename T>
concept Machine = 
					requires(T 	m) {  	// любая переменная 'm' типа 'T', удовлетворяющего концепту Machine,
							m.start();  // должна иметь метод 'm.start()' 
							m.stop();   // и метод 'm.stop()'
							m.init();   // и метод 'm.init()'
					};  
template <Machine T>
class	TestClass_3_1{
public:
	T	class_object_mem;
public:
	TestClass_3_1() {
		cout << "-------------- I'm COTOR of " << __func__ << endl;
		class_object_mem.init();
	}	
};

template <typename T>					
class	TestClass_3{
public:	
	void init(){
		cout << "-------------- I'm " << __func__ << endl;
	};
	void start(){
		cout << "-------------- I'm " << __func__ << endl;
	};
	void stop(){
		cout << "-------------- I'm " << __func__ << endl;
	};
};
void	test_4(){
	cout << "-------------- I'm void " << __func__ << endl;
	
	TestClass_3_1<TestClass_3<int>>		test_class;
	test_class.class_object_mem.start();
	test_class.class_object_mem.stop();
};	
/*
	Кстати, все переменные, 
	которые могут потребоваться в тестируемом коде requires expression
	(не только параметры шаблона), 
	нужно объявлять в (круглых скобках) requires expression. 
	
	Просто так объявить переменную нельзя.

	-----------------------------------------------------------------------------------------------------
	Проверка типов внутри requires expression

	Здесь начинаются отличия requires-кода от стандартного С++. 
	
	Для проверки 'возвращаемых типов' 
	используется специальный синтаксис: 
	
	объект берется в фигурные скобки, 
	ставится стрелка ->, 
	и после нее пишется библиотечный std::-концепт, 
	которому должен удовлетворять тип. 
	
	Причем использование непосредственно типов не допускается,
	только с использованием библиотечного std::-концепта:

	{a < a} -> std::convertible_to<bool>;	//	ОК!
	
	{a < a} -> bool;						//	НЕ КОМПИЛИРУЕСТЯ!!!
	
	Проверяем, что 'возврат функции' "может быть сконвертирован к int":
*/
template<typename T>
concept GeoObj =
				requires(T v, int i) {
						{ v.f(i) } -> std::convertible_to<int>;	//	"может быть сконвертирован к int"
				};  
//	Проверяем, что 'возврат функции' "в точности равен int":
template<typename T>
concept CheckGeoObj =
						requires(T v, int i) {
							{ v.f(i) } -> std::same_as<int>; 	//	"в точности равен int"
						};  
/*
	std::same_as и std::convertible_to это концепты из стандартной библиотеки).

	Если не заключить выражение, тип которого проверяется, в {фигурные скобки}, 
	компилятор не поймет, что от него хотят, 
	и интерпретирует всю строку как единое выражение, 
	которое нужно проверить на компилируемость.

	--------------------------------------------------------------------------------------------
	Вложенные requires-выражения 	-	requires внутри requires

	'Ключевое слово' 'requires' имеет специальное значение 
	внутри 'выражений' 'requires'. 
	
	'Вложенные' requires-выражения 
	могут быть 
	и в {фигурных скобках} 
	и без {фигурных скобок}.
	
	'Вложенные' requires-выражения без {фигурных скобок} 
	проверяются уже 
	'не на компилируемость', 
	а 'на равенство' 'true' или 'false'. 
	
	Если такое выражение окажется 'false', 
	то и компилируемость 'объемлющего выражения' 
	немедленно окажется 'false' 
	(и дальнейший анализ компилируемости прерывается). 
	
	Общий вид:

	requires { 
		expression;         // expression is valid
		requires predicate; // predicate is true
	};

	В качестве предиката 
	могут использоваться, например, 
	ранее определенные концепты или свойства типов (type traits). 
	
	Пример:
*/
template<typename Iter>
concept GeoObj_requires_in_requires =
				requires(Iter it) {
					// проверяем код на валидность (что для типа Iter допустимы операции * и ++)
					*it++;
					// проверяем на истинность - с концептом
					requires std::convertible_to<decltype(*it++), typename Iter::value_type>;
					// проверяем на истинность - с трейтом
					requires std::is_convertible_v<decltype(*it++), typename Iter::value_type>;
				};
				// std::convertible_to	---	std::is_convertible_v
/*
	При этом допускаются и 'вложенные requires-выражения' с кодом в {фигурных скобках}, 
	который проверяется именно 'на валидность'. 
	
	Однако если записать просто одно requires-выражение внутри другого, 
	то вложенное выражение (всё в целом, включая вложенное ключевое слово requires) 
	будет просто проверено 'на валидность':
*/
template<typename T>
concept GeoObj_requires_in_requires_2 = 
	requires (T v) { 
	  requires 
		requires (typename T::value_type x) { ++x; }; 	// это ВЫРАЖЕНИЕ а не предикат, 
														// оно просто проверяется на валидность!
	};  

// Поэтому возникла странная форма с двойным requires:
template<typename T>
concept GeoObj_requires_in_requires_3 =
	requires (T v) { 
		requires 
			requires (typename T::value_type x) { ++x; }; // вот теперь на валидность будет проверено "++x"
	};  
//	Вот такая вот забавная escape-последовательность из «requires».
//	Кстати, еще одно сочетание двух requires — 
// 	на этот раз requires-clause (см. далее) и requires-expression:
template <typename T>
  	requires 
		requires(T x, T y) { bool(x < y); }
bool equivalent(T const& x, T const& y)	{
  return !(x < y) && !(y < x);
};
/*
	----------------------------------------------------------------------------------------------------
	Декларации requires 	-	requires clause

	Теперь перейдем к еще одному использованию слова requires — 
	для 'декларации' "ограничений шаблонного типа". 
	
	Это альтернатива использованию 'имен концептов' вместо 'typename' или 'class'. 
	
	В следующем примере все три способа эквивалентны:
*/
template<typename T> 					//	T 				-	параметр концепта
concept Sortable = 						//	Sortable 		-	имя концепта
					std::sortable<T>;	//					-	тело концепта
// декларация require
template<typename Container>
	requires Sortable<Container>
void sort(Container& container)
{};

// хвостовая декларация require (только для функций)
template<typename Container>
void sort(Container& container) requires Sortable<Container>
{};

// имя концепта вместо typename
template<Sortable Container>
void sort(Container& container)
{};  
//	В декларации requires могут использоваться 
//	несколько предикатов, объединенных логическими операторами.
template <typename T>
	requires 		std::is_standard_layout_v<T> 
				&& 	std::is_trivial_v<T>
void fun(T v)
{
	cout << "-------------- I'm void " << __func__ << "(T v); v == " << v << endl;
}; 
 
void test_5()	{
	cout << "-------------- I'm void " << __func__ << endl;
	std::string s;
 
	fun(1);  // ok
	//fun(s);  // compiler error
};
/*
	Однако, cтоит только инвертировать одно из условий, как возникнет ошибка компиляции:

	template <typename T>
		requires 		is_standard_layout_v<T> 
					&& 	!is_trivial_v<T>
	void fun(T v); 

	Вот такой пример тоже не будет компилироваться

	template <typename T>
		requires !is_trivial_v<T>
	void fun(T v);	

	Причина этого — неоднозначности, 
	возникающие при разборе некоторых выражений. 
	
	Например в таком шаблоне:

	template <typename T> 
			requires (bool)&T::operator 
		short unsigned 
	int foo();

	непонятно к чему отнести unsigned — 
	к оператору или к прототипу функции foo(). 
	
	Поэтому разработчиками было принято решение, 
	что 'без круглых скобок' 
	в качестве аргументов 'requires clause' 
	могут использоваться только очень ограниченный набор сущностей:
	
	-	литералы true или false, 
	
	-	имена полей типа bool вида value, value, T::value, ns::trait::value, 
	
	-	имена концептов вида 'concept' и 'requires expressions'. 
	
	Все остальное следует заключать в круглые скобки:

	template <typename T>
		requires (!is_trivial_v<T>)
	void fun(T v);

	-------------------------------------------------------------------------------------------------
	Теперь об особенностях предикатов в 'requires clause'

	Рассмотрим другой пример.
*/
template <typename T>
	requires std::is_trivial_v<typename T::value_type> 
void fun_requires_clause(T v)
{}; 
/*
	В этом примере в requires используется трейт, 
	зависящий от вложенного типа value_type. 
	
	Заранее неизвестно, есть ли такой вложенный тип у произвольного типа, 
	который можно передать в шаблон. 
	
	Если передать в такой шаблон например простой тип int, 
	будет ошибка компиляции, 
	однако если у нас есть две специализации шаблона — 
	то ошибки не будет; 
	просто будет выбрана другая специализация.
*/
template <typename T>
	requires std::is_trivial_v<typename T::value_type> 
void fun6(T v) 	{ 
	cout << "-------------- I'm void " << __func__ << "(T v); " << endl;
	cout << "1" << endl;
}; 

template <typename T>
void fun6(T v) { 
	cout << "-------------- I'm void " << __func__ << "(T v); " << endl;
	cout << "2" << endl;; 
} 
struct TRIVIAL_A 
{ 
	using value_type	=	int;
	int m; 
}; 
static_assert(std::is_trivial_v<TRIVIAL_A> == true);

void test_6()	{
	cout << "-------------- I'm void " << __func__ << endl;
	// Тут я кое-что изменил в тривиальном классе TRIVIAL_A добавил using value_type	=	int;
	// а в типе int, видимо такого члена нет
	fun6(TRIVIAL_A{});	// displays: "1"
	fun6(1);  			// displays: "2"
}
/*
	Таким образом, специализация отбрасывается 
	не только, 
	когда предикат 'require clause' возвращает 'false', 
	но и тогда, 
	когда он оказывается некорректным.

	Круглые скобки вокруг предиката являются важным напоминанием того, 
	что в 'requires clause' 
	инверсия предиката не является противоположностью самого предиката. 
	
	Так,

	requires is_trivial_v<typename T::value_type> 

	означает что «трейт корректый и возвращает true». 
	
	При этом

	!is_trivial_v<typename T::value_type> 

	означало бы «трейт корректный и возвращает false»
	
	Настоящая логическая инверсия первого предиката — 

	НЕ(«трейт корректый и возвращает true») == «трейт НЕкорректный или возвращает false» 
	
	достигается чуть более сложным образом — через явное определение концепта:

	template <typename T>
	concept 	value_type_valid_and_trivial = 
											std::is_trivial_v<typename T::value_type>; 
 
	template <typename T>
		requires (!value_type_valid_and_trivial<T>)
	void fun(T v); 

	*************************************************************************************************
	Конъюнкция и дизъюнкция

	Операторы логической конъюнкции и дизъюнкции выглядят как обычно, 
	но на самом деле работают немного иначе, чем в обычном С++.

	Рассмотрим два очень похожих фрагмента кода.

	--------------------------------------------------------------------------------------------------
	Первый — предикат без скобок:
*/
template <typename T, typename U>
	requires 	std::is_trivial_v<typename T::value_type>
			|| 	std::is_trivial_v<typename U::value_type>
void fun7(T v, U u)
{}; 
/*
	--------------------------------------------------------------------------------------------------
	Второй — со скобками:

template <typename T, typename U>
	requires 	(	std::is_trivial_v<typename T::value_type>
				|| 	std::is_trivial_v<typename U::value_type>
				)
void fun7(T v, U u)
{}; 
*/
/*
	!!!!!!!! Что-то не так!
	
	Разница только в скобках. 
	
	Но из-за этого во втором шаблоне не два ограничения, 
	объединенных «requires-дизъюнкцией», 
	а одно, 
	объединенное обычным 'логическим ИЛИ'.

	Эта разница проявляется следующим образом. 
	
	Рассмотрим код
*/
void test_7()	{
	cout << "-------------- I'm void " << __func__ << endl;

	std::optional<int> 	oi{};
	
	int 				i{};
	
	fun7(i, oi);
};	
/*
	Здесь шаблон инстанцируется типами int и std::optional.

	В первом случае тип int::value_type невалидный, 
	и первое ограничение тем самым не удовлетворяется.

	Но тип optional::value_type валидный, 
	второй трейт возвращает true, 
	а поскольку между ограничениями стоит оператор ИЛИ, 
	то весь предикат в целом удовлетворяется.

	Во втором случае это единое выражение, 
	содержащее невалидный тип, 
	из-за чего оно оказывается невалидно в целом и предикат не удовлетворяется. 
	
	Вот так простые скобки незаметно меняют смысл происходящего.

	**********************************************************************************************************
	В завершение

	Конечно здесь показаны далеко не все особенности концептов. 
	
	Идея с кодом, проверяемым на компилируемость — однозначно хорошая. 
	
	Это даже чем-то похоже на «квази-цитирование» в синтаксических макросах. 
	
	Неявное смешивание понятий 
	«истинно/ложно» 
	и 
	«компилируется/не компилируется» 
	в одну кучу, и как следствие приколы со скобочками — тоже ... 
*/
//	Из книги Александреску
class Widget
{
public:
	Widget() {
		cout << "-------------- I'm COTOR " << __func__ << "()" << endl;
	}
	Widget* Clone() {
		return this;
	}
};

template <class	T>
class SingleThreaded {
public:
	class	Lock {
	public:
		Lock(T&)
		{}
	};		// Точка с запятой обязательна для внутреннего 'вложенного' (nested class) класса	

	class NullPointerException : public std::exception
	{};
	static void Check(T* ptr) {
		if (!ptr)
			throw NullPointerException();
	}
};
template <class	T>
class EnforceNotNull {
public:
	class NullPointerException : public std::exception
	{};

	static void Check(T* ptr) {
		if (!ptr)
			throw NullPointerException();
	}
};
template <class T>
class DefaultSmartPtrStorage
{
public:
	typedef 	T* PointerType;
	typedef 	T& ReferenceType;

	DefaultSmartPtrStorage() : ptr_(nullptr)
	{}
protected:
	PointerType 	GetPointer() {
		return ptr_;
	}
	void 			SetPointer(PointerType ptr) {
		ptr_ = ptr;
	}
private:
	PointerType 	ptr_;
};

template	<	class T
	, template <class> class CheckPolicy
	, template <class> class ThreadingModel
	, template <class> class Storage = DefaultSmartPtrStorage
>
requires requires	{
	requires requires (CheckPolicy<T> m, T* ptr) { m.Check(ptr); };
	requires requires (ThreadingModel<T> m, T* ptr) { 
		m.Check(ptr); 
		// typename T::Lock;
	};
}
class SmartPtr_192
{
public:
	SmartPtr_192() {
		cout << "-------------- I'm COTOR " << __func__ << "()" << endl;
		cout << "-- Template T Parameter's Type " << type_name_as_str<T>() << endl;
		cout << "-- Template CheckPolicy Parameter's Type " << type_name_as_str<CheckPolicy<T>>() << endl;
		cout << "-- Template ThreadingModel Parameter's Type " << type_name_as_str<ThreadingModel<T>>() << endl;
		cout << "-- Template Storage Parameter's Type " << type_name_as_str<Storage<T>>() << endl;
	}
};
void test_strategy_polyce_5() {
	cout << "-------------- I'm void " << __func__ << "()" << endl;

	SmartPtr_192<Widget, EnforceNotNull, SingleThreaded>		smartPtr_1;
	SmartPtr_192<Widget, EnforceNotNull, SingleThreaded>		smartPtr_2;
}
//	-----------------------------------------------------------------------------------------------------------