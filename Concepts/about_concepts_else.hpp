#pragma once
/*
	about_concepts_else.hpp
*/
#include	<iostream>
using std::cout; using std::endl; using std::boolalpha;
#include <limits>
using std::numeric_limits;
#include <string>
using std::string;
#include <cassert>
#include <cstring>
#include <concepts>
#include <iterator>
#include <optional>
#include <vector>
#include <list>
#include <string>
#include <type_traits>
#include <memory>
#include "..\type_name_as_str.h"
/*	
	***********************************************************************************************
	Концепты: упрощаем реализацию классов STD Utility
	https://temofeev.ru/info/articles/kontsepty-uproshchaem-realizatsiyu-klassov-std-utility/
	***********************************************************************************************
	Появляющиеся в C++20 концепты — давно и широко обсуждаемая тема. 
	
	Несмотря на избыток материала, накопившегося за годы 
	(в т.ч. выступления экспертов мирового уровня), 
	среди прикладных программистов (не засыпающих ежедневно в обнимку со стандартом) 
	все еще остается неразбериха, 
	что же такое С++20-концепты, 
	и так ли они нам нужны, 
	если есть проверенный годами enable_if. 
	
	Частично виной тому то, как концепты эволюционировали за ~15 лет 
	(Concepts Full + Concept Map -> Concepts Lite), 
	а частично то, 
	что концепты получились непохожими на аналогичные средства в других языках 
	(Java/С# generic bounds, Rust traits, ...).
	****************************************************************************************************
	Поговорим о концептах. 
	
	По мнению Александреску, концепты — это скучно. 
	
	Что мы имеем в виду, когда говорим о концептах в C++20? 
	
	Эта фича обсуждалась как минимум с 2003 года, 
	и она за это время успела сильно эволюционировать. 
	Новая сущность под названием «концепты» 
	определяется ключевым словом 'concept'. 
	
	Это 'предикат' на 'шаблонных параметрах'. 
	
	Выглядит это примерно так:
*/
#include <utility>
template <typename T> 							// 	T 								- параметр концепта
concept NoThrowDefaultConstructible = 			// 	NoThrowDefaultConstructible		- имя концепта
								noexept(T{});	//									- тело концепта	
template <typename From, typename To>
concept Assignable = 
						std::is_assignable_v<From, To>;	//	https://en.cppreference.com/w/cpp/concepts
/*
	Я не просто так использовал фразу «на шаблонных параметрах», 
	а не «на типах», 
	потому что концепты можно определять 
	и на нетиповых шаблонных параметрах, 
	например, можно определить концепт для числа:
*/
template<int I> 
	concept Even = 
					I % 2 == 0;
/*
	Но гораздо больше смысла в том,
	чтобы "смешивать" 'типовые' и 'нетиповые' шаблонные параметры. 
	
	Назовём тип маленьким, 
	если его size и alignment не превышает заданных ограничений:
*/
template	<	typename 	T
			, 	size_t 		MaxSize
			, 	size_t 		MaxAlign
			>
concept SmallType = 
			sizeof(T) <= MaxSize && alignof(T) <= MaxAlign;
/*
	Наверное, пока неочевидно, 
	почему нужно городить в языке новую сущность, 
	и почему концепт — это не просто constexpr bool переменная.
	// почему `concept` нельзя определить таким образом?
	#define concept constexpr bool
	************************************************************************************************
	Как используются концепты
	Чтобы разобраться, давайте посмотрим, как используются концепты.
	Во-первых, так же, как и 'constexpr bool'-переменные, 
	их можно использовать везде, 
	где в compile-time нужно булевское выражение. 
	
	Например, 
	внутри static_assert 
	или 
	внутри noexcept-спецификаций:
*/
// bool expression evaluated in compile-time
// Концепт Assignable определен выше по коду.
// static_assert(Assignable<float, int>);
template<typename T>
using	NothrowDefaultConstructible	=	std::is_nothrow_default_constructible<T>;
template<typename T>
void test() noexcept(NothrowDefaultConstructible<T>::value) 
//void test() noexcept(std::is_nothrow_default_constructible_v<T>) 
{
  T t;
  //...
}
/*
	Во-вторых, концепты можно использовать 
	вместо ключевых слов 'typename' или 'class' 
	при определении шаблонных параметров. 
	
	Определим простой класс 'optional', 
	который будет просто хранить пару из булевского флажка initialized и значения. 
	
	Естественно, такой 'optional' применим только для 'тривиальных типов'. 
	
	Поэтому мы тут пишем 'Trivial',
	и при попытке проинстанцировать от чего-то нетривиального, 
	например, от std::string, у нас будет ошибка компиляции:
*/
template <typename T>
concept Trivial = 
					std::is_trivial_v<T>;	//	https://en.cppreference.com/w/cpp/concepts
// вместо type-parameter-key (class, typename)
template<Trivial T>
class simple_optional {
  T 		value;
  bool 		initialized = false;
  // ...
};
void test_1(){
	cout << "-------------- I'm " << __func__ << endl;
	simple_optional<int>			int_optional;
	//simple_optional<std::string>	string_optional;
};
/*
	Концепты можно применять "частично". 
	
	К примеру, реализуем свой класс 'AnyClass' со 'small buffer'-оптимизацией. 
	
	Определим структуру SB (small buffer) 
	с фиксированным Size и Alignment, 
	будем хранить union из SB и указателя в куче. 
	
	И теперь, если в конструктор приходит маленький тип, 
	то мы можем просто разместить его в SB. 
	
	Чтобы определить, что тип маленький, мы пишем, 
	что он удовлетворяет концепту Small. 
	
	Концепт Small принимает 3 шаблонных параметра: 
	два мы определили, 
	и у нас как бы получилась функция от одного шаблонного параметра:
*/
template	<	typename 	T
			, 	size_t 		MaxSize
			, 	size_t 		MaxAlign
			>
concept Small = 
			sizeof(T) <= MaxSize && alignof(T) <= MaxAlign;
/* 
	При использовании шаблона концепта в данном случае,
	у него уже два аргумента, а не три.
	
	Первый параметр (типовой) шаблона концепта 
	становится параметром шаблона функции,
	а шаблон концепта 
	но уже с двумя параметрами - 
	становится типом параметра шаблона функции.
	
	template<Small<SB::Size, SB::Alignment> T>	
	AnyClass(T const & t) 	{
		memcpy(&sb.storage, &t, sizeof(sb.storage));
	}
*/			
#include <memory.h>
// Частичное применение
class AnyClass {
public:	
	struct SB {
		static constexpr size_t Size 		= 4;
		static constexpr size_t Alignment 	= 16;
		std::aligned_storage_t<Size, Alignment> 	storage;
	};
	union {
		SB 		sb;
		void* 	handle;
	};
	// Конструктор 
	/* 
	template<Small<SB::Size, SB::Alignment> T>	
	AnyClass(T const & t) 	{
		memcpy(&sb.storage, &t, sizeof(sb.storage));
	}
	*/
	// Есть и более краткая запись. 
	AnyClass(Small<SB::Size, SB::Alignment> auto const & t)	{
		memcpy(&sb.storage, &t, sizeof(sb.storage));
	}
	/*
		Наверное, в любом месте, где мы пишем 'auto', 
		теперь можно писать 
		'перед ним' 
		'имя концепта',
		ранее определенного.
	*/
};
void test_2(){
	cout << "-------------- I'm " << __func__ << endl;
	AnyClass	any_class{21};
	cout << reinterpret_cast<int&>(any_class.sb.storage) << endl;
};
/*
	Определим функцию get_handle(), 
	которая возвращает для объекта некоторый 'handle'.
	Будем считать, 
	что 'маленькие' объекты 'сами для себя' являются 'handle', 
	а для 'больших' — 'указатель на них' является 'handle'. 
	
	Поскольку у нас две ветки if constexpr 
	обозначают выражения разных типов, 
	то нам удобно не указывать тип этой функции явно, 
	а попросить компилятор его вывести. 
	
	Но, написав там просто 'auto', 
	мы потеряем информацию о том, 
	что обозначаемое значение маленькое, 
	оно не превышает указатель:
*/
//	Terse syntax (ограничение на auto)
template<typename T>
concept LEPtr = 
				Small	<	T
						, 	sizeof(void *)
						, 	alignof(void *)
						>;
template<typename T>
auto get_handle(T& object) {
	cout << "-------------- I'm template<typename T> auto " << __func__ << "(T& object)" << endl;
	if constexpr (LEPtr<T>)
		return object;
	else
		return &object;
}
//	В C++20 можно будет перед ним написать, что это не просто auto, это ограниченное auto:
template<typename T>
LEPtr auto get_handle_c20(T &object) {
	cout << "-------------- I'm template<typename T> LEPtr auto " << __func__ << "(T& object)" << endl;
    if constexpr (LEPtr<T>)
        return object;
    else
        return &object;
}
void test_3(){
	cout << "-------------- I'm " << __func__ << endl;
	
	std::vector<std::string>  	big_vector(20, "Congratulations!");
	std::string					small_string{"C!"};
	int							int_variable{42};
	char						small_char_variable{'C'};
	
	cout << get_handle(int_variable);
	cout << get_handle_c20(small_char_variable);
	cout << get_handle_c20(big_vector)->at(0);
};
/*
	**********************************************************************************************
	Requires expression
	Requires-expression 	— 	это целое семейство expression'ов, 
								все они имеют тип bool и вычисляются в compile-time. 
								
	Их используют 
	для проверки утверждений 
	о выражениях и типах. 
	
	Requires-expression очень удобно применять для определения концептов.
	----------------------------------------------------------------------------------------------
	Пример с Constructible. 
	
	Проверяет, является ли тип конструируемым при использовании указанных типов аргументов.
*/
template<typename T, typename... Args>
	concept Constructible = 
							requires(Args... args) 
							{ T{args...}; };
/*
	Класс std::is_constructible
	Проверяет, является ли тип конструируемым при использовании указанных типов аргументов.
	template <class T, class... Args>
	struct is_constructible;
	
	Параметры
	
	T		-	Запрашиваемый тип.
	Args	-	Типы аргументов для сопоставления в конструкторе T.
	Замечания
	Экземпляр предиката типа 
	имеет значение 'true', 
	если тип 'T' создается с помощью типов аргументов в 'Args', 
	в противном случае он содержит значение 'false'. 
	
	Тип 'T' является конструируемым, 
	если определение 
	
	T 	type_of_variable_is_T(std::declval<Args>()...); 
	
	переменной хорошо сформировано. 
	
	Как 'T', так и все типы в 'Args' 
	должны быть 
	'полными типами' 
	или 
	'void-массивами неизвестной границы'.
*/
template 	<	typename 		Concrete
			, 	typename		... Ts
			>
std::enable_if_t	<	std::is_constructible<Concrete, Ts...>::value
					, 	std::unique_ptr<Concrete>
					>
constructArgsOld(Ts&&... params) {
	cout << "-------------- I'm " << __func__ << "(Ts&&... params)" << endl;
	
	((cout << params << ", "), ...);
    cout << "\n";
    
	return std::make_unique<Concrete>(std::forward<Ts>(params)...); 
}
template 	<	typename 		Concrete
			, 	typename... 	Ts
			>
std::enable_if_t	<	!std::is_constructible<Concrete, Ts...>::value
					, 	std::unique_ptr<Concrete> 
					>
constructArgsOld(...) {
	cout << "-------------- I'm " << __func__ << "(...)" << endl;
    return nullptr;
}
/*
	std::is_constructible (смотрите c++ reference.com) — 
	позволяет нам быстро проверить, 
	можно ли использовать список аргументов для создания данного типа.
*/
void test_4(){
	cout << "-------------- I'm void " << __func__ << endl;
	
	constructArgsOld<std::vector<int>>(11, 12);
	constructArgsOld<void*[]>(7);
};
/*
	-----------------------------------------------------------------------------------------------
	И пример с Comparable. 
	
	Скажем, что тип 'T' является Comparable, 
	если два объекта типа 'T' можно сравнить с помощью оператора «меньше»,
	и результат конвертируется в bool. 
	
	Cтрелочка -> и тип bool после неё означают, 
	что тип expression конвертируется в bool, 
	а не то, что он равен bool:
*/
template<typename T>
concept Comparable 
				= requires(T const & a, T const & b) {
					{a < b} -> std::convertible_to<bool>;
				};
/*
	Того, что мы рассмотрели, уже достаточно, 
	чтобы показать полноценный пример использования концептов.
	У нас есть уже концепт Comparable, 
	давайте определим концепты для итераторов. 
	
	Скажем, 
	RandomAccessIterator 	— 	это BidirectionalIterator и ещё какие-то свойства. 
	
	Имея это, определим концепт Sortable. 
	
	Range называется Sortable, 
	если его итератор RandomAccessи его элементы можно сравнивать. 
	
	И теперь мы можем написать функцию sort(), 
	которая принимает не абы что, а Sortable Range:
*/
// concepts, полный пример в С++20
template<typename Iterator>
concept BidirectionalIterator 
			= Comparable<Iterator>;
			
template<typename Iterator>
concept RandomAccessIterator 
			= BidirectionalIterator<Iterator>;// && true;  // true - "и ещё какие-то свойства"
template<typename Iterator>
concept Sortable = RandomAccessIterator<Iterator> && Comparable<Iterator>;
template<Sortable Range>
void sort(Range & range) 
{
	cout << "-------------- I'm void " << __func__ << "(Range & range)" << endl;
};
/*
	Теперь, если попробуем вызвать эту функцию от чего-то, 
	не удовлетворяющего концепту Sortable, 
	получим от компилятора хорошую, 
	SFINAE-friendly ошибку с понятным сообщением. 
	
	Попробуем проинстанцировать std::list'ом или вектором элементов, 
	которые не умеют сравниваться:
*/
//concepts, полный пример в С++20, тесты
struct X {};
void test_5() {
	cout << "-------------- I'm void " << __func__ << endl;
	std::vector<int> 	vi; 
	sort(vi); 					// OK
  
	std::list<int> 		li; 
	//sort(li); 				// Fail, list<int>::iterator is not random access
  
	std::vector<X> 		vx; 
	//sort(vx); 				// Fail, X is not Comparable
}
/*
	***********************************************************************************************
	Подробнее о requires expression
	Во-первых, их можно применять не только для определения концептов. 
	
	Hа майкрософтовском компиляторе есть расширение 
	
	__if_exists-
	__if_not_exists. 
	
	Оно позволяет в compile-time проверять существование имени 
	и в зависимости от этого включать или выключать компиляцию блока кода. 
	
	Есть функция f(), она принимает точку Point шаблонного типа 
	и берёт от этой точки высоту. 
	
	Она может инстанцироваться трёхмерной или двухмерной точкой. 
	
	Для трёхмерной мы считаем высотой координату 'z', 
	для двухмерной мы обращаемся к специальному сенсору поверхности. 
	
	Это выглядит вот таким образом:
*/
struct Point2 { float x, y; };
struct Point3 { float x, y, z; };
template<typename Point>
struct Sensor { 
	Sensor(Point& _point) : point{_point}
	{}
	Point 	point;
	
	float get_height(Point const & point){
		cout << "-------------- I'm float " << __func__ << "(Point const & point)" << endl;
		return 0;
	}
};
template<typename Point>
void f_exist_noexist(Point const & point) {
	cout << "-------------- I'm void " << __func__ << "(Point const & point)" << endl;
	float 	h;
	Sensor	sensor{point};
	/*
	__if_exists(Point::z){
		h = point.z;
	}
  
	__if_not_exists(Point::z){
		h = sensor.get_height(point);
	}
		
		В C++20 мы можем это переписать без использования расширений компилятора, 
		стандартным кодом. 
	*/
	if constexpr(requires { Point::z; })
		h = point.z;
	else
		h = sensor.get_height(point);
}
void test_6() {
	cout << "-------------- I'm void " << __func__ << endl;
	Point2	point2;
	Point3	point3;
	f_exist_noexist<Point2>(point2);
	f_exist_noexist<Point3>(point3);
};
/*
	Второй момент — это то, 
	что надо быть бдительным с синтаксисом requires expression.
	Они довольно мощные, и эта мощь достигается тем, 
	что вводится много новых синтаксических конструкций. 
	
	В них можно запутаться, по крайней мере, поначалу.
	Давайте определим концепт Sizable, 
	который проверяет, что у контейнера есть константный метод size, 
	возвращающий size_t. 
	
	Мы, естественно, ожидаем, что vector<int> является Sizable, 
	однако этот static_assert заваливается. 
	
	Понимаете, из-за чего у нас ошибка? 
	
	Почему этот код не компилируется?
*/
template<typename Container>
concept Sizable = requires(Container const & container) {
	{ container.size() } -> std::convertible_to<size_t>; 
};
static_assert(Sizable<std::vector<int>>); 	// Fail
/*
	Давайте я покажу код, который компилируется. 
	
	Такой класс X удовлетворяет концепту Sizable. 
	
	Теперь понимаете, в чём у нас проблема?
*/
struct XX {
	struct Inner {
		int 	size_t;
	};
	Inner* size() const;
};
// static_assert(Sizable<XX>); 		// OK
/*
	Давайте я исправлю подсветку кода. 
	
	Слева код раскрашен так, как мне бы хотелось. 
	
	А на самом деле, он должен быть раскрашен так, как справа:
	Видите, поменялся цвет size_t, стоящего после стрелочки? 
	
	Я хотел, чтобы это был тип, но это просто поле, к которому мы обращаемся. 
	
	Всё, что у нас в requires expression — это одно большое выражение, 
	и мы проверяем его корректность. 
	
	Для типа X — да, это корректное выражение, 
	для std::vector<int> — нет. 
	
	Чтобы достичь того, что мы хотели, нужно взять выражение в {фигурные скобки}:
template<typename Container>
concept Sizable = requires(Container const & c) {
	{c.size()} -> size_t;
};
static_assert(Sizable<vector<int>>); // OK
struct X {
	struct Inner {
		int size_t;
	};
	Inner* 	size() 	const;
};
static_assert(Sizable<X>); // Fail
	Но это просто забавный пример. 
	
	В общем-то, просто нужно проявлять аккуратность.
	***********************************************************************************************
	Примеры использования концептов
	-----------------------------------------------------------------------------------------------
	Реализация класса pair
	Дальше я буду демонстрировать какие-то фрагменты STL, 
	которые можно реализовать в C++17, но довольно громоздко.
	
	А затем мы посмотрим, как в C++20 мы можем улучшить имплементацию.
	Давайте начнём с класса pair<>.
	Это очень старый класс, он есть ещё в C++98.
	Он не содержит какой-то сложной логики, так что
	хотелось бы, чтобы его определение выглядело примерно таким образом.
	Оно, с моей точки зрения, должно примерно на этом и закончиться:
	template<typename F, typename S> 
	struct pair {
		F 	f; 		// first
		S 	s;		// second
		...
	};
	Но, согласно cppreference, у pair одних только конструкторов 8 штук.
	
	А если посмотреть в настоящую реализацию, 
	допустим, в майкрософтовскую STL, 
	то будет целых 15 конструкторов класса pair<>. 
	
	Мы не будем смотреть на всю эту мощь и ограничимся конструктором по умолчанию.
	Казалось бы, в нём-то что сложного? 
	
	Для начала поймём, зачем он нужен. 
	
	Мы хотим, чтобы если один из аргументов класса pair<> был тривиального типа, 
	допустим, int, то после конструирования класса pair<> 
	он был инициализирован нулём, 
	а не оставался неинициализированным. 
	
	Для этого мы хотим написать такой конструктор, 
	который вызовет value-инициализацию для полей f (first) и s (second).
	template<typename F, typename S> 
	struct pair {
		F 	first; 
		S 	second;
		pair() : first(), second()
		{}
	};
	К сожалению, если мы попробуем проинстанцировать pair<> от чего-то, 
	что не имеет конструктора по умолчанию, 
	допустим, от такого класса 'А', 
	мы сразу же получим ошибку компиляции. 
	
	Желаемое поведение — 
	это чтобы при попытке сконструировать pair<> по умолчанию 
	была бы ошибка компиляции, 
	но если мы явно передаём значения 'f' и 's', 
	то всё бы работало:
	struct A {
		A(int);
	};
	pair<int, A> a2;     		// must fail
	pair<int, A> a1{ 1, 2 };    // must be OK
	Для этого нужно сделать конструктор по умолчанию шаблонным и ограничить его по SFINAE.
	
	Первая идея, которая приходит в голову, — 
	давайте напишем так, 
	что этот конструктор разрешён, 
	только если f и s — is_default_constructable:
	template<typename F, typename S> 
	struct pair {
		F 	f; 
		S 	s;
		
		template	<typename = enable_if_t	<conjunction_v	<	is_default_constructible<F>
															,   is_default_constructible<S>
															>
											>
					>
		pair() : f(), s() 
		{}
	};
	Это работать не будет, потому что аргументы enable_if_t<> 
	зависят только от шаблонных параметров класса. 
	
	То есть после подстановки класса они становятся независимыми, 
	их можно немедленно вычислить. 
	
	Но если мы получим false, соответственно, 
	мы снова получим hard compiler error.
	Чтобы это преодолеть, 
	давайте добавим ещё шаблонных параметров в этот конструктор 
	и сделаем так, 
	чтобы условие enable_if_t было зависимо от этих шаблонных параметров:
template<typename F, typename S> 
struct pair {
	F 	f; 
	S 	s;
	template	<	typename T = F
				, 	typename U = S
				,	typename = std::enable_if_t <std::conjunction_v	<	std::is_default_constructible<T>
																	,   std::is_default_constructible<U>
																	>
												>
				>
	pair() : f(), s() 
	{}
};
	Cитуация довольно забавная. 
	
	Дело в том, что шаблонные параметры 'T' и 'U' 
	не могут быть заданы пользователем явно. 
	
	В C++ нет синтаксиса для того, 
	чтобы явно задать шаблонные параметры конструктора, 
	они не могут быть выведены компилятором, 
	потому что ему их неоткуда выводить. 
	
	Они могут прийти только из значения по умолчанию. 
	
	То есть, 
	эффективно этот код ничем не отличается от кода в предыдущем примере. 
	
	Однако с точки зрения компилятора, он валидный, 
	а в предыдущем примере — нет.
	Мы решили нашу первую проблему, 
	но сталкиваемся со второй, чуть более тонкой. 
	
	Предположим, у нас есть класс 'B' 
	с explicit конструктором по умолчанию, 
	и мы хотим неявно сконструировать pair<int, B>:
	struct B { explicit B(); };
	pair<int, B> 	pair_obj = {};
	У нас это получится, 
	но, по стандарту, не должно получиться. 
	
	По стандарту, пара должна неявно дефолт конструироваться, 
	только если оба её элемента неявно дефолт конструируются.
	Вопрос: нужно ли нам писать конструктор пары explicit или нет? 
	
	В C++17 у нас есть соломоново решение: давайте напишем и такой, и такой.
	template<typename F, typename S> 
	struct pair {
		F 	f; 
		S 	s;
		template	<	typename T 	= F
					, 	typename U 	= S
					,   typename 	= enable_if_t<conjunction_v	<	is_default_constructible<T>
																,   is_default_constructible<U>
																,   is_implicity_default_constructible<T>
																,   is_implicity_default_constructible<U>
																>
												 >
					>
		pair() : f(), s() 
		{}
		template<...> 
		explicit pair() : f(), s() 
		{}
	};
	Теперь у нас два конструктора по умолчанию:
	-	один из них мы отрежем по SFINAE для случая, 
		когда элементы — implicitly default constructible;
	и 
	
	-	второй для противоположного случая.
	К слову, для реализации type trait 
	
	std::is_implicitly_default_constructible 
	
	в C++17, я знаю такое решение, 
	но решения без SFINAE я не знаю:

	template<typename T> 
	true_type 	test(T, int);
	
	template<typename T> 
	false_type 	test(int, ...);
	template<typrname T>
	using 	is_implicity_default_constructible = decltype(test<T>({}, 0));
	Если мы теперь попробуем всё-таки неявно сконструировать pair<int, B>, 
	то получим ошибку компиляции, как и хотели:
	template<..., typename = enable_if_t<conjuction_v	<	is_default_constructible<T>
														,   is_default_constructible<U>
														,	is_implicity_default_constructible<T>
														,	is_implicity_default_constructible<U>
														>
										>
			>
	pair<int, B> 	p = {};
	candidate template ignored: requirement 'conjunction_v<
	is_default_constructible<int>,
	is_default_constructible<B>,
	is_implicity_default_constructible<int>,
	is_implicity_default_constructible<B>
	>' 
	was not satisfied [with T=int, U=B]
	В разных компиляторах эта ошибка будет разной степени понятности. 
	
	К примеру, майкрософтовский компилятор в данном случае говорит: 
	«Не получилось сконструировать пару <int, B> от пустых фигурных скобок». 
	
	GCC и Clang к этому ещё добавят: 
	«Мы попробовали такой и такой конструктор, ни один из них не подошёл», — 
	и про каждый скажут причину.
	Какие у нас тут есть конструкторы? 
	
	Есть сгенерированные компилятором 'copy'- и 'move'- конструкторы, 
	есть написанные нами. 
	
	С 'copy' и 'move' всё просто: 
	они ожидают один параметр, мы передаём ноль. 
	
	Для нашего конструктора причина в том, что подстановка зафейлилась.
	GCC говорит: 
	«Substitution failed, попытался найти внутри enable_if<false> тип type — не нашёл, извините».
	Clang считает эту ситуацию special case. 
	
	Поэтому он очень здорово показывает эту ошибку. 
	
	Если у нас при вычислении enable_if первого аргумента получился false, 
	он пишет, что конкретный requirement не удовлетворён.
	При этом мы сами себе испортили жизнь тем, 
	что сделали громоздкое условие enable_if. 
	
	Мы видим, что оно получилось false, но пока не видим, почему.
	Это можно преодолеть, если мы разобьём enable_if на четыре таким образом:
	template	<...
				,	typename = enable_if_t<is_default_constructible<T>::value>>
				,   typename = enable_if_t<is_default_constructible<U>::value>>
				,   typename = enable_if_t<is_implicity_default_constructible<T>::value>>
				,   typename = enable_if_t<is_implicity_default_constructible<U>::value>>
				>
	...
	Теперь при попытке неявно сконструировать пару 
	мы получим отличное сообщение, 
	что такой-то кандидат не подходит, 
	потому что type trait is_implicitly_default_constructable не удовлетворён:
	pair<int, B> p = {};
	// candidate template ignored: 
	// requirement 'is_implicity_default_constructible<B>::value' was not satisfied with...
	Может даже на секунду показаться: 
	зачем нам концепт, если у нас такой классный компилятор?
	Но потом мы вспоминаем, 
	что для реализации конструктора по умолчанию 
	используются две шаблонные функции, 
	а в каждой по шесть шаблонных параметров. 
	
	Для языка, претендующего на звание мощного, это перебор.
	Чем нам поможет C++20? 
	
	Сначала избавимся от шаблонов, 
	переписав это с помощью requires clause. 
	
	То, что мы раньше писали внутри enable_if, 
	теперь пишем внутри аргумента requires clause:
	template<typename F, typename S> 
	struct pair {
		F 	f; 
		S 	s;
		pair() 	requires 		DefaultConstructible<F>
							&& 	DefaultConstructible<S>
							&& 	ImplicitlyDefaultConstructible<F>
							&& 	ImplicitlyDefaultConstructible<S>
        : f(), s() 
		{}
		// explicit pair() ...      
	};
	Концепт ImplicitlyDefaultConstructible 
	можно реализовать с помощью такого симпатичного requires expression, 
	внутри которого используются почти только 'скобки разной формы':
*/
template<typename T> 
concept 	ImplicitlyDefaultConstructible =
										requires 	{ 
														[](T){} 	({}); 
													};
/*
	Здесь тип 'T' является 
	ImplicitlyDefaultConstructible, 
	если 'лямбду', ожидающую один параметр типа 'T' 
	можно вызвать от пустых {фигурных скобок}. 
	
	В принципе, та же идея, что и в реализации через SFINAE.
	Ещё одна фича C++20: 
	появляется условный (conditional) explicit 
	(так же как условный noexcept). 
	
	Теперь мы можем в explicit писать условия. 
	
	Поэтому теперь 
	не нужно писать 
	два шаблона 
	и 
	два конструктора, 
	можно ограничиться одним соответствующим explicit.
	template<typename F, typename S> 
	struct pair {
		F 	f; 
		S 	s;
		explicit	(	!ImplicityDefaultConstructible<F> 
					||  !ImplicityDefaultConstructible<S>
					)
		pair() 	requires 		DefaultConstructible<F>
							&& 	DefaultConstructible<S>
		: f(), s() 
		{}
	};
	Такое решение мне уже кажется хорошим, 
	оно понятно читается. 
	
	Конструктор определён тогда и только тогда, 
	когда оба элемента DefaultConstructible, 
	и он explicit, 
	если хотя бы один из них explicit.
	---------------------------------------------------------------------------------------------------
	Реализация класса Optional в C++17
	Теперь мы посмотрим на реализацию класса Optional. 
	
	Тут мы не ограничимся конструктором по умолчанию, 
	это было бы слишком просто.
	Давайте попробуем написать минимальную рабочую реализацию. 
	
	Как она должна выглядеть? 
	
	Хотелось бы так, 
	но у нас в C++ нет алгебраических типов данных:
	enum Option<T> {
		None,
		Some(t)
	}
	Тогда так:
	class Optional<T> 
	{
		final 	T 	value;
		
		Optional()  		{this.value = null; }
		Optional(T value)  	{this.value = value; }
	
		Это решение тоже не подходит для C++: 
		какие там null, когда у нас value-семантика?
		Давайте писать настоящее C++ решение. 
		
		Возьмём булевский флажок 'initialized' и 'storage', 
		в котором мы будем хранить объект, 
		если он есть. 
		
		Мы не можем хранить значение как объект типа 'T', 
		потому что для пустого 'optional' 
		никакого объекта 'T' существовать не должно, 
		по C++ memory model.
		template<typename T> 
		class optional {
			bool 										initialized;
			
			aligned_storage_t<sizeof(T), alignof(T)> 	storage;
			...
		}
		Давайте сразу напишем геттеры, они нам пригодятся. 
		
		Ещё конструкторы: 
		один создаёт пустой 'optional', 
		другой создаёт 'optional' со значением. 
		
		И ещё нам нужен деструктор:
		...
		T       & 	get()       & 	{ return reinterpret_cast<T       &>(storage); }
		T const & 	get() const & 	{ return reinterpret_cast<T const &>(storage); }
		T      	&& 	get()      	&& 	{ return move(get()); }
		optional() 					noexcept 
					: initialized(false) 
		{}
		optional(T const & value) 	noexcept(NothrowCopyConstructible<T>)
					: initialized(true) 
		{
			new (&storage) T(value);
		}
		
		~optional() : noexcept(NothrowDestructible<T>) 
		{ 
			if (initialized) 
				get().~T(); 
		}
	};
	Таким optional'ом уже можно пользоваться. 
	
	Можно конструировать пустой optional, 
	можно конструировать optional со значением, 
	он корректно разрушится, 
	но такой optional пока ещё нельзя копировать и перемещать. 
	
	Если мы явно пишем деструктор, то компилятор у нас уже не генерирует copy и move операции.
	Давайте напишем их. 
	
	Их всего четыре: 
	два конструктора 
	и 
	два assignment оператора. 
	
	Я ограничусь двумя, поскольку они симметричны. 
	
	Выберем из каждого класса по представителю. 
	
	Напишем copy constructor. 
	
	Он довольно простой:
	template<typename T> 
	class optional {
		bool 										initialized;
		
		aligned_storage_t<sizeof(T), alignof(T)> 	storage;
		...
		optional(optional const & other) 	noexcept(NothrowCopyConstructible<T>)
			: initialized(other.initialized) 
		{
			if (initialized) 
				new (&storage) T(other.get());
		}
		optional& operator =(optional && other) noexcept(...) 
		{
			...
		}
	};
	Напишем move assignment. 
	
	Он чуть более громоздкий, поскольку нужно разбирать случаи:
	Если оба optional'а пустые, не надо ничего делать.
	
	Если они оба содержат значение, мы присваиваем значение.
	Если один содержит значение, а другой — нет, мы перемещаем значение, разрушаем старое.
	Здесь мы использовали для типа T три операции: 
	-	move constructor, 
	-	move assignment 
	и 
	-	деструктор:
	
	optional& operator =(optional && other) noexcept(...) {
  if (initialized) {
    if (other.initialized) {
      get() = move(other.get());
    } else {
      initialized = false; other.initilized = true;
      new(&other.storage) T(move(get()));
      get().~T();
    }
  } else if (other.initialized) {
    initialized = true; other.initialized = false;
    new(&storage) T(move(get()));
    other.get().~T();
  }
  return *this;
}
На эти три операции нам нужно написать спецификацию noexcept:

optional& operator =(optional && other)
    noexcept(NothrowAssignable<T> &&
             NothrowMoveConstructible<T> &&
             NothrowDestructible<T>) {
  if (initialized) {
    if (other.initialized) {
      get() = move(other.get());
    } else {
      initialized = false; other.initialized = true;
      new (&other.storage) T(move(get()));
      get().~T();
    }
  }
  ...
}
Класс optional будет выглядеть примерно таким образом:

template<typename T> class optional {
  ...
  optional(optional const &)  noexcept(NothrowCopyConstructible<T>);
  optional(optional &&)       noexcept(NothrowMoveConstructible<T>);
  optional& operator =(optional const &)  noexcept(...);
  optional& operator =(optional &&)       noexcept(...);
};
Но тут мы сталкиваемся с той же проблемой, что и с конструктором по умолчанию класса pair:
когда мы пытаемся проинстанцировать этот Optional от чего-то, у чего некоторые специальные операции не существуют (например, deleted), мы получаем compilation error.

template class optional<unique_ptr<int>>; // compilation error
Желаемое поведение было бы, чтобы optional от unique_ptr можно было бы инстанцировать,
просто copy constructor и copy assignment были бы deleted. В случае с конструктором по умолчанию пары мы решали это тем, что делали его шаблонным, а потом ограничивали по SFINAE.
Это решение не подходит для copy и move конструкторов и assignment операторов, поскольку у них жёстко определена сигнатура — они не могут быть шаблонными. Можно написать что-то шаблонное, что после подстановки напоминает copy конструктор, но в действительности им не является.

Возможное решение — использовать трюк. В каждой из специальных операций начнём с copy конструктора и определим две вспомогательные структуры: deleted operation и, собственно, operation:

deleted_copy_construct объявляет соответствующую операцию delete, а остальные — default;
copy_construct дефолтит три операции, а в copy_construct просто вызывает метод базового класса.
template<class Base> struct deleted_copy_construct : Base {
  deleted_copy_construct(deleted_copy_construct const &)                = delete;
  deleted_copy_construct(deleted_copy_construct &&)                     = default;
  deleted_copy_construct& operator =(deleted_copy_construct const &)    = default;
  deleted_copy_construct& operator =(deleted_copy_construct &&)         = default;
};
template<class Base> struct copy_construct : Base {
  copy_construct(copy_construct const & other)
      noexcept(noexcept(Base::construct(other))) {
    Base::construct(other);
  }
  copy_construct(copy_construct &&)                     = default;
  copy_construct& operator =(copy_construct const &)    = default;
  copy_construct& operator =(copy_construct &&)         = default;
};
Заведём метафункцию select_copy_construct, которая в зависимости от того, является тип CopyConstrictuble или нет, либо вернёт наш copy_construct, либо deleted_copy_construct:

template<typename T, class Base>
using select_copy_construct = conditional_t<CopyConstructible<T>
  copy_construct<Base>
  deleted_copy_construct<Base>
>;
То, что раньше называлась optional, переименуем в optional_base, copy конструктор переименуем в метод construct с соответствующей логикой, а класс optional унаследуем от
select_copy_construct<T, optional_base<T>>. Так мы обеспечим правильную семантику для copy конструктора:

template<typename T> class optional_base {
  ...
  void construct(optional_base const & other) noexcept(NothrowCopyConstructible<T>) {
    if ((initialized = other.initialized)) new (&storage) t(other.get());
  }
};
template<typename T> class optional : select_copy_construct<T, optional_base<T>> {
  ...
};
Аналогично мы поступим с остальными операциями. Единственное, что, если у нас copy_construct к базе делегировал работу, то move_construct у нас будет делегировать работу к copy_construct, copy_assign, соответственно, к move_construct, реализует свою операцию и передаёт по цепочке другой, мол, ты реализуй свою операцию:

template<typename T, class Base>
using select_move_construct = select_copy_construct<T,
  conditional_t<MoveConstructible<T>,
    move_construct<Base>
  >
>;
template<typename T, class Base>
using select_copy_assign = select_move_construct<T,
  conditional_t<CopyAssignable<T> && CopyConstructible<T>,
    copy_assign<Base>
    delete_copy_assign<Base>
  >
>;
Соответственно, move_assign к copy_assign, optional_base выглядит таким образом, вместо конструкторов и assignment операторов методы construct и assign, и optional наследуется от select_move_assign<T, optional_base<T>>.

template<typename T, class Base>
using select_move_assign = select_copy_assign<T, ...>;
template<typename T> class optional_base {
  ...
  void construct(optional_base const&)  noexcept(NothrowCopyConstructible<T>);
  void construct(optional_base &&)      noexcept(NothrowMoveConstructible<T>);
  optional_base& assign(optional_base &&)       noexcept(...);
  optional_base& assign(optional_base const &)  noexcept(...);
};
template<typename T> class optional : select_move_assign<T, optional_base<T>> {
  ...
};
Соответственно, мы получаем такую симпатичную иерархию наследования:
optional<unique_ptr> наследуется от deleted_copy_construct, тот от
move_construct и так далее. Зато работает!

optional<unique_ptr<int>>
  : deleted_copy_construct<...>
    : move_construct<...>
      : deleted_copy_assign<...>
        : move_assign<...>
          : optional_base<unique_ptr<int>>
Но по дороге мы потеряли полезные свойства: наш optional даже от TriviallyCopyable типов перестал быть TriviallyCopyable.

Что такое TriviallyCopyable? Грубо говоря, тип T является TriviallyCopyable, если его можно
копировать с помощью memcpy. Это довольно полезное свойство, которое позволяет компилятору во многих случаях генерировать более оптимальный код.

К примеру, именно такие типы можно передавать как аргументы функции через регистры и возвращать, соответственно, через регистры. Если мы делаем resize для vector TriviallyCopyable типов, то нам для перемещения элементов из старого буфера в новый можно выполнить один memcpy, а не в цикле копировать старые элементы, а потом разрушать. В общем, свойство полезное, терять его не хочется.

Для того чтобы тип был TriviallyCopyable, нужно, чтобы выполнялись следующие пять static_assert'ов, его специальные операции copy-move и деструктор должны быть тривиальными:

template<typename T>
class optional : select_move_assign<T, optional_base<T>> {...};
static_assert(TriviallyCopyable<optional<int>>);
static_assert(TriviallyCopyConstructible<optional<int>>);
static_assert(TriviallyMoveConstructible<optional<int>>);
static_assert(TriviallyCopyAssignable   <optional<int>>);
static_assert(TriviallyMoveAssignable   <optional<int>>);
static_assert(TriviallyDestructible     <optional<int>>);
У нас все эти пять static_assert'ов заваливаются. Что обидно, нет фундаментальных причин, почему оно должно себя так вести. Ведь поля optional — это aligned_storage, грубо говоря, массив байт, и булевский флажок, оба TriviallyCopyable.

Вся беда в том, что мы явно написали логику конструктора копирования и так далее. Если бы мы просто ничего не писали, мы бы сохранили свойство TriviallyCopyable.

К счастью, у нас есть способ сделать лучше. Давайте вспомним нашу метафункцию select_copy_construct:

template<typename T, class Base>
using select_copy_construct = conditional_t<CopyConstructible<T>,
  copy_construct<Base>
  deleted_copy_construct<Base>
>;
Для CopyContructible типов мы выдавали copy_construct, но можно написать ещё один if в compile-time: если тип CopyContructible и TriviallyCopyContructible, тогда мы просто выдаём Base.

template<typename T, class Base>
using select_copy_construct = conditional_t<CopyConstructible<T>,
  conditional_t<TriviallyCopyConstructible<T>,
    Base,
    copy_construct<Base>
  >,
  deleted_copy_construct<Base>
>;
Соответственно, у нас нет нашего copy конструктора. Так же мы сделаем для всех остальных операций, плюс ещё добавим select_destruct для деструктора. Поскольку деструктор для int ничего не делает, но из-за того что мы написали какой-то код, он перестал быть тривиальным.

template<typename T, class Base>
using select_destruct = conditional_t<TriviallyDenstructible<T>,
    Base,
    destruct<Base>
  >
>;
Таким образом, наша иерархия увеличивается ещё немножко, мы добавили туда деструктор. Опять же, безобразно, зато работает:

optional<unique_ptr<int>>
  : deleted_copy_construct<...>
    : move_construct<...>
      : deleted_copy_assign<...>
        : move_assign<...>
          : destruct<optional_base<unique_ptr<int>>>
            : optional_base<unique_ptr<int>>
Таким образом, в C++17 для реализации optional нам потребовалась иерархия наследования глубиной 7; завести вспомогательные классы для каждой операции: по два класса operation, deleted_operation и метафункцию select_operation; вынести реализацию construct и assign во вспомогательные функции. В общем, довольно много рутинной работы.

Все наши проблемы были из-за реализации специальных мемберов. Давайте посмотрим на них немного отвлечённо. Операции в первом приближении делятся на два класса: нормальные и deleted.

Если присмотреться, то из нормальных операций выделяется подкласс noexcept операций.
Если присмотреться, в свою очередь, к этому подклассу, то выделяется подкласс trivial операций, noexcept операции могут быть тривиальными или мы их вручную написали. В некотором смысле, эти классы упорядочены, то есть из trivial следует noexcept, а из noexcept следует, что операция не deleted. Поэтому я позволил себе их разместить тут вдоль некой воображаемой оси. Четыре класса на этой оси образуют четыре промежутка, четыре промежутка разделены тремя перегородками, на слайде они жёлтые.

Для каждой из этих перегородок у нас есть специальный type trait, который говорит, в какую сторону от перегородки попадает наша операция. Тут, например, copy конструктор: он deleted или нормальный, он nothrow или нет, и тривиальный ли он?

С другой стороны, если вы реализуете свой класс и вы хотите обеспечить в нём какой-то special member, то, в зависимости о того, чего вы пытаетесь достичь, у вас есть следующие средства:

если вы хотите, чтобы он стал deleted, вы пишете = delete или наследуетесь от нашего класса deleted_copy_construct;
если он попадает в промежуточные классы, то вы там пишете copy_construct, реализуете его c соответствующей noexcept спецификацией;
наконец, если он тривиальный, вы просто не пишите его, либо дефолтите.
Давайте просто возьмём эту схему и перенесём один в один на код с концептами.

Реализация класса optional в C++20
Как в C++20 должен выглядеть optional и его copy конструктор?
Есть три реализации:

если тип T не CopyConstructible, мы объявляем его как deleted;
если он TriviallyCopyConstructible, мы его дефолтим;
иначе мы пишем его реализацию с соответствующей noexcept спецификацией.
template<typename T> class optional {
  ...
  optional(optional const &) requires(!CopyConstructible<T>) = delete;          // #1
  optional(optional const &) requires(TriviallyCopyConstructible<T>) = default; // #2
  optional(optional const &) noexcept(NothrowCopyConstructible<T>) {...}        // #3
  ...
  ~optional() requires(TriviallyDestructible<T>) = default;
  ~optional() noexcept(NothroeDestructible<T>) {...}
};
Давайте поймём, как это работает. Во-первых, первые две перегрузки являются взаимоисключающими, то есть после подстановки типа T аргумент requires clause одной из них вычислится в false. Мы получим там requires(false), и это значит, что данная перегрузка исключается из overload resolution. У нас может возникнуть ситуация, что одна из этих перегрузок вычислилась в requires(true), то есть она не отброшена и при этом у нас есть третья перегрузка.
В этом случае будет предпочтена первая или вторая перегрузка, поскольку она более ограничена.

То есть requires clause работает не как = delete:

Функция с = delete участвует в overload resolution, но если она будет выбрана, то вы получите ошибку компиляции, потому что пытаетесь вызвать deleted функцию.
Функция с requires(false) будет просто исключена из overload resolution.
И это нормально, что мы имеем несколько copy конструкторов, несколько перегрузок, даже не отрезанных requires clause. Просто из них будет выбираться более предпочтительная.
Более того, мы даже можем иметь несколько деструкторов. Дождались! Сорок лет в C++ нельзя было перегружать деструкторы, ну что такое? И вот, наконец, можно. По этому поводу было обсуждение и некоторые люди, особенно разработчики компилятора, были не очень рады, что теперь можно перегружать деструкторы. Но консенсус таков, что фича полезная, поэтому давайте дадим её пользователям, чтобы можно было вот таким образом, допустим, тот же optional реализовывать.
Правда, если вы сейчас попробуете скомпилировать этот код, то GCC упадёт с internal compiler error, а Clang не будет правильно работать. Но это понятно, поддержать концепты сложно. У них ещё есть время, они подтянутся.
В принципе, это почти всё, что я хотел сказать про реализацию optional в C++20. Как видим, существенно меньше мучений, чем в C++17.

Альтернатива aligned_storage и aligned_union
Остаётся один момент: я использовал aligned_storage и для этого мне в геттерах приходилось делать reinterpret_cast, а reinterpret_cast нельзя использовать в constexpr функциях. Соответственно, в compile-time такой optional работать не будет, а мы же хотим всю стандартную библиотеку в compile-time. Поэтому настоящая реализация STL не использует aligned_storage для реализации optional и не использует aligned_union для реализации variant. Хотя, казалось бы, эти классы перекочевали в STL из Boost именно для реализации optional и variant соответственно. Давайте посмотрим на примере variant, что используется вместо этого:
template<bool all_types_are_trivially_destructible, typename...>
class _Variant_storage_;
template<typename... _Types> using _Variant_storage = _Variant_storage_<
  conjunction_v<is_trivially_destructible<_Types>...>,
  _Types...
>;
template<typename _First, typename... _Rest>
class _Variant_storage_<true, _First, _Rest...> {
  union {
    remove_const_t<First> _Head;
    _Variant_storage<_Rest...> _Tail;
  };
};
Это фрагмент из майкрософтовской реализации variant. Есть вспомогательный класс _Variant_storage_, который параметризуется, во-первых, альтернативами, которые может хранить variant, а во-вторых, дополнительным булевским шаблонным параметром. Правда ли, что все альтернативы trivially_destructible? У нас есть вспомогательный type alias, который подставляет реальное значение этого шаблонного параметра. И у нас есть специализация _Variant_storage_ на случай, когда этот параметр true и когда он false. Вариант, когда он true, более простой. Если у нас все типы trivially_destructible, мы можем просто хранить union из первого элемента и Variant'а от остальных.
Я думаю, что идея, как это работает, примерно понятна, но получилось довольно громоздко. Нам потребовалось вводить дополнительный type alias в _Variant_storage. Обычно такие штуки записываются примерно так:
template<typename... _Types,
         bool = conjunction_v<is_trivially_destructible<_Types>...>
  >
class _Variant_storage_;
Мы просто указываем значения по умолчанию шаблонного параметра. Однако в данном случае это сделать не получится, потому что variadic template параметр должен идти последним. Соответственно, мы не можем писать булевский параметр после него, но мы не можем написать булевский параметр с дефолтным значением перед ним, поскольку тогда мы не будем видеть ещё определения идентификатора _Types. Поэтому на C++17 приходится мучиться, как мы мучились.

На C++20 нам не нужно вообще вводить дополнительный шаблонный параметр,
для того чтобы специализировать классы, ведь у нас есть для этого
requires clause. То есть та же самая идея в C++20 записывается такой вот requires clause:
template<typename... _Types>
class _Variant_storage_;
template<typename _First, typename... _Rest>
  requires(TriviallyDestructible<_First> && ... && TriviallyDestuctible<_Rest>)
class _Variant_storage_<_First, _Rest...> {
  union {
    remove_const_t<_First> _Head;
    _Variant_storage_<_Rest...> _Tail
  };
};
Эта реализация класса _Variant_storage_ подходит, только если все типы TriviallyDestructible. Таким образом, мы с вами уже видели применение requires clause для шаблонной функции, для нешаблонной функции, а теперь и для других шаблонных деклараций.
Использование requires clause для template type alias
Говорят, что можно будет использовать requires clause даже для template type alias. То есть если вам в C++20 вдруг зачем-то понадобится enable_if, вы сможете реализовать его таким простым и изящным способом:
template<bool condition, typename T = void>
  requires condition
using enable_if_t = T;
Код, который не поддержит ни один компилятор
Я надеюсь, что вам было не очень скучно читать про концепты. У меня есть ещё один пример кода:

// Equivalent, but functionally not equivalent
template<typename T> enable_if_t<(sizeof(T) < 239)> f();
template<typename T> enable_if_t<(sizeof(T) > 239)> f();
// Not equivalent
template<typename T> requires(sizeof(T) < 239) void f();
template<typename T> requires(sizeof(T) > 239) void f();
Я готов заключить пари с любым желающим, что реализованный таким образом enable_if никогда ни один компилятор не поддержит. В чём тут проблема? Давайте напишем две функции f(): одну с enable_if, который зарезает типы меньшие по размеру, чем 239, вторую, соответственно, которые больше, чем 239. И мы получаем ещё один пример эквивалентных, но функционально не эквивалентных функций:

с одной стороны, это одна и та же функция, поскольку компилятор обязан раскрывать template type alias'ы, и для него это просто «void f(); void f();
с другой стороны, с точки зрения SFINAE, очевидно, это должны быть две разные функции, у них непересекающиеся области определения.
Программист, который написал код через enable_if, на самом деле имел в виду, что первая функция должна быть ограничена для типов с size < 239, а вторая для типов с size > 239. Однако у компилятора нет способа понять, что два эти определения на самом деле об одном и том же. Для него второй вариант является валидным, поскольку во втором случае функции f() не являются эквивалентными. При эквивалентности мы проверяем ещё и requires clause. А первый вариант — это эквивалентные функции, при этом функционально они не эквивалентны.

	Если вам понравился этот доклад Андрея Давыдова — 
	обратите внимание, что он уже вовсю готовит новый. 
	
	Совсем скоро в Петербурге пройдет C++ Russia 2019 Piter, 
	где Андрей выступит с темой «Модули: изменения в core language». 
	
	В этом докладе он расскажет, например, следующее: 
	что такое reachable entity, 
	и чем это отличается от visible, 
	как модули влияют на ADL, 
	могут ли entities с internal linkage 
	протечь в другой модуль. 
	
	А кроме него, тему модулей на C++ Russia 
	будет раскрывать ещё и Дмитрий Кожевников (JetBrains) 
	с докладом «Модули в С++20 — правда или вымысел?»
*/