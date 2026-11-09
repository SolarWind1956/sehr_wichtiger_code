//	concepts.cpp
/*
	ИИ ============================================================
	std::equality_comparable - это стандартный концепт в C++20 (объявленный в заголовке <concepts>), 
	который проверяет, поддерживает ли тип `T` операции сравнения на равенство (==) и неравенство (!=), 
	и гарантирует, что эти сравнения ведут себя математически корректно. 
	[1] (https://en.cppreference.com/cpp/concepts/equality_comparable), 
	[2] (https://cppreference-45864d.gitlab-pages.liu.se/en/cpp/concepts/equality_comparable.html)
	
	Проще говоря, этот концепт требует, чтобы вы могли написать a == b и a != b, 
	а результат выражения отражал истинное равенство объектов. 
	

	Основные требования к типу
	
	Чтобы тип удовлетворял концепту std::equality_comparable, 
	он должен соответствовать синтаксическим и семантическим правилам:
	
	Синтаксис: 	Выражения a == b и a != b должны быть валидными, 
				а их результат должен приводиться к типу `bool`. 

	Сохранение равенства (Equality Preserving): 	Если входные данные одинаковы, 
											то и результат сравнения всегда должен быть одинаковым. 
											
											Сравнение не должно менять состояние объектов. 
	
	Математические свойства:
	
		Рефлексивность: 		a == a всегда возвращает `true`.
		
		Симметричность: 		Если a == b, то и b == a.
		
		Транзитивность: 		Если a == b и b == c, то a == c. 
		
	Пример использования
	
	Концепты чаще всего применяются для ограничения параметров шаблонов 
	(через ключевое слово `requires` или `прямо в объявлении типа`).	
*/	
#include <iostream>
using std::cout; using std::endl; using std::flush;  using std::cerr;

#include <concepts>

// 	Функция принимает только те типы, которые можно сравнивать на равенство
template <std::equality_comparable T>
auto is_equal(const T& a_par, const T& b_par) ->bool {
    return a_par == b_par; 
}

struct Point {
    int x;
    int y;

    // 	Достаточно определить operator==, 
	//	а operator!= 
	//	C++20 сгенерирует автоматически
    auto operator==(const Point&) const ->bool = default;
};

struct NoEquality {
    int data;
};

void concepts_example_1() 
{
    Point 		point1{.x=1, .y=2};
	Point		point2{.x=1, .y=2};
	
    [[maybe_unused]] 	NoEquality 	noeq1{1};
	[[maybe_unused]]	NoEquality 	noeq2{1};

    cout << std::boolalpha;
    cout << is_equal(5, 5) << "\n";     // Отлично: int поддерживает ==
    cout << is_equal(point1, point2) << "\n";   // Отлично: Point имеет дефолтный operator==

    // 	СЛЕДУЮЩАЯ СТРОКА ВЫЗОВЕТ ОШИБКУ КОМПИЛЯЦИИ:
    // is_equal(noeq1, noeq2); 
    // Ошибка: NoEquality не удовлетворяет концепту std::equality_comparable
}
/*
	ИИ ==============================================================
	Родственные концепты
	
	В стандартной библиотеке есть расширенная версия этого концепта:
	std::equality_comparable_with<T, U> 
	- проверяет возможность сравнивать на равенство объекты разных типов 
	(например, std::string и const char*, или int и double). 
	
	Она требует, чтобы результат сравнения разнородных типов 
	совпадал с результатом `после их приведения к общему типу`. 

	------------------------------------------------------------------------------------------------------------------------------------------------------
	B C++20 есть четыре аналогичных концепта 
	для проверки операций 
	«меньше» (<), «больше» (>), «меньше или равно» (<=) и «больше или равно» (>=). 
	
	Все они находятся в заголовочном файле <concepts>.
	
	Они делятся на две категории: 
	
	-	для сравнения объектов одного типа 
	и 
	-	для сравнения объектов разных типов.
	
	1. 	Для объектов `одного типа`
		
		std::totally_ordered
		
		Это главный концепт, который, скорее всего, нужен. 
		
		Он проверяет, что тип поддерживает все четыре оператора отношения 
		(<, >, <=, >=), 
		а также равенство (==, !=).
		
		Математическое требование: 	Отношение должно быть строгим слабым упорядочением 
									(strict weak ordering). 
									
									Это значит, что любые два элемента можно сравнить между собой, 
									и это сравнение транзитивно 
									(если \(a < b\) и \(b < c\), то \(a < c\)).
									
		std::three_way_comparable
		
		Этот концепт проверяет, поддерживает ли тип новый `трехсторонний оператор сравнения` 
		`<=>` (spaceship operator) 
		и возвращает ли он валидный тип категории сравнения 
		(например, std::strong_ordering или std::partial_ordering).
		
	2. 	Для объектов `разных типов` (`разнородные сравнения`)
	
		Если нужно проверить, можно ли сравнивать между собой, 
		например, int и double, или std::string и const char*, 
		используются версии с суффиксом `_with`:
		
		2.1.
		std::totally_ordered_with<T, U>
		
		Проверяет, что объекты типов `T` и `U` 
		можно сравнивать друг с другом во всех комбинациях 
		(t < u, u < t, t <= u и т.д.), 
		и результаты будут математически согласованы.
		
		2.2.
		std::three_way_comparable_with<T, U>
		
		Проверяет, поддерживает ли выражение `t <=> u` (и `u <=> t`) 
		корректное трехстороннее сравнение.
		
	Пример использования std::totally_ordered	
*/
#include <vector>
#include <algorithm>

// 	Функция принимает только те типы, 
//	которые можно строго упорядочить 
//	(сравнивать на <, >, <=, >=)
template <std::totally_ordered T>
auto find_min(const T& a_par, const T& b_par) -> const T& {
    return (a_par < b_par) ? a_par : b_par;
}

struct CustomInt {
    int value;
    // 	В C++20 достаточно определить оператор <=>, чтобы тип автоматически 
    // 	удовлетворял и std::equality_comparable, и std::totally_ordered!
    auto operator<=>(const CustomInt&) const = default;
};

struct OnlyEquality {
    int value;
    auto operator==(const OnlyEquality&) const ->bool = default;
    // 	Операторов <, >, <=, >= здесь нет
};

void concepts_example_2() {
    CustomInt 	customInt1{10};
 	CustomInt 	customInt2{20};
    
	[[maybe_unused]]	OnlyEquality 	onlyeq1{10};
 	[[maybe_unused]]	OnlyEquality 	onlyeq2{20};

    [[maybe_unused]] 	auto min_val = find_min(customInt1, customInt2); 	
	// 	Отлично: `CustomInt` поддерживает авто-упорядочивание

    // 	СЛЕДУЮЩАЯ СТРОКА ВЫЗОВЕТ ОШИБКУ КОМПИЛЯЦИИ:
    // find_min(onlyeq1, onlyeq2); 
    // 	Ошибка: `OnlyEquality` не удовлетворяет концепту std::totally_ordered
}

auto main() ->int {
	
	return 0;
}
