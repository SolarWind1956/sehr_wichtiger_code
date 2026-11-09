/*
	
*/
#include	<iostream>
using std::cout; using std::endl; using std::boolalpha;
#include <limits>
using std::string;
using std::numeric_limits;
#include <cassert>
#include <cstring>
#include <concepts>
#include <vector>
#include <compare>			//	spaceship
#include "..\type_name_as_str.h"
/*	
	***************************************************************************************************
	Оператор трехстороннего сравнения в С++
	Дмитрий Бушуев
	https://ravesli.com/operator-trehstoronnego-sravneniya-v-s/
	--------------------------------------------------------------------------------------------
	Оператор трехстороннего сравнения (<=>) 
	добавили в С++20, 
	его часто еще называют «оператор-spaceship». 
	
	Данный оператор для двух значений 'A' и 'B' определяет, 
	является ли 
	A < B, 
	A = B 
	или 
	A > B. 
	
	Вы можете сами задать оператор трехстороннего сравнения 
	или 
	же компилятор автоматически сгенерирует его для вас.

	Чтобы оценить преимущества трехстороннего оператора сравнения, 
	позвольте мне начать с классических примеров.

	-	Сравнения до C++20
	-	Сравнения в C++20
	-	Сравнение во время компиляции
	-	Прямое использование оператора трехстороннего сравнения

	--------------------------------------------------------------------------------------------
	Сравнения до C++20

	Я написал небольшую обертку для типа int в виде структуры MyInt. 
	
	И, конечно же, я хочу сравнить переменные только что созданного типа. 
	
	Ниже представлено решение, 
	использующее шаблонную функцию isLessThan(), 
	которая выполняет сравнение с помощью оператора <:
*/
// comparisonOperator.cpp
 
#include <iostream>
 
struct MyInt {
    int value;
    explicit MyInt(int val)	: value{val} 
	{}
    bool operator < (const MyInt& rhs) const {                  
        return value < rhs.value;
    }
};
 
template <typename T>
constexpr bool isLessThan(const T& lhs, const T& rhs) {
    return lhs < rhs;
}
 
void test_1() 
{
	cout << "-------------- I'm void " << __func__ << "()" << endl;
    cout << std::boolalpha << endl;
 
    MyInt 	myInt2011(2011);
    MyInt 	myInt2014(2014);
 
    cout << "isLessThan(myInt2011, myInt2014): "
              << isLessThan(myInt2011, myInt2014) << endl;
}
/*
	Когда вы определяете одно из шести упорядочивающих отношений, 
	то должны задать и остальные пять. 
	
	А это значит, 
	что вам каждый раз придется писать 
	большое количество однообразного кода:

bool operator==(const MyInt& rhs) const { 
    return value == rhs.value; 
}
bool operator!=(const MyInt& rhs) const { 
    return !(*this == rhs);    
}
bool operator<=(const MyInt& rhs) const { 
    return !(rhs < *this);     
}
bool operator>(const MyInt& rhs)  const { 
    return rhs < *this;        
}
bool operator>=(const MyInt& rhs) const {   
    return !(*this < rhs);     
}

	Думаете всё? 
	
	Как бы не так! 
	
	А что, если теперь вы захотите сравнить MyInt с int? 
	
	Для этого вам потребуется реализовать поддержку сравнения MyInt и int, 
	а также варианта int и MyInt, 
	вдобавок необходимо будет перегрузить каждый оператор 3 раза, 
	т.к. наш конструктор объявлен со спецификатором explicit. 
	
	Благодаря данному спецификатору неявное преобразование из int в MyInt не сработает.

	Вы также можете объявить операторы в качестве дружественных функций класса, 
	добавляя три дополнительные дружественные функции для оператора сравнения <:

friend bool operator < (const MyInts& lhs, const MyInt& rhs) {                  
    return lhs.value < rhs.value;
}
 
friend bool operator < (int lhs, const MyInt& rhs) {                  
    return lhs < rhs.value;
}
 
friend bool operator < (const MyInts& lhs, int rhs) {                  
    return lhs.value < rhs;
}
	Это означает, что в общей сложности вы должны реализовать 18 операторов сравнения. 
	
	Похоже на конец истории? 
	
	Не факт! 
	
	Вдруг вы решили, что MyInt и все операторы должны стать constexpr. 
	
	Значит необходимо рассмотреть возможность создания noexcept-операторов.

	Я полагаю, что вы уже прочувствовали 
	основные проблемные места вышеописанного подхода 
	применения сравнений 
	и теперь готовы перейти к рассмотрению оператора трехстороннего сравнения.

	----------------------------------------------------------------------------------------------
	Сравнения в C++20


	Вы можете как сами определить оператор трехстороннего сравнения, 
	так и отдать эту работу на откуп компилятору,		
	использовав ключевое слово default. 
	
	В обоих случаях вы получите все шесть операторов сравнения: 
	
	==, 
	!=, 
	<, 
	<=, 
	>,
	>=

// threeWayComparison.cpp
*/ 
struct MyIntCPP20 {
    int 	value;
    explicit MyIntCPP20(int val): value{val} 
	{}
    auto operator<=>(const MyIntCPP20& rhs) const { 		// (1) оператор сравнения, определенный нами
        return value <=> rhs.value;
    }
};
 
struct MyDoubleCPP20 {
    double 	value;
    explicit constexpr MyDoubleCPP20(double val): value{val} 
	{}
    auto operator<=>(const MyDoubleCPP20&) const = default; // (2) оператор сравнения, определяемый компилятором
};
 
template <typename T>
constexpr bool isLessThanCPP20(const T& lhs, const T& rhs) {
    return lhs < rhs;
}
 
void test_2() 
{
	cout << "-------------- I'm void " << __func__ << "()" << endl;
    cout << std::boolalpha << endl;
    
    MyIntCPP20 myInt1(2011);
    MyIntCPP20 myInt2(2014);
    
    cout << "isLessThanCPP20(myInt1, myInt2): "
              << isLessThanCPP20(myInt1, myInt2) << endl;
              
    MyDoubleCPP20 myDouble1(2011);
    MyDoubleCPP20 myDouble2(2014);
    
    cout << "isLessThanCPP20(myDouble1, myDouble2): "
              << isLessThanCPP20(myDouble1, myDouble2) << endl;                 
	
	constexpr MyDoubleCPP20 myDouble3(2011);
    constexpr MyDoubleCPP20 myDouble4(2014);
}
/*
	Операторы сравнения (1) и (2) показывают ожидаемый результат своей работы:

	isLessThan(myInt1, myInt2): true
	isLessThan(myDouble1, myDouble2): true

	Стоит отметить, что при таком подходе есть несколько нюансов. 
	
	В первом варианте (1) поддерживается 
	'строгое упорядочивание', 
	а во втором (2) — 
	'частичное упорядочивание'. 
	
	Здесь дело не в операторе <=>.
	
	Числа типа с 'плавающей точкой' поддерживают только 'частичное упорядочивание', 
	поскольку значения типа с плавающей точкой, 
	такие как NaN (англ. «Not a Number» = «Не Является Числом»), 
	не могут быть упорядочены. 
	
	Например, сравнение NaN == NaN вернет false.

	------------------------------------------------------------------------------------------------
	Сравнение во время компиляции

	Оператор трехстороннего сравнения 
	является неявным constexpr-оператором. 
	
	Поэтому я упрощу предыдущий пример threeWayComparison.cpp, 
	выполняя сравнение при помощи функции MyDouble() на этапе компиляции:

// threeWayComparisonAtCompileTime.cpp
 
#include <compare>
#include <iostream>
 
struct MyDouble {
    double value;
    explicit constexpr MyDouble(double val): value{val} { }
    auto operator<=>(const MyDouble&) const = default;    
};
 
template <typename T>
constexpr bool isLessThan(const T& lhs, const T& rhs) {
    return lhs < rhs;
}
 
int main()
{
    cout << std::boolalpha << endl;
 
    constexpr MyDouble myDouble1(2011);
    constexpr MyDouble myDouble2(2014);
    
    constexpr bool res = isLessThan(myDouble1, myDouble2); // (1)
    
    cout << "isLessThan(myDouble1, myDouble2): "
              << res << endl;          
}
Результат выполнения программы:

isLessThan(myDouble1, myDouble2): true

	------------------------------------------------------------------------------------
	Прямое использование оператора трехстороннего сравнения

	Вы можете напрямую использовать оператор трехстороннего сравнения:

	// spaceship.cpp
*/ 
 
void test_3() 
{
	cout << "-------------- I'm void " << __func__ << "()" << endl;
	//	----------------------------------------------------------------
    int a(2011);
    int b(2014);
 
    auto res = a <=> b; 										// (1)
 
    if (res < 0) 
		cout << "a < b" << endl;
    else 
		if (res == 0) 
			cout << "a == b" << endl;
		else 
			if (res > 0) 
				cout << "a > b" << endl;
	//	-------------------------------------------------------------------------------------		
	if (a <=> b < 0) 
		cout << "a < b" << endl;
    else 
		if (a <=> b == 0) 
			cout << "a == b" << endl;
		else 
			if (a <=> b > 0) 
				cout << "a > b" << endl;		
 
	//	----------------------------------------------------------------
    std::string str1("2014");
    std::string str2("2011");
 
    auto res2 = str1 <=> str2; 									// (2)
 
    if (res2 < 0) 
		cout << "str1 < str2" << endl;
    else 
		if (res2 == 0) 
			cout << "str1 == str2" << endl;
		else 
			if (res2 > 0) 
				cout << "str1 > str2" << endl;
    
	//	----------------------------------------------------------------
    std::vector<int> vec1{1, 2, 3};
    std::vector<int> vec2{1, 2, 3};
 
    auto res3 = vec1 <=> vec2; 									// (3)
 
    if (res3 < 0) 	
		cout << "vec1 < vec2" << endl;
    else 
		if (res3 == 0) 
			cout << "vec1 == vec2" << endl;
		else 
			if (res3 > 0) 
				cout << "vec1 > vec2" << endl;   
	//	----------------------------------------------------------------		
}
/*
	Результат выполнения программы:

	a < b
	str1 > str2
	vec1 == vec2

	В вышеизложенных примерах мы увидели, 
	насколько сообразительным может быть наш компилятор, 
	самостоятельно генерируя все операторы сравнения.
*/
template<typename T>
struct AnyStructure{
	explicit constexpr AnyStructure(T val_) : value(val_) 
	{}
	auto operator<=>(const AnyStructure&) const = default;
	
	T value{};
};

void test_4() 
{
	cout << "-------------- I'm void " << __func__ << "()" << endl;
	AnyStructure	anyStructure1(142.5);
	AnyStructure	anyStructure2(542.2);
	
	if (anyStructure1 <=> anyStructure2 < 0) 
		cout << "anyStructure1 < anyStructure2" << endl;
    else 
		if (anyStructure1 <=> anyStructure2 == 0) 
			cout << "anyStructure1 == anyStructure2" << endl;
		else 
			if (anyStructure1 <=> anyStructure2 != 0) 
				cout << "anyStructure1 > anyStructure2" << endl;
			else 
				if (anyStructure1 > anyStructure2) 
					cout << "anyStructure1 >= anyStructure2" << endl;
}
/*
	********************		main()		****************************************
*/
int 	main()
{
	cout << "I'm *******	Introduction_to_CPP_20_spaceship_operator.cpp" << endl;
	//		

	test_1();
	test_2();
	test_3();
	test_4();
	
	return 0;
}
/*

*/
