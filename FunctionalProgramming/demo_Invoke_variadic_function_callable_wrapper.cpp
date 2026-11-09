/*
	demo_Invoke_variadic_function_callable_wrapper.cpp
*/
#include <iostream>
using std::cout; using std::endl; using std::cerr;  using std::boolalpha;
#include <algorithm>          // 	std::max
#include <functional>
#include <utility>

#include "../../include/color_output.hpp"

using namespace	color_output;

/*************************************************************************************
	Invoke_variadic_function_callable_wrapper
	 
	Вызов обертки callable вариативной функции 
**************************************************************************************/
/*
	===================================================================
	ИИ
		Для создания обертки (wrapper) над вариативной вызываемой функцией в C++ (variadic callable) 
		используется шаблонная функция с variadic-шаблонами (typename... Args) 
		и 
		std::invoke<>
		или 
		std::forward<> `прямой вызов` 
		для обработки `переменного числа аргументов`. 
		
		Обертка перенаправляет вызов, сохраняя типы аргументов. 
	ИИ
	===================================================================	
*/
// 	Вариативная обертка (Wrapper/Proxy)
template<typename F, typename... Args>
auto 	wrapper	(	F&& 		f
				, 	Args&&... 	args) 
{
    cout << bright_yellow << "-------------- I'm " << __PRETTY_FUNCTION__ << reset << endl;
    
	// 	std::invoke обрабатывает функции, лямбды и методы
    return 	std::invoke	(	std::forward<F>(f)
						, 	std::forward<Args>(args)...
						);
}

// 	Примеры целевых функций
void 	printSum(int a, int b) { cout << "Сумма: " << a + b << endl; }
int 		multiply(int a, int b) { return a * b; }

void demo_1() 
{
	cout << bright_white << "-------------- I'm " << __PRETTY_FUNCTION__ << reset << endl;
	
    // 	1. 	Вызов обычной функции
    wrapper(printSum, 10, 20);

    // 	2. 	Вызов лямбды с возвращаемым значением
    auto 	res = wrapper([](int a, int b) { return a + b; }, 5, 3);
    cout << "Результат лямбды: " << res << endl;

}
void demo_2() 
{
	cout << bright_white << "-------------- I'm " << __PRETTY_FUNCTION__ << reset << endl;
	
	auto result = wrapper(multiply, 10, 20);
	cout << "Результат multiply(10, 20) : " << result << endl;
}
/*
	===================================================================
	ИИ
		Основные моменты реализации:
		
		template<typename... Args>: 				Принимает переменное число типов аргументов.

		Args&&... args: 							Универсальные ссылки `&&` (forwarding references) 
												для сохранения lvalue/rvalue свойств.
												
		std::forward<Args>(args)...: 				Передает аргументы с сохранением их типа (perfect forwarding).
		
		std::invoke: 								стандарт для вызова любого вызываемого объекта 
												(функция, указатель на метод, функтор).
												
		std::function<>(): 							Используется, 
												если обертке нужно хранить или передавать 
												вызываемый объект, а не просто вызывать его. 
	ИИ
	===================================================================	
*/	
/*
	===================================================================
	ИИ
		Для реализации универсальной обертки 
		над любым вызываемым объектом (callable) 
		с произвольным количеством аргументов 
		используется 
		-	std::invoke() 
		-	совместно с `variadic templates` 
		и 
		-	std::forward.
		
		Базовая реализация обертки

		Этот шаблон позволяет 
		передать 
		-	любую функцию, 
		-	лямбду 
		или 
		-	метод 
		
		и выполнить `дополнительные действия` (например, логирование) 
		до и после вызова:
	ИИ
	===================================================================	
*/
template <typename F, typename... Args>
auto wrapper_2	(	F&& 		f
				, 	Args&&... 	args
				) 
{
    cout << "Before call" << endl;
    
    // 	std::invoke автоматически обрабатывает указатели на функции, 
    // 	лямбды и даже указатели на методы классов
    auto 	result 	= 	std::invoke	(	std::forward<F>(f)
									, 	std::forward<Args>(args)...
									);
    
    cout << "After call" << endl;
    
	return 	result;
}

// 	Пример использования
int summa(int a, int b) { return a + b; }

void demo_3() 
{
	cout << bright_white << "-------------- I'm " << __PRETTY_FUNCTION__ << reset << endl;

    int 	res 	= wrapper(summa, 10, 20); 	// 	Вызовет обычную функцию
	cout << "Результат summa(10, 20) : " << res << endl;
	
    wrapper([](const std::string& s) { cout << s << endl; }, "Hello!"); 	// 	Лямбду
}
/*
	===================================================================
	ИИ
		Ключевые моменты

		std::invoke: 					Унифицирует вызов. 
							
									Вам не нужно писать разные перегрузки для обычных функций 
									и указателей на методы классов ((obj.*ptr)(args...)) 
									std::invoke сделает это за вас.
							
		std::forward<Args>(args)...: 	Техника Perfect Forwarding. 
									
									Она сохраняет категорию значения (lvalue/rvalue) аргументов 
									при передаче в целевую функцию.
									
		auto (return type): 			Позволяет функции-обертке 
									автоматически выводить тип возвращаемого значения 
									целевой функции. 
		
		Если нужно вернуть `void`

		В C++ можно просто возвращать `результат` std::invoke, 
		даже если это `void`. 
		
		Однако, если вы используете C++14 или ниже, 
		где std::invoke нет, для возврата `void` может потребоваться 
		std::result_of_t или использование decltype(auto).
	ИИ
	===================================================================	
*/
/*
	--------------------------------------------------------------------------------------------------------------------------------------------------------
	Пример из книги Джона Лакоса
*/
#include <mutex>     		// 	std::mutex 	и 	std::lock_guard
template <typename Func, typename... Args>
decltype(auto) 	lockedInvokeFunction	(	std::mutex& 	m
									, 	Func&& 		f
									, 	Args&&... 	args
									)
{
	cout << bright_yellow << "-------------- I'm " << __PRETTY_FUNCTION__ << reset << endl;
	
    std::lock_guard<std::mutex> 	mutexLock(m);
	#if 0	
	return 	std::forward<Func>(f)(std::forward<Args>(args)...);
	#endif	
	#if 1
	return 	std::invoke	(	std::forward<Func>(f)
						, 	std::forward<Args>(args)...
						);
	#endif		    
}
//	Вариант с рекурсией (рекомендуемый в книгах по современному C++)
//	Не использовать initializer_list, а сделать рекурсивную версию :
template <typename T>
constexpr T 	variadic_max_wrap(T v) 
{
	return v;
}

template <typename T, typename... Args>
constexpr T variadic_max_wrap(T first, Args... args) 
{
	T 	tail_max 	= 	variadic_max_wrap(args...);
	
	return 	first < tail_max 
			? 	tail_max 
			: 	first
			;
}
void 	demo_4(){
	cout << bright_white << "-------------- I'm " << __PRETTY_FUNCTION__ << reset << endl;
	
	std::mutex 	mtx;
	
	decltype(auto)	result_0 = variadic_max_wrap(3.14, 2.73, 9.81);
	cout << "Максимальное значение из списка 3.14, 2.73, 9.81 : " << result_0  << endl;
	
	// 	Оборачиваем шаблонную функцию в вызываемый объект (generic lambda),
	// 	чтобы иметь возможность передать её в lockedInvokeFunction.
	auto variadic_max_callable = [](auto&&... args) -> decltype(auto) {
		return variadic_max_wrap(std::forward<decltype(args)>(args)...);
	};
	
	decltype(auto)	result_1 = lockedInvokeFunction	(	mtx
												,	variadic_max_callable
												,	3.14, 2.73, 9.81, 6.314'462'618
												);									
	cout << "Максимальное значение из списка 3.14, 2.73, 9.81, 6.314'462'618 : " << result_1 << endl;
}
//	------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int main()	{
	color_output::init();
	cout << cyan << "-------------- I'm " << __PRETTY_FUNCTION__ << " of Chapter_25.cpp" << reset << endl;

	//demo_1();
	//demo_2();
	//demo_3();
	demo_4();
	
	//system("pause");
	return 0;
}