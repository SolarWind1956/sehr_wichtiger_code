#pragma once
/*
	The_concepts_from_the_book_next_next.hpp
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
#include <forward_list>
#include <unordered_set>
#include <ranges>
#include <typeinfo> 
#include <type_traits> 
#include <compare>

// Для clang и gcc математические библиотеки в разных заголовках описаны
#if defined(__clang__)
	#include <math.h>
#else	
	#include <cmath>
#endif

#include "..\type_name_as_str.h"
/*	
	***************************************************************************************************
	Rainer Grimm 'C++20 Get the Details'
	--------------------------------------------------------------------------------------------
	
	--------------------------------------------------------------------------------------------
*/	
/*
	*********************************************************************************************************************
	4.1.8 Определение концептов
 
	Когда нужный концепт не входит в число предопределенных концептов в С++20, 
	то придется определить свой концепт. 
	
	В этом разделе я определю несколько концептов, 
	которые будут отличаться от предопределенных 
	использованием синтаксиса CamelCase. 
	
	Соответственно, мой концепт для знаковых целых чисел будет называться SignedIntegral, 
	в то время как соответствующий стандартный концепт в С++20 называется signed_integral.
	
	Синтаксис для определения концепта очень прост.
	
	Определение концепта

template <template parameter-list>
	concept concept-name = 
			constraint-expression;

	Определение концепта начинается с ключевого слова template 
	и содержит список параметров шаблона 'parameter-list'. 
	
	Вторая строка определения более интересна – 
	она использует ключевое слово 'concept', 
	за которым идет имя концепта concept-name

	и выражение, 
	задающее соответствующее ограничение constraint-expression (тело концепта).
	
	Часть constraint-expression может быть:
 
	-	логической комбинацией других концептов или предикатов времени компиляции:
 
		'логическая комбинация' может быть построена при помощи 
		конъюнкции (&&), 
		дизъюнкции (||) 
		или 
		отрицания (!);
		
		'предикаты времени компиляции' 	– 	это вызываемые сущности, 
											которые возвращают логическое значение 
											во время компиляции;
	-	выражение-requires 				-	определяет:
 
											-	простые требования;
											-	требования типов; 
											-	составные требования;
											-	вложенные требования.
 
	В следующих двух разделах я покажу различные способы определения концептов.
	
	4.1.8.1 Логическая комбинация других концептов и предикатов времени компиляции
	
	Вы можете объединять концепты и предикаты времени компиляции 
	при помощи конъюнкции (&&) и дизъюнкции (||). 
	
	При построении логической комбинации вы можете применять к компонентам отрицание (!). 
	
	Благодаря наличию большого количества готовых предикатов в библиотеке признаков типа (type-traits library)
	в вашем распоряжении есть все необходимые инструменты для построения концептов.
	
	Не определяйте концепты рекурсивно и не пытайтесь ограничить их.
	
	!!!!!!!!		Рекурсивное определение концепта недопустимо.
 
	Рекурсивное определение концепта
	
	template<typename T>
	concept Recursive = Recursive<T*>;
 
	В этом случае GCC выдает ошибку, что 'Recursive' was not declared in this scope 
	(Recursive не был определен в этой области видимости).
	
	Когда вы пытаетесь ограничить концепт так, как показано в следующем примере кода, 
	GCC выдает ошибку, что concept cannot be constrained (концепт нельзя ограничивать).

	**********************************************************************************************
	4.1.8.2 Выражения requires
 
	Благодаря выражениям requires 
	вы можете определять концепты с мощными возможностями. 
	
	Выражение requires имеет следующий вид.
 
	Выражение requires
 
	requires (parameter-list(optional)) 	{
			requirement-seq
	}
 
	parameter-list: 	разделенный запятыми список параметров, 
					аналогичный списку в определении функции.
	
	requirement-seq: 	последовательность, состоящая из 
					-	простых, 
					-	составных 
					или 
					-	вложенных 
									требований 
					и 
					-				требований типа.
	
	-----------------------------------------------------------------------------------------------------------	
	4.1.8.2.1 Простые требования
	
	... The_concepts_from_the_book_next.hpp
	
	Концепты должны основываться на общих идеях 
	и необходимо давать им понятные имена для переиспользования. 
	
	Они незаменимы для обеспечения поддержки кода. 
	
	Анонимные концепты выглядят больше как синтаксические ограничения на параметры шаблона.
*/
/*
	---------------------------------------------------------------------------------------------------
	4.1.8.2.2 Требования типа			-		 4.1.8.2.2 Type Requirements
	
	В требованиях типа вы должны использовать ключевое слово typename вместе с именем типа.

	Концепт TypeRequirement

 template<typename T>
 concept TypeRequirement = 	requires {
								typename T::value_type;
								typename Other<T>;
							};
							
	Концепт TypeRequirement требует, 
	чтобы у типа 'T' был вложенный тип value_type, 
	а класс Other<T> можно было инстанцировать при помощи типа 'Т'.
	
	Использование концепта TypeRequirement
*/ 
template <typename>
struct Other;

template <>
struct Other<std::vector<int>> 
{
	Other(){
		cout << "-------------- I'm COTOR " << __func__ << "()" << endl;
	}	
};

template<typename T>
concept TypeRequirement = 	requires {
								typename 	T::value_type;
								typename	Other<T>;
							};
							
void test_14(){
	cout << "-------------- I'm void " << __func__ << "()" << endl;

	TypeRequirement auto myVec = std::vector<int>{1, 2, 3};
	
	cout << "" << type_name_as_str<decltype(myVec)>() << endl;
}
/*
	В этом примере выражение 
	
	TypeRequirement auto myVec = std::vector<int>{1, 2, 3} 
	
	является допустимым. 
	
	У std::vector есть внутренний член value_type. 
	
	Шаблонный класс Other<T> может быть инстанцирован при помощи std::vector<int>.
*/
/*
	***********************************************************************************
	4.1.8.2.3 Составные требования
	
	Составное требование имеет следующий вид
	
	 {expression} 		noexcept(optional) 	return-type-requirement(optional);
	 
	Вдобавок к простому требованию 
	составное требование также может содержать спецификатор noexcept
	и требование на тип возвращаемого значения.
	
	Концепт Equal, показанный в следующем примере, использует составные требования.
*/
template<typename T>
concept Equal = 	requires(T a, T b) {
						{ a == b } -> std::convertible_to<bool>;
						{ a != b } -> std::convertible_to<bool>;
					};

bool areEqual(Equal auto a, Equal auto b){
	return a == b;
}

struct WithoutEqual{
	bool 	operator==(const WithoutEqual& other) = delete;
};

struct WithoutUnequal{
	bool 	operator!=(const WithoutUnequal& other) = delete;
};

void test_15(){
	cout << "-------------- I'm void " << __func__ << "()" << endl;
	cout << std::boolalpha << '\n';
	
    cout << "areEqual(1, 5): " << areEqual(1, 5) << '\n';
	cout << "areEqual(15, 15): " << areEqual(15, 15) << '\n';
  
	/*
     bool res 		= areEqual(WithoutEqual(),  WithoutEqual());
	bool res2 	= areEqual(WithoutUnequal(),  WithoutUnequal());
    */
    cout << '\n';
}
/*
	 Концепт Equal требует, чтобы его параметр 'Т' 
	 поддерживал операторы равенства и неравенства. 
	 
	 Кроме того, оба этих оператора должны возвращать значение, конвертируемое в bool. 
	 
	 Конечно, тип int поддерживает концепт Equal, 
	 но это не относится к типам WithoutEqual и WithoutUnequal. 
*/
/*
	----------------------------------------------------------------------------------
	4.1.8.2.4 Вложенные требования
	
	Вложенные требования имеют следующий вид.
 
	requires		constraint-expression;
	
	Вложенные требования используются 
	для задания требований на параметры типа.
	
	Ниже приводится другой способ определения концепта UnsignedIntegral.

template <typename T>
concept Integral = std::is_integral<T>::value;

template <typename T>
concept SignedIntegral = Integral<T> && std::is_signed<T>::value;

// template <typename T>
// concept UnsignedIntegral = Integral<T> && !SignedIntegral<T>;
*/
template <typename T>
concept UnsignedIntegralExt = Integral<T> &&
	requires(T) {
		requires !SignedIntegral<T>;
	};

void test_16(){
	cout << "-------------- I'm void " << __func__ << "()" << endl;
	
    UnsignedIntegralExt auto n = 5u;  // works
    // UnsignedIntegralExt auto m = 5;   // compile time error, 5 is a signed literal
}
/*
	4.1.9.1.1 Концепт Ordering
	
template<typename T>
concept Equal = 	requires(T a, T b) {
						{ a == b } -> std::convertible_to<bool>;
						{ a != b } -> std::convertible_to<bool>;
					};
*/
template <typename T>
concept	Ordering	= 		Equal<T>
						&& 	requires(T a,T b){
								 {a <= b}	->	std::convertible_to<bool>;
								 {a <  b}	->	std::convertible_to<bool>;
								 {a >  b}	->	std::convertible_to<bool>;
								 {a >= b}	->	std::convertible_to<bool>;
							};
/*
	Концепт Ordering использует вложенные требования внутри себя. 
	
	Тип 'Т' поддерживает концепт Ordering, 
	если он поддерживает концепт Equal и, кроме того, 
	четыре оператора сравнения. 
	
	Давайте это проверим.
*/
template <Equal T>
bool 	areEqual(const T& a, const T& b) {
    return a == b;
}

template <Ordering T>
T 	getSmaller(const T& a, const T& b) {
    return (a < b) ? a : b;
}
    
void test_17(){
	cout << "-------------- I'm void " << __func__ << "()" << endl;
    cout << std::boolalpha << '\n';
  
    cout << "areEqual(1, 5): " 		<< areEqual(1, 5) 	<< '\n';
    cout << "getSmaller(11, 5): " 	<< getSmaller(11, 5) << '\n';
  
    std::unordered_set<int> firSet{1, 2, 3, 4, 5};
    std::unordered_set<int> secSet{5, 4, 3, 2, 1};
  
    cout << "areEqual(firSet, secSet): " << areEqual(firSet, secSet) << '\n';
  
    // auto smallerSet = getSmaller(firSet, secSet);
    cout << '\n';
}
/*
	На самом деле концепт Ordering уже является частью С++20.
	
	std::three_way_comparable: 			-	эквивалентно концепту Ordering, приведенному выше.
	
	std::three_way_comparable_with: 		-	позволяет осуществлять сравнения значений разных типов, 
										например 1.0 < 1.0f.
										
	В С++20 у нас есть 'оператор трехстороннего сравнения' <=>. 
	
	Более подробно мы рассмотрим его в соответствующей главе.
	
	-------------------------------------------------------------------------------------------------
	4.1.9.2 Концепты SemiRegular и Regular
	
	Когда вы хотите определить конкретный тип, который хорошо работает в экосистеме С++, 
	то вам нужно определить тип, который «ведет себя как int». 
	
	Формально ваш тип должен быть регулярным (regular). 
	
	И в этом разделе я определю типы SemiRegular и Regular.
	
	Типы SemiRegular и Regular являются очень важными концептами в С++. 
	
	Например, вот правило 
	
	Т.46 из «Руководящих принципов С++»: 
	
	"... требуйте, чтобы параметры шаблона были как минимум Regular или SemiRegular...". 
	
	Остается один важный вопрос: что это за типы такие, SemiRegular и Regular? 
	
	Прежде чем я уйду глубоко в детали, вот неформальный ответ:
	
	тип regular ведет себя как int. 
	
	Он может быть скопирован, 
	и результат операции копирования не зависит от исходного значения 
	и имеет то же самое значение.
	http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rt-regular.
	
	Отлично, давайте теперь это формализуем. 
	
	Также regular является semiregular, поэтому начнем.
	------------------------------------------------------------------------------------------
	Типы regular

	Александр Степанов, придумавший идею стандартной библиотеки шаблонов, 
	определил типы regular и semiregular. 
	
	Согласно этому, тип является regular, 
	если он поддерживает следующие функции:
	
	-	создание копии (copy construction);
	
	-	присваивание;
	
	-	равенство;
	
	-	уничтожение;
	
	-	полное упорядочение.
	
	Создание копии 
	подразумевает создание конструктора по умолчанию (default construction), 
	
	а равенство 
	подразумевает неравенство. 
	
	Когда Степанов определял эти требования, то в С++ еще не было семантики перемещения. 
	
	Книга «Элементы программирования», которую Александр Степанов написал вместе с Полом Мак Джонсом, 
	посвящена регулярным типам.
	
	-------------------------------------------------------------------------------------------------
	4.1.9.2.1 Концепт SemiRegular
	
	Тип X является semiregular, если он поддерживает большую шестерку функций (Big Six) 
	и операцию swap. Большая шестерка состоит из следующих функций:
	
	-	конструктор по умолчанию 		X();
	-	конструктор копирования 		X( const X& );
	-	оператор присваивания 		X& operator ( const X& );
	-	конструктор перемещения 		X( X&& );
	-	оператор перемещения 		X& operator=(X&&);
	-	деструктор 					~X().
	
	Дополнительно к этому 
	должна быть определена операция 	swap(X&, X&).
	
	Благодаря библиотеке свойств типов,
	определить соответствующий концепт легко. 
	
	Для начала я определю свойство типа isSemiRegular
	и затем использую его для определения концепта SemiRegular.
	
	std::integral_constant 	-	cоздает целочисленную константу из типа и значения.
	
	В нашем случае тип - bool, значение true или false.
*/
template<typename T>
struct	isSemiRegular	:	std::integral_constant	<	bool
													,		std::is_default_constructible<T>::value
														&&	std::is_copy_constructible<T>::value
														&&	std::is_copy_assignable<T>::value
														&&	std::is_move_constructible<T>::value
														&&	std::is_move_assignable<T>::value
														&&	std::is_destructible<T>::value
														&&	std::is_swappable<T>::value
													>
{};

template<typename T>
concept	SemiRegular	=	isSemiRegular<T>::value;
/*
	 Свойство типа isSemiRegular выполнено, 
	 когда выполнены все свойства, соответствующие большой шестерке, 
	 и свойство std::is_swappable. 
	 
	 Все, что после этого остается для определения концепта SemiRegular, – 
	 это использовать свойство типа isSemiRegular.
	
	//	-----------------------------------------------------------------------------------------
	4.1.9.2.2 Концепт Regular
 
	Остался еще один шаг, и мы закончим с пояснением концепта Regular. 
	
	В дополнение к свойствам концепта SemiRegular концепт Regular требует, 
	чтобы в типе было определено сравнение элементов на равенство. 
	
	Я уже определил концепт Equal ранее. 
	
	Теперь осталось только соединить концепты Equal и SemiRegular при помощи операции 'И'.
*/
template<typename T>
concept Regular = 
					Equal<T> && SemiRegular<T>;
/*
	-----------------------------------------------------------------------------------
	 4.1.9.2.3 std::semiregular и std::regular
 
	С++20 для построения std::semiregular и std::regular 
	использует свойства типов и ранее введенные концепты.
	
	Определение концептов std::semiregular и std::regular
*/
template<class T>
concept movable = 		std::is_object_v<T> 
					&& 	std::move_constructible<T> 
					&&	std::assignable_from<T&, T> 
					&& 	std::swappable<T>;
template<class T>
concept copyable = 		movable<T> 
					&& 	std::copy_constructible<T> 
					&&	std::assignable_from<T&, T&> 
					&&	std::assignable_from<T&, const T&> 
					&& 	std::assignable_from<T&, const T>;
					
template<class T>
concept semiregular = 	copyable<T> 
					&& 	std::default_initializable<T>;
 
template<class T>
concept regular = 		semiregular<T> 
					&& 	std::equality_comparable<T>;
/*
	Интересно то, что концепт std::regular определен аналогично Regular. 
	
	С другой стороны, концепт std::semi_regular построен из более базовых концептов, 
	таких как std::copyable и std::moveable. 
	
	Концепт std::moveable основан на свойствах типа std::is_object. 
	
	На сайте cppreference.com также приведена своя реализация предиката, выполняемого во время компиляции.
	
	Возможная реализация свойства типа std::is_object
*/	
template<class T>
struct is_object : std::integral_constant	<	bool
											,		std::is_scalar<T>::value 
												||	std::is_array<T>::value 
												||	std::is_union<T>::value 
												||	std::is_class<T>::value
											> 
{};				
/*
	'Тип' является 'объектом', 
	если он 
	-	скаляр, 
	-	массив, 
	-	объединение 
	или 
	-	класс. 

	В завершение этого раздела я хочу показать применение концептов 
	Regular 
	и 
	std::regular. 
	
	Для этого я использую программу regularSemiRegular.cpp.

template<typename T>
struct isSemiRegular: std::integral_constant<bool,
                                      std::is_default_constructible<T>::value &&
                                      std::is_copy_constructible<T>::value &&
                                      std::is_copy_assignable<T>::value &&
                                      std::is_move_constructible<T>::value &&
                                      std::is_move_assignable<T>::value &&
                                      std::is_destructible<T>::value &&
                                      std::is_swappable<T>::value >{};

template<typename T>
concept SemiRegular = isSemiRegular<T>::value;

template<typename T>
concept Equal =
    requires(T a, T b) {
        { a == b } -> std::convertible_to<bool>;
        { a != b } -> std::convertible_to<bool>;
};

template<typename T>                              
concept Regular = Equal<T> && 
                  SemiRegular<T>;
*/

template <Regular T>                              
void behavesLikeAnInt(T) {
	cout << "-------------- I'm template <Regular T> void " << __func__ << "(T)" << endl;
	//cout << "" << type_name_as_str<decltype(T)>() << endl;
	cout << "" << type_name_as_str<T>() << endl;
    // ...
}

template <std::regular T>                         
void behavesLikeAnInt2(T) {
	cout << "-------------- I'm template <std::regular T> void " << __func__ << "(T)" << endl;
	//cout << "" << type_name_as_str<decltype(T)>() << endl;
	cout << "" << type_name_as_str<T>() << endl;
    // ...
}

struct EqualityComparable {	
	EqualityComparable()	{
		cout << "-------------- I'm COTOR " << __func__ << "()" << endl;
	}
};                                                              

bool operator==	(	EqualityComparable const&
				,   EqualityComparable const&) { 
	cout << "-------------- I'm bool " << __func__ << "(EqualityComparable const&, EqualityComparable const&)" 
		 << endl;			
    return true; 
}

struct NotEqualityComparable {
	NotEqualityComparable()	{
		cout << "-------------- I'm COTOR " << __func__ << "()" << endl;
	}
};                 
//	Этот оператор превращает структуру NotEqualityComparable в структуру EqualityComparable 
bool operator==	(	NotEqualityComparable const&
				,   NotEqualityComparable const&) { 
	cout << "-------------- I'm bool " << __func__ 
	     << "(NotEqualityComparable const&, NotEqualityComparable const&)" 
		 << endl;			
    return true; 
}
void test_18(){
	cout << "-------------- I'm void " << __func__ << "()" << endl;

    int 						myInt{};
    behavesLikeAnInt(myInt);
    behavesLikeAnInt2(myInt);

    std::vector<int> 			myVec{};
    behavesLikeAnInt(myVec);
    behavesLikeAnInt2(myVec);

    EqualityComparable 			equComp;
    behavesLikeAnInt(equComp);
    behavesLikeAnInt2(equComp);
	
    NotEqualityComparable 		notEquComp;             
    behavesLikeAnInt(notEquComp);                  
    behavesLikeAnInt2(notEquComp);                 			
}
/*
	Я собрал здесь вместе все фрагменты из предыдущих примеров кода для определения концепта Regular. 
	
	Шаблонная функция behavesLikeAnInt() проверяет, 
	ведет ли себя ее аргумент как int. 
	
	Это значит, 
	что определенный пользователем концепт Regular 
	и концепт из C++20 std::regular 
	применяются для проверки условия. 
	
	Как подсказывает название, тип EqualityComparable поддерживает проверку на равенство, 
	но тип NotEqualityComparable – нет. 
	
	Имеется соответствующий оператор operator==().
	
	Использование типа NotEqualityComparable в обоих вызовах функций 
	является наиболее интересной частью этой программы.
	
	(Я добавил еще один оператор operator==(), 
	чтобы убедиться, что наличие такого оператора критично в данном случае).
	
	***************************************************************************************************
	Концепты в С++20: эволюция или революция?
	
	Следующий небольшой обзор выражает мое личное мнение. 
	
	Сначала я приведу факты, а потом сделаю из них выводы. 
	
	Факты основаны на том, что я представил в данной главе. 
	
	Итак, какие аргументы говорят за эволюцию и какие – за революцию?
	
	----------------------------------------------------------------------------------------------
	Эволюция
	
	-	Концепты продвигают работу с обобщенным кодом на более	высоком	уровне абстракции.
	
	-	Концепты дают вам понятные сообщения об ошибках, когда компиляция шаблона приводит к ошибке. 
	
	-	То, что они дают, нельзя достичь при помощи библиотеки свойств типов, 
		SFINAE2 и static_assert. 
	
	-	auto выступает в качестве заполнителя без каких-либо ограничений. 
	
	С С++20 вы можете использовать концепты как заполнители с ограничением.
	
	Начиная со стандарта С++14 вы можете использовать обобщенные лямбды 
	как удобный способ задания шаблонных функций.
	
	---------------------------------------------------------------------------------------------------
	Революция
 
	-	Концепты позволяют нам впервые проверять	требования шаблона. 
	
	-	Конечно, вы также могли достичь верификации параметров шаблона 
		при помощи комбинации библиотеки свойств типов, SFINAE и static_assert, 
		но это слишком сложно в качестве общего решения.
		
	-	Благодаря сокращенному синтаксису шаблонных функций 
		задание шаблонов стало гораздо проще.
		
	-	Концепты представляют собой семантические	категории, 
		а не синтаксические ограничения. 
		
		Вместо такого концепта, как Addable, требующего поддержки оператора +, 
		мы должны думать в категориях Number, 
		где Number – это семантическая категория, такая как Equal или Ordering.
 
	**************************************************************************************************
	Выводы
 
	Есть много споров о том, являются концепты эволюционным шагом или революционным прыжком вперед. 
	
	В основном из-за семантических категорий я на стороне революционного прыжка. 
	
	Такие концепты, как Number, Equality или Ordering, напоминают мне мир идей Платона. 
	
	Революционно то, как мы теперь можем думать о программировании в терминах этих категорий.
 
	------------------------------------------------------------------------------------------------------
	Важные замечания
	
	-	Использование функций или классов, определенных для заданного типа данных, 
		приводит к возникновению ряда проблем. 
		
		Концепты преодолевают эти проблемы, позволяя задавать семантические ограничения на параметры.
		
	-	Концепты могут быть использованы в директивах require 
		как параметры шаблона с ограничением 
		или же в сокращенной форме шаблонных функций.
		
	-	Концепты являются предикатами времени компиляции 
		и могут быть использованы в самых разных шаблонах. 
		
	-	Вы можете осуществлять перегрузку при помощи концептов, 
		специализировать шаблоны при помощи концептов, 
		использовать концепты для методов класса или шаблонов с переменным количеством аргументов.
		
	-	Благодаря С++20 и концептам 
		использование заполнителей без ограничений (auto) 
		и заполнителей с ограничением (концепты) 
		было унифицировано. 
		
		Везде, где вы применяете auto, вы можете использовать концепты.
		
	-	Благодаря новому сокращенному синтаксису для задания шаблонов 
		задание шаблонной функции стало очень простым и красивым.
		
	Не изобретайте колесо. 
	
	Прежде чем определять свои собственные концепты, 
	изучите богатый набор предопределенных концептов в стандарте С++20. 
	
	При определении своих собственных концептов вы можете применять два приема: 
	
	1.	комбинировать концепты и предикаты времени компиляции 
	
	и 
	
	2.	использовать директиву requires.
	
	
*/
