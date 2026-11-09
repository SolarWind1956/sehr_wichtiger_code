// 	type_trait_tester.cpp
/*
	Я начал с малого:
	С Вашего разрешения :) разбил Ваш файл "type_trait_tester.cpp" на два: 
	собственно "type_trait_tester.cpp"
	и заголовочный файл "trivially_type_tester.hpp".
	Добавил еще один заголовок, который Вы мне ещё раньше любезно подправили:
	"..\..\include\type_name_as_str_chat_gpt.hpp"

	Теперь у меня уже есть семейство из двух тестеров.

	Так можно будет охватить тестирванием весь <type_traits>.
*/
#include <iostream>
using std::cout; using std::endl;
#include <type_traits>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>

#include "..\..\include\trivially_type_tester.hpp"
#include "..\..\include\type_name_as_str_chat_gpt.hpp"
#include "..\..\include\value_category_of_expression.hpp"
/*
	*********************************************************************************************************************
	trivially type test
	-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
// ---------- Add / modify test-types below ---------------
struct S_ok 				{ int a; double b; };           			// 	trivial, standard-layout, pod
struct S_user_ctor 		{ S_user_ctor() {} int a; }; 			// 	non-trivial: user-provided ctor
struct S_deleted_ctor 	{ S_deleted_ctor() = delete; int x; }; 	// 	trivial per standard (deleted ctor is not user-provided)
struct S_user_dtor 		{ ~S_user_dtor() {} int a; }; 			// 	non-trivial: user dtor
struct S_virtual 		{ virtual void f(){} int a; }; 			// 	non-trivial, not standard-layout

// 	example with `defaulted ctor` (still trivial if defaulted and not user-provided)
struct S_defaulted { S_defaulted() = default; int a; };

// 	generic wrapper to print type by name (we pass string manually)
void trivially_type_test_cpp20()	{
	cout << "Type traits interactive mini-tester (C++20)\n\n";

    analyze_trivially_type<S_ok>				("S_ok");
    analyze_trivially_type<S_user_ctor>		("S_user_ctor");
    analyze_trivially_type<S_deleted_ctor>		("S_deleted_ctor");
    analyze_trivially_type<S_user_dtor>		("S_user_dtor");
    analyze_trivially_type<S_virtual>			("S_virtual");
    analyze_trivially_type<S_defaulted>		("S_defaulted");

    // 	----- How to add own types:
    cout << "\n To test your own type: ";
	cout << "\n add its definition in main-file above ";
	cout << "\n and call analyze_trivially_type<YourType>(\"YourType\");\n\n";

    // 	Optional: if you want to include your own uploaded header (uncomment and adjust the path)
    // 	#include "path/to/your/header.hpp"
    // 	then call analyze_trivially_type<MyType>("MyType");
}
/*
	******************************************************************************************************
	value category of expression test
	---------------------------------------------------------------------------------------------------------------------------------------------------
*/
template <typename T>
T	templ_func_ret_value(){
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	return T{};
}
template <typename T>
struct AnyStruct{
	T 	m_any_field{T{}};
};

void value_category_of_expression_test(){
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	
	/*
		--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	*/
	cout << " ********************************************************	 `r-value` " << endl;
	/*
	К категории `r-value` относятся следующие выражения,
	являющиеся `pr-value` или `x-value`:	
	
	-	чисто математические значения (например, 7 или символ 'а'), 
		которые не обязательно имеют связанное с ними место хранения;
		они существуют только для вычислений, 
		но после того, как они были использованы, 
		на них нельзя ссылаться. 
*/
	cout << "  Тип 'a' == " << type_name_as_str_chat_gpt<decltype('a')>() << endl;	
	cout_test_result(value_category<decltype(('a'))>());
	static_assert(is_rvalue_expr_v<decltype(('a'))>);
	static_assert(is_prvalue_expr_v<decltype(('a'))>);
/*	
	- 	любые `литеральные значения`, за исключением `строковых литералов` 
		(например, 7, 'а', true, nullptr), являются r-значениями, 
		как и результаты многих встроенных арифметических вычислений
		(например, х + 5 для `х` целочисленного типа).
*/
	cout << "  Тип 'true' == " << type_name_as_str_chat_gpt<decltype(true)>() << endl;	
	cout_test_result(value_category<decltype((true))>());
	static_assert(is_rvalue_expr_v<decltype((true))>);
	static_assert(is_prvalue_expr_v<decltype((true))>);
	
	cout << "  Тип 'nullptr' == " << type_name_as_str_chat_gpt<decltype(nullptr)>() << endl;	
	cout_test_result(value_category<decltype((nullptr))>());
	static_assert(is_rvalue_expr_v<decltype((nullptr))>);
	static_assert(is_prvalue_expr_v<decltype((nullptr))>);
	
	cout << "  Тип '7 + true' == " << type_name_as_str_chat_gpt<decltype(7 + true)>() << endl;	
	cout_test_result(value_category<decltype((7 + true))>());
	static_assert(is_rvalue_expr_v<decltype((7 + true))>);
	static_assert(is_prvalue_expr_v<decltype((7 + true))>);

/*
	-	вызовов функций, возвращающих результат "по значению". 
*/
	cout << "  Тип 'templ_func_ret_value<double>()' == " 
			<< type_name_as_str_chat_gpt<decltype(templ_func_ret_value<double>())>() << endl;	
	cout_test_result(value_category<decltype((templ_func_ret_value<double>()))>());
	static_assert(is_rvalue_expr_v<decltype((templ_func_ret_value<double>()))>);
	static_assert(is_prvalue_expr_v<decltype((templ_func_ret_value<double>()))>);
/*	
	-	все `временные значения` являются `r-значениями`.
		(Хотя это не относится к именованным ссылкам, которые ссылаются на них.)
*/
	cout << "  Тип 'AnyStruct<int>{42}' == " << type_name_as_str_chat_gpt<decltype(AnyStruct<int>{42})>() << endl;	
	cout_test_result(value_category<decltype((AnyStruct<int>{42}))>());
	static_assert(is_rvalue_expr_v<decltype((AnyStruct<int>{42}))>);
	static_assert(is_prvalue_expr_v<decltype((AnyStruct<int>{42}))>);
/*
	--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
	cout << " ********************************************************	 `l-value` " << endl;
/*
	К категории `l-value` относятся следующие выражения:

	-	`имя переменной`, `функции` или `поле класса` любого типа. 
*/
	double dbl_var{3.14};
	cout << "  Тип `double dbl_var{3.14}` == " 
		<< type_name_as_str_chat_gpt<decltype(dbl_var)>() << endl;	
	cout_test_result(value_category<decltype((dbl_var))>());
	static_assert(is_lvalue_expr_v<decltype((dbl_var))>);
	static_assert(is_glvalue_expr_v<decltype((dbl_var))>);
	
	cout << "  Тип 'templ_func_ret_value<int>' == " 
			<< type_name_as_str_chat_gpt<decltype(templ_func_ret_value<int>)>() << endl;	
	cout_test_result(value_category<decltype((templ_func_ret_value<int>))>());
	static_assert(is_lvalue_expr_v<decltype((templ_func_ret_value<int>))>);
	static_assert(is_glvalue_expr_v<decltype((templ_func_ret_value<int>))>);
	
	AnyStruct<int>	any_struct{42};
	cout << "  Тип `any_struct.m_any_field` == " 
		<< type_name_as_str_chat_gpt<decltype(any_struct.m_any_field)>() << endl;	
	cout_test_result(value_category<decltype((any_struct.m_any_field))>());
	static_assert(is_lvalue_expr_v<decltype((any_struct.m_any_field))>);
	static_assert(is_glvalue_expr_v<decltype((any_struct.m_any_field))>);
/*	
		Даже если 'переменная' является `r-value`-ссылкой, 
		'имя этой переменной' в выражении является `l-value`;
*/
	int&&	r_value_ref	=	42;
	cout << "  Тип `auto&&	r_value_ref	=	42` == " 
		<< type_name_as_str_chat_gpt<decltype(r_value_ref)>() << endl;	
	cout_test_result(value_category<decltype((r_value_ref))>());
	static_assert(is_lvalue_expr_v<decltype((r_value_ref))>);
	static_assert(is_glvalue_expr_v<decltype((r_value_ref))>);
/*
	-	вызов функции или перегруженного оператора, 
		возвращающего `l-value`-ссылку, 
		либо выражение преобразования к типу `l-value`-ссылки;
		
	-	`встроенные операторы присваивания`, 
		составные операторы присваивания (=, +=, /= и т. д.), 
		`встроенные преинкремент` и `предекремент` (++a, --b), 
		`встроенный оператор разыменования указателя` (*ptr);
		
	-	`встроенный оператор обращения по индексу` (a[n] или n[a]), 
		когда один из операндов – `l-value` массив;
		
	-	вызов функции или перегруженного оператора, 
		возвращающего `r-value`-ссылку на функцию;

	-	строковый литерал, например "Hello, world!".
*/
	cout << "  Тип строкового литерала `Hello, world!` == " 
		<< type_name_as_str_chat_gpt<decltype("Hello, world!")>() << endl;	
	cout_test_result(value_category<decltype(("Hello, world!"))>());
	static_assert(is_lvalue_expr_v<decltype(("Hello, world!"))>);
	static_assert(is_glvalue_expr_v<decltype(("Hello, world!"))>);
/*
		`Строковый литерал` отличается от всех остальных литералов в языке C++ именно тем, 
		что является `l-value` (хотя и неизменяемым). 
		
		Например, можно получить его адрес:

		auto * 	ptr = &”Hello, world!”; 			// 	тут `константный указатель`, на самом деле
*/
/*
	-----------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
	cout << "********************************************************		`x-value`" << endl;
/*
	
	Примеры выражений категории `x-value`:

	-	вызов функции или встроенного оператора, 
		возвращающего `r-value`-ссылку, 
		например std::move(x);

		и в самом деле, для результата вызова std::move() 
		нельзя получить адрес в памяти 
		или 
		инициализировать им ссылку, 
		
		но в то же время, это выражение может быть полиморфным.
*/
	std::string 	s 		= "hello";
	auto&& 		rref_s 	= std::move(s); 	// 	`rref_s` ссылается на объект `s`, превращая его в "подвижный"
	cout << "string s = `hello`;  Тип  == std::move(s); " 
		<< type_name_as_str_chat_gpt<decltype(std::move(s))>() << endl;	
	cout_test_result(value_category<decltype((std::move(s)))>());
	static_assert(is_xvalue_expr_v<decltype((std::move(s)))>);
	static_assert(is_glvalue_and_rvalue_expr_v<decltype((std::move(s)))>);
/*		
	-	встроенный оператор обращения по индексу (a[n] или n[a]), 
		когда один из операндов – `r-value`-массив.	
*/		
/*
	-----------------------------------------------------------------------------------------------------------------------------------------------------------------
*/		
	cout << "********************************************************	  `pr-value`" << endl;	
/*
	К категории 'pr-value' относятся следующие выражения:

	-	литерал (кроме строкового), например `42`, `true` или `nullptr`;
	
	-	вызов функции или перегруженного оператора, 
		который возвращает не ссылку 
		
		(str.substr(1, 2), str1 + str2, it++) 
		
		или выражение преобразования к нессылочному типу 
		
		(например static_cast<double>(x), std::string{}, (int)42);
		
	-	`встроенные постинкремент` и `постдекремент` (a++, b--), 
		`встроенные математические операции` (a + b, a % b, a & b, a << b, и т.д.), 
		`встроенные логические операции` (a && b, a || b, !a, и т.д.), 
		`операции сравнения` (a < b, a == b, a >= b, и т.д.), 
		`встроенная операция взятия адреса` (&a);

	-	указатель `this`;

	-	`элемент перечисления`;

	-	`нетиповой параметр шаблона`, если он – не класс;
	
	-	`лямбда-выражение`, например [](int x){ return x * x; }.
*/
	auto any_lambda	=	[](int x){ return x * x; };
	cout << "auto any_lambda	=	[](int x){ return x * x; };  Тип  any_lambda ==  " 
		<< type_name_as_str_chat_gpt<decltype(any_lambda(11))>() << endl;	
	cout_test_result(value_category<decltype((any_lambda(11)))>());
	static_assert(is_prvalue_expr_v<decltype((any_lambda(11)))>);
	static_assert(is_rvalue_expr_v<decltype((any_lambda(11)))>);
}
/*
	******************************************************************************************************
	main()
	---------------------------------------------------------------------------------------------------------------------------------------------------
*/
int main() {
    
	//trivially_type_test_cpp20();
	
	value_category_of_expression_test();
	
    return 0;
}
