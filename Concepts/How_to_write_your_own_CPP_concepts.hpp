#pragma once
/*
	How_to_write_your_own_CPP_concepts.hpp
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
#include <ranges>
#include <typeinfo> 

// Для clang и gcc математические библиотеки в разных заголовках описаны
#if defined(__clang__)
	#include <math.h>
#else	
	#include <cmath>
#endif



#include "..\type_name_as_str.h"
/*	
	***********************************************************************************************
	Как написать свои собственные концепции C++? 
	Шандор Дарго 10 марта 2021 г.
	https://www.sandordargo.com/blog/2021/03/10/write-your-own-cpp-concepts-part-i
	https://www.sandordargo.com/blog/2021/03/17/write-your-own-cpp-concepts-part-ii
	--------------------------------------------------------------------------------------------
	Часть 1.
	--------------------------------------------------------------------------------------------
	Мы обсуждали мотивы концепций C++ и способы их использования с функциями и классами . 
	
	Но мы почти ничего не написали. 
	
	Мы определили функционально неполную концепцию, названную Number ради примера, на этом всё. 
	
	Теперь мы подробно рассмотрим, какие ограничения мы можем выразить в концепции.

	В этом разделе мы начнем с простых концепций, объединяющих существующие, 
	а затем закончим необходимыми операциями и общими требованиями к API класса.

	Далее я покажу вам, как писать требования к возвращаемым типам, 
	как выражать требования к типам и как вкладывать ограничения.
	--------------------------------------------------------------------------------------------------------
	
	Простейший concept

	Давайте сначала определим самую простую концепцию, 
	которую мы можем себе представить, 
	просто чтобы увидеть синтаксис.
*/
template<typename T> 
	concept Any = 
					true;
/*
	Сначала мы перечисляем 'параметры шаблона', в данном случае у нас есть только один, 'T'
	но мы можем иметь несколько параметров, разделенных запятыми. 
	
	Затем после 'ключевого слова' concept,
	мы объявляем 'имя концепции', 
	
	а затем после 'ключевого слова' '='
	мы определяем концепцию (тело концепта).

	В этом примере мы просто говорим true, подразумевая, что для любого типа 'T'
	концепция будет оценена как true; 
	принимается любой тип. 
	
	Если бы мы написали false, ничего бы не было принято.

	Теперь, когда мы рассмотрели простейшую концепцию, 
	давайте проверим, 
	какие 'строительные блоки' имеются в нашем распоряжении 
	для построения более подробной концепции.

	---------------------------------------------------------------------------------------------------------
	Используйте уже определенные концепции

	Вероятно, самый простой способ определить новые концепции — 
	это объединить существующие.

	Например, в следующем примере мы собираемся снова создать концепцию, 
	вызываемую Number 
	как целыми числами, так и числами с плавающей запятой.
*/
template<typename T> 
concept NumberConcept_1 = 
					std::integral<T> || std::floating_point<T>;
/*
	Как вы можете видеть в приведенном выше примере, 
	мы могли бы легко объединить с || оператором две концепции. 
	
	Конечно, мы можем использовать любой логический оператор.

	Возможно, это самоочевидно, но мы также можем использовать концепции, определяемые пользователем.
*/
template<typename T> 
concept IntegerConcept = 
					std::integral<T>;
template<typename T> 
concept FloatConcept 	= 
					std::floating_point<T>;
					
//	Концепт как дизъюнкция двух пользовательских концептов
template<typename T> 
concept NumberConcept_2 	
				= IntegerConcept<T> || FloatConcept<T>;
/*
	В этом примере мы просто использовали псевдонимы 
	(и добавили слой косвенности) std::integral, std::floating_point
	чтобы показать, что определяемые пользователем концепции 
	также могут использоваться в комбинации концепций.

	Как мы видели ранее , 
	в различных заголовках стандартной библиотеки определено множество концепций, 
	поэтому существует бесконечное количество способов их объединения.

	Но как определить действительно уникальные концепции?
	
	----------------------------------------------------------------------------------------------------
	Напишите свои собственные ограничения
	
	В следующих разделах мы собираемся углубиться в то, 
	как выразить наши собственные уникальные требования 
	без использования каких-либо 'предопределенных концепций'.

	----------------------------------------------------------------------------------------------------
	Требования к операциям

	Мы можем просто выразить, что нам требуется, 
	чтобы параметр шаблона поддерживал определенную операцию или оператор, 
	написав желаемое за действительное .

	Если вам требуется, 
	чтобы параметры шаблона можно было добавлять, 
	вы можете создать для этого концепцию:
*/
template <typename T>
concept AddableConcept = 
						requires (T a, T b) {
							a + b; 
						};	// Требуется, чтобы для 'а' и 'b' была определена операция сложеня '+'
auto addTwoVariables(AddableConcept auto x, AddableConcept auto y) {
  return x + y;
}
struct WrappedInt {
	WrappedInt operator+(WrappedInt const& rhs)   {
        WrappedInt result;
        result.m_int = m_int + rhs.m_int;
        return result;
    }
	int m_int;
};
void test_14() {
	cout << "-------------- I'm void " << __func__ << "()" << endl;

	cout << addTwoVariables(4, 5) << '\n';
	cout << addTwoVariables(true, true) << '\n';
	
	//	cout << addTwoVariables(WrappedInt{4}, WrappedInt{5})<< '\n'; 	
	// 	error: use of function 'auto addTwoVariables(auto:11, auto:12) 
	//	[with auto:11 = WrappedInt; auto:12 = WrappedInt]' with unsatisfied constraints
	//  После того, как я определил в классе WrappedInt оператор сложения operator+(), 
	//	все стало соответствовать концепту
	cout << addTwoVariables(WrappedInt{4}, WrappedInt{5}).m_int << '\n'; 
}
/*
	Мы можем наблюдать, что при addTwoVariables() вызове с параметрами типа WrappedInt 
	(поскольку они не поддерживаются operator+) компиляция завершается неудачей 
	с довольно описательным сообщением об ошибке 
	(не все сообщение об ошибке копируется в приведенный выше пример).

	Написать Addable-концепцию кажется довольно простым, не так ли? 
	
	После ключевого слова requires мы в основном записали, какой синтаксис мы собираемся скомпилировать и запустить.

	-----------------------------------------------------------------------------------------------------------------
	Простые требования к интерфейсу
	
	Давайте еще немного подумаем об операциях. 
	
	В конце концов, что значит требовать поддержки операции сложения '+'?

	Это означает, что мы ограничиваем допустимые типы теми, 
	которые имеют функцию-функцию 
	T T::operator+(const T& other) const. 
	
	Или даже может быть 
	T T::operator+(const U& other) const, 
	так как, возможно, мы хотим добавить к экземпляру другого типа, 
	но здесь дело не в этом. 
	
	Я хочу сказать, что мы выдвинули требование о наличии конкретной функции.

	Итак, мы должны иметь возможность определить требование для любого вызова функции, не так ли?

	Хорошо, давайте посмотрим, как это сделать.
*/
template <typename T> // 2
concept HasSquare = 	
					requires (T t) {
						t.square();
					};
class IntWithoutSquare {
public:
	IntWithoutSquare(int num) : m_num(num) {}
private:
	int m_num;
};
class IntWithSquare {
public:
	IntWithSquare(int num) : m_num(num) {}
	int square() {
		return m_num * m_num;
	}
private:
	int m_num;
};

void printSquare(HasSquare auto number) { // 1
  std::cout << number.square() << '\n';
}

void test_15() {
	cout << "-------------- I'm void " << __func__ << "()" << endl;

	//	printSquare(IntWithoutSquare{4}); 
	// 	error: use of function 'void printSquare(auto:11) [with auto:11 = IntWithoutSquare]' with unsatisfied constraints, 
    // 	the required expression 't.square()' is invalid
	printSquare(IntWithSquare{5});
}
/*
	В этом примере у нас есть функция printSquare(1), 
	которой требуется параметр, удовлетворяющий концепции HasSquare(2). 
	
	В этой концепции мы видим, что очень легко определить, 
	какой интерфейс мы ожидаем. 
	
	После ключевого слова requires мы должны написать, 
	какие вызовы должен поддерживать интерфейс принимаемых типов.

	Наши ожидания пишутся после ключевого слова requires. 
	
	Во-первых, в круглых скобках есть список параметров — 
	для функции — 
	где мы должны перечислить все параметры шаблона, 
	которые будут ограничены, 
	и любые другие параметры, которые могут появиться в ограничениях. 
	
	Подробнее об этом позже.

	Если мы ожидаем, что любой переданный тип будет вызывать функцию square(), 
	нам просто нужно написать 
	
	(T t) { t.square(); }. 
	
	(T t)		-	потому что мы хотим определить ограничение для экземпляра типа шаблона 'T' 
	и 
	t.square() 	-	потому что мы ожидаем, что 't' экземпляр типа 'T'
					'должен иметь' 'открытую функцию' square().

	Если у нас есть требования к достоверности вызовов нескольких функций, 
	нам просто нужно перечислить их все, 
	разделенные точкой с запятой, как если бы мы вызывали их один за другим:

template <typename T>
concept HasSquare = requires (T t) {
  t.square();
  t.sqrt();
};

	А как насчет параметров? 
	
	Давайте определим power() функцию, 
	которая принимает int параметр для показателя степени:

template <typename T>
concept 	HasPower = 	
						requires (T t, int exponent) {
							t.power(exponent);
						};
void 	printPower(HasPower auto number) {
	cout << number.power(3) << '\n';
}

	Переменная exponent, которую мы передаем функции T::power(),
	должна быть указана после ключевого слова requires с ее типом, 
	а также типом(ами) шаблона, который мы ограничиваем. 
	
	Таким образом, мы исправим ситуацию, 
	когда параметр будет чем-то, что (конвертируется) в int.

	Но что, если бы мы захотели принять в качестве показателя степени любое целое число? 
	
	Где воля, там и способ! 
	
	Во-первых, наш концепт HasPower должен принимать два параметра. 
	
	Один для базового типа и один для экспонентного типа.
	
template <typename Base, typename Exponent>
concept HasPowerExtra = 
						std::integral<Exponent> && 	requires (Base base, Exponent exponent) { 
														base.power(exponent);
													};
	Мы убеждаемся, что тип шаблона Exponent является целым, 
	и что его можно передать Base::power() в качестве параметра.

	Следующим шагом будет обновление нашей printPower() функции. 
	
	Концепция HasPower изменилась, 
	теперь она имеет два типа, 
	соответственно придется внести некоторые изменения:

template<typename Exponent>
void printPower(HasPowerExtra<Exponent> auto number, Exponent exponent) {
	cout << number.power(exponent) << '\n';
}

	Поскольку это явно указано в качестве параметра типа шаблона Exponent, 
	ключевое слово auto после него не требуется . 
	
	С другой стороны, auto нужно после HasPowerExtra<Exponent>, 
	иначе как бы мы узнали, 
	!!! что это концепт, а не конкретный тип?! 
	
	Exponent передается как параметр типа шаблона, 
	HasPowerExtra<Exponent> чтобы к нему также применялись ограничения.

	Теперь printPower можно вызвать следующим образом — 
	учитывая, что мы переименовали его IntWithSquare, 
	IntWithPower следуя изменениям нашего API:

printPower(IntWithPower{5}, 3);
printPower(IntWithPower{5}, 4L);

	В то же время вызов 
	
printPower(IntWithPower{5}, 3.0);
	
	завершится неудачей, поскольку тип float не удовлетворяет ограничению целостности показателя степени.

	Мы что-то упускаем? 
	
	Да! 
	
	Мы не можем использовать IntWithPower в качестве показателя степени. 
	
	Мы хотим иметь возможность вызывать Base::power(Exponent exp) с пользовательским типом, 
	таким как, например, IntWithPower, и для этого нам нужны две вещи:
	
	-	IntWithPower	следует рассматривать как integralтип

	-	IntWithPower	должно быть конвертировано во что-то, принятое powиз cmath-заголовка.

	Давайте по одному.

	Явно указав type_trait std::is_integral для IntWithPower, 
	мы можем создать целочисленный тип IntWithPower. 
	
	Конечно, если мы планируем сделать это в реальной жизни, 
	лучше убедиться, что наш тип обладает всеми характеристиками целочисленного типа, 
	но это выходит за рамки наших задач. 
	
	Обновление: специализация большинства характеристик (свойств) типа приводит к неопределенному поведению, 
	поэтому не делайте этого в рабочем коде!!!!!!!

template<>
struct std::is_integral<IntWithPower> : public std::integral_constant<bool, true> {};

class IntWithPower {
public:
	IntWithPower(int int_value_) : int_value(int_value_)	{
		cout << "-------------- I'm COTOR " << __func__ << "()" << endl;
	}
	int power(IntWithPower exponent) {
		return pow(int_value, exponent);
	}
	
	operator int() const { return int_value; }
//private:
	int		int_value;
};

	Теперь нам нужно убедиться, что IntWithPower можно преобразовать в тип, 
	принимаемый pow(). 
	
	Он принимает типы с плавающей запятой, 
	но когда дело доходит до IntWithPower, 
	на мой взгляд, 
	более разумно преобразовать его в int 
	и позволить компилятору выполнить неявное преобразование в float - 
	хотя лучше вообще избегать неявных преобразований. 
	
	Но в конце концов, IntWithPower можно использовать и в других контекстах - как целое число.

	Для этого нам необходимо определить operator int():

class IntWithPower {
public:
	IntWithPower(int num) : m_num(num) {}
	int power(IntWithPower exp) {
		return pow(m_num, exp);
	}
	operator int() const { return m_num; }
private:
	int m_num;
}

	Если мы проверим наш пример сейчас, мы увидим, 
	что оба варианта 
	
	printPower(IntWithPower{5}, IntWithPower{4});
	и 
	printPower(IntWithPower{5}, 4L);
	
	скомпилируются, но 
	
	printPower(IntWithPower{5}, 3.0);
	
	не будут выполнены, поскольку 3.0 не являются целочисленными.

	Правильно, как мы только что сказали, pow() работает с числами с плавающей запятой, 
	но мы принимаем только целые числа. 
	
	Давайте обновим нашу концепцию соответствующим образом!

template <typename Base, typename Exponent>
concept HasPower = 		(std::integral<Exponent> || std::floating_point<Exponent>) 
					&& 	requires (Base base, Exponent exponent) {     
									base.power(exponent);
						};
						
	Теперь мы можем вызывать printPower любой тип base, 
	который удовлетворяет HasPower концепции, 
	а также целые числа и числа с плавающей запятой в качестве показателя степени.

	Давайте теперь посмотрим на полный пример:
*/
template <typename Base, typename Exponent>
concept HasPower = 		(std::integral<Exponent> || std::floating_point<Exponent>) 
					&& 	requires (Base base, Exponent exponent) { 
											base.power(exponent);
						};
//	-------------------------------------------------------

class IntWithPower {
public:
	IntWithPower(int num) : m_num(num) 
	{}
	int power(int exp) {
		#if defined(__clang__)
			return powf(m_num, exp);		
		#else	
			return pow(m_num, exp);		// std::pow - НЕ КАТИТ!!!!!!! а clang++ не хавает ни так, ни так
		#endif
	}
	operator int() const {return m_num;}
private:
	int m_num;
};

template<>
struct std::is_integral<IntWithPower> : public std::integral_constant<bool, true> 
{};

template<typename Exponent> 
void printPower(HasPower<Exponent> auto number, Exponent exponent) {
	cout << number.power(exponent) << '\n';
}
void test_16() {
	cout << "-------------- I'm void " << __func__ << "()" << endl;
	//printPower(IntWithPower{5}, IntWithPower{4});
	printPower(IntWithPower{5}, 4L);
	printPower(IntWithPower{5}, 3.0);
}
/*
	В этом примере мы можем наблюдать, как написать концепцию, 
	предполагающую наличие определенной функции, 
	способной принимать параметр разных ограниченных типов. 
	
	Мы также можем увидеть, 
	как создать тип, 
	удовлетворяющий встроенным признакам типа, 
	например std::is_integral.

	*************************************************************************************************
	Заключение

	Сегодня мы начали узнавать, как писать свои собственные концепции. 
	
	Сначала мы объединили уже существующие концепции в более сложные, 
	затем продолжили выдвигать требования к валидности операций над ограниченными типами, 
	а затем закончили написанием требований для любого вызова функции со списком параметров или без него.
*/
/*
	***************************************************************************************************************
	Часть 2.
	-------------------------------------------------------------------------------------------------------
	Требования к типам возвращаемых данных (также известные как сложные требования)

	Мы видели, как написать требование, 
	выражающее потребность в определенном API определенной функции.

	Но ограничили ли мы также тип возвращаемого значения этих функций?

	Нет, мы этого не сделали. 
	
	IntWithSquare удовлетворяет HasSquare-концепции как с int square(), таки с void square().

	Если вы хотите указать 'тип возвращаемого значения', 
	вы должны использовать так называемое "составное требование".

	Вот пример:
*/
template <typename T>
concept HasSquareEx = 	
						requires (T t) {
							{ t.square() } -> std::convertible_to<int>;
						}; 
/*
	Обратите внимание на следующее:

	Выражение того, что вы хотите установить требование типа возвращаемого значения, 
	должно быть заключено в {фигурные скобки}, затем идет стрелка (->), 
	за которой следует ограничение типа возвращаемого значения.
	
	Ограничение не может быть просто типом. 
	
	Если бы вы написали просто int, 
	вы бы получили сообщение об ошибке: 
	требование возвращаемого типа не является ограничением типа. 
	
	Исходные концепции TS допускали прямое использование типов, 
	поэтому, если вы поэкспериментировали с этим, 
	вы можете быть удивлены этой ошибкой. 
	
	Эта возможность была устранена в P1452R2 .

	Для такого удаления есть ряд причин. 
	
	Одним из мотивов было то, 
	что это помешало бы будущему желанию принять обобщенную форму auto, 
	подобную vector<auto> или vector<Concept>.

	Таким образом, вместо того, чтобы просто назвать тип, 
	вам нужно выбрать концепцию! 
	
	Если вы хотите установить тип возвращаемого значения, 
	вам подойдет один из двух следующих вариантов:

{ t.square() } -> std::same_as<int>;
{ t.square() } -> std::convertible_to<int>;

	Я думаю, что разница очевидна. 
	
	В случае std::same_as, 
	возвращаемое значение должно быть таким же, 
	как указано в аргументе шаблона, 
	
	при std::convertible_to разрешены преобразования.

	Чтобы продемонстрировать это, давайте посмотрим на следующий пример:
*/
template <typename T>
concept HasIntSquareRet = 	
						requires (T t) {
								{t.square()} -> std::same_as<int>;
						};

template <typename T>
concept HasConvertibleToIntSquare 	= 
									requires (T t) {
										{t.square()} -> std::convertible_to<int>;
									};

class IntWithIntSquare {
public:
	IntWithIntSquare(int num) : m_num(num) {}
	int square() const {
		return m_num * m_num;
	}
private:
	int m_num;
};

class IntWithLongSquare {
public:
	IntWithLongSquare(int num) : m_num(num) {}
	long square() const {
		return m_num * m_num;
	}
private:
	int m_num;
};

class IntWithVoidSquare {
public:
	IntWithVoidSquare(int num) : m_num(num) {}
	void square() const {
		cout << m_num * m_num << '\n';
	}
private:
	int m_num;
};

void printSquareSame(HasIntSquareRet auto number) {
	cout << number.square() << '\n';
}

void printSquareConvertible(HasConvertibleToIntSquare auto number) {
	cout << number.square() << '\n';
}

void test_17() {
	cout << "-------------- I'm void " << __func__ << "()" << endl;

	printSquareSame(IntWithIntSquare{1}); 			// int same as int
	//   printSquareSame(IntWithLongSquare{2}); 			// long not same as int
	//   printSquareSame(IntWithVoidSquare{3}); 			// void not same as int
	
	printSquareConvertible(IntWithIntSquare{4}); 	// int convertible to int
	printSquareConvertible(IntWithLongSquare{5}); 	// long convertible to int
	//   printSquareConvertible(IntWithVoidSquare{6}); 		// void not convertible to int
}
/*
	В приведенном выше примере мы можем наблюдать, что класс с void square() const
	не удовлетворяет ни концепции HasIntSquare, ни HasConvertibleToIntSquare.

	IntWithLongSquare, поэтому класс с функцией long square() const не удовлетворяет концепции, 
	HasIntSquareпока он не совпадает с int, но он удовлетворяет HasConvertibleToInt Square-концепции, 
	поскольку long может быть преобразован в int.

	Класс IntWithIntSquare удовлетворяет обеим концепциям, поскольку int очевидно, аналогичен int.

	--------------------------------------------------------------------------------------------------
	Требования к типу
	
	С помощью требований к типу мы можем выразить, 
	что определенный тип действителен в определенном контексте. 
	
	Требования к типу могут использоваться для проверки того, что

	-	существует определенный вложенный тип
	
	-	специализация шаблона класса называет тип
	
	-	специализация шаблона псевдонима называет тип

	Вы должны использовать ключевое слово typename вместе с именем типа, 
	который, как ожидается, будет существовать:

	
template<typename T>
concept TypeRequirement = 	requires {
								typename T::value_type;
								typename Other<T>;
							};

	Концепция TypeRequirement требует, чтобы тип 'T' имел вложенный тип value_typeи 
	чтобы шаблон класса Other мог быть создан с помощью 'T'.

	Давайте посмотрим, как это работает:
*/
template <typename>
struct Other;

template<typename T>
concept TypeRequirement = 	requires {
								typename T::value_type;
								typename Other<T>;
							};

void test_18() {
	cout << "-------------- I'm void " << __func__ << "()" << endl;

	TypeRequirement 	auto 	myVec = std::vector<int>{1, 2, 3};
	// TypeRequirement auto myInt {3}; 
	// error: deduced initializer does not satisfy placeholder constraints ... the required type 'typename T::value_type' is invalid 
}
/*
	Выражение TypeRequirement auto myVec = std::vector<int>{1, 2, 3} допустимо.

	A std::vector имеет внутренний тип члена value_type, 
	а шаблон класса Other может быть создан с помощью std::vector<int>.
	
	В то же время у int нет какого-либо члена, 
	в частности value_type, 
	поэтому он не удовлетворяет ограничениям TypeRequirement.

	Давайте изменим шаблон класса Other и сделаем требование к параметру шаблона, 
	убедившись, что его Other экземпляр не может быть создан с помощью vector<int>.

template <typename T>
	requires (!std::same_as<T, std::vector<int>>)
struct Other;

	Now, the line TypeRequirement auto myVec = std::vector<int>{1, 2, 3}; fails with the following error message:

	main.cpp: In function 'int main()':
	main.cpp:16:55: error: deduced initializer does not satisfy placeholder constraints
	16 |   TypeRequirement auto myVec = std::vector<int>{1, 2, 3};
      |                                                       ^
	main.cpp:16:55: note: constraints not satisfied
	main.cpp:10:9:   required for the satisfaction of 'TypeRequirement<std::vector<int, std::allocator<int> > >'
	main.cpp:10:27:   in requirements  [with T = std::vector<int, std::allocator<int> >]
	main.cpp:12:12: note: the required type 'Other<T>' is invalid
	12 |   typename Other<T>;
     |   ~~~~~~~~~^~~~~~~~~
	cc1plus: note: set '-fconcepts-diagnostics-depth=' to at least 2 for more detail

	With type requirements, we can make sure that a class has a nested member type 
	or that a template specialization is possible.

	To show that a concept can be used to prove that an alias template specialization names a type, 
	let's take our original example and create a template alias Reference:

template<typename T> using Reference = T&;

	And use it in the concept TypeRequirement:

template<typename T>
concept TypeRequirementExt = 
							requires {
								typename T::value_type;
								typename Other<T>;
								typename Reference<T>;
							};

	Our example should still compile:
*/
template <typename>
struct Other;

template<typename T> using Reference = T&;

template<typename T>
concept TypeRequirementExt = requires {
								  typename T::value_type;
								  typename Other<T>;
								  typename Reference<T>;
							};
void test_19() {
	cout << "-------------- I'm void " << __func__ << "()" << endl;

	TypeRequirement auto myVec = std::vector<int>{1, 2, 3};
}
/*
	-----------------------------------------------------------------------------------------------------------
	Вложенные требования
	
	Мы можем использовать вложенные требования 
	для указания дополнительных ограничений в концепции 
	без введения других именованных концепций.

	Вы можете думать о вложенных требованиях так же, 
	как о лямбда-функциях для алгоритмов STL. 
	
	Вы можете использовать лямбды для изменения поведения алгоритма 
	без необходимости называть функцию или объект функции.

	В этом случае вы можете написать ограничение, 
	более подходящее для ваших нужд, 
	без необходимости называть еще одно ограничение, 
	которое вы будете использовать только в одном (вложенном) контексте.

	Его синтаксис имеет следующую форму:
	
	requires constraint-expression;

	Let's start with a simpler example. 
	
	Where the concept Coupe uses two other concepts Car and Convertible.
*/
struct AwesomeCabrio {
	void openRoof(){}
	void startEngine(){}
};
struct CoolCoupe {
    void startEngine(){}
};
template<typename C>
concept CarExt = 	requires (C car) {
						car.startEngine();
					};

template<typename C>
concept Convertible = CarExt<C> && requires (C car) {
									car.openRoof();
								};

template<typename C>
concept Coupe = CarExt<C> && 	requires (C car) {
								requires !Convertible<C>;
							};

void test_20() {
	cout << "-------------- I'm void " << __func__ << "()" << endl;

	Convertible auto cabrio = AwesomeCabrio{};
	//Coupe auto notACoupe = AwesomeCabrio{}; // nested requirement '! Convertible<C>' is not satisfied
	Coupe auto coupe = CoolCoupe{};
}
/*
	Давайте посмотрим на концепцию Coupe. 
	
	Во-первых, мы убеждаемся, что в Car принимаются только типы, 
	удовлетворяющие концепции. 
	
	Затем мы вводим концепцию вложенности, 
	которая требует, 
	чтобы тип нашего шаблона не был расширением Convertible.

	Это правда, что нам не нужно вложенное ограничение, 
	мы могли бы выразить себя без него:

template<typename C>
concept Coupe = Car<C> && !Convertible<C>;

	Тем не менее, синтаксис мы увидели на рабочем примере.

	Вложенные предложения require можно более эффективно использовать с локальными параметрами, 
	перечисленными во внешней requires-области, как в следующем примере C clonable:
*/
struct Droid {
	Droid clone(){
		return Droid{};
	}
};
struct DroidV2 {
	Droid clones(){
		return Droid{};
	}
};

template<typename C>
concept Clonable = 	requires (C clonable) {
						clonable.clone();
						requires std::same_as<C, decltype(clonable.clone())>;
					};

void test_21() {
	cout << "-------------- I'm void " << __func__ << "()" << endl;

	Clonable auto c = Droid{};
	// 	Clonable auto c2 = DroidV2{}; 
	// 	nested requirement 'same_as<C, decltype (clonable.clone())>' is not satisfied
}
/*
	В этом примере у нас есть два типа дроидов: 
	
	Droid и DroidV2. 
	
	Мы ожидаем, что дроидов можно клонировать, 
	то есть каждый тип должен иметь метод клонирования, 
	который возвращает другого дроида того же типа. 
	
	Мы c DroidV2 допустили ошибку, и она все равно возвращается Droid.

	Можем ли мы написать концепцию, которая отловит эту ошибку?

	Мы можем, на самом деле, как вы, наверное, заметили, мы уже это сделали. 
	
	В концепции Clonable мы работаем с C cloneable локальным параметром. 
	
	С помощью вложенного требования requires std::same_as<C, decltype(clonable.clone())>
	мы выражаем, что метод клонирования должен возвращать тот же тип, что и параметр.

	Вы можете возразить, что есть другой способ выразить это, без вложенного предложения, и будете правы:

template<typename C>
concept Clonable = requires (C clonable) {
    { clonable.clone() } -> std::same_as<C>;
};
	Для более сложного примера я бы рекомендовал вам проверить реализацию SemiRegular концепций в C++ Reference .

	Чтобы включить одно из требований в Semiregular нашу Clonable концепцию, 
	мы могли бы написать следующее:

template<typename C>
concept Clonable = requires (C clonable) {
    { clonable.clone() } -> std::same_as<C>;
    requires std::same_as<C*, decltype(&clonable)>;
};
	
	Эта дополнительная строка гарантирует, что адрес оператора (&) 
	возвращает тот же тип параметра, cloneable что и C*.
	
	Я согласен, в данном контексте это не имеет особого смысла (имеет смысл для SemiRegular), 
	но, наконец, это пример, который не легче выразить без вложенного требования, чем с его помощью.

	В следующем посте мы увидим, 
	как использовать вложенное требование, когда даже включающее его понятие не имеет имени.

	----------------------------------------------------------------------------------------------
	Заключение

	Сегодня мы продолжили и закончили обсуждение того, 
	какие строительные блоки доступны нам для написания наших собственных концепций. 
	
	Мы увидели, как налагать ограничения на типы возвращаемых функций, 
	как использовать требования к типам для внутренних типов, 
	псевдонимы шаблонов и специализации, 
	и, наконец, 
	мы увидели, что можно вкладывать требования, хотя часто существуют более простые способы выразить себя.
*/	
//	-----------------------------------------------------------------------------------------------------------