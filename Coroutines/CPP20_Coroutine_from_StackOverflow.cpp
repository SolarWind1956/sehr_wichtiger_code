/*
	CPP20_Coroutine_from_StackOverflow.cpp
*/	
#include	<chrono>
using namespace std::chrono_literals;
#include <coroutine>
#include <stdexcept>
#include <thread>
#include	<iostream>
using std::cout; using std::endl; using std::boolalpha;
#include <numeric>
#include <limits>
using std::numeric_limits;
#include <string>
using std::string;
#include "..\type_name_as_str.h"
#include "CPP20_Coroutine_from_StackOverflow.hpp"
/*
	***************************************************************************************************
	Сопрограммы в С++
	https://ru.stackoverflow.com/questions/702454/Сопрограммы-в-С
	
	--------------------------------------------------------------------------------------------
	
	--------------------------------------------------------------------------------------------
	В С++ сопрограммой называется функция, 
	в которой используются co_await, co_yield, co_return.
*/
/*
template <typename X, typename Y>
Future 	coroutine(X x) 
{
	Future::promise_type 	promise;
	
	Y 	y = f(x);
	co_await 	promise.initial_suspend();
	
	co_return y;
	
final_suspend:
	
}
void test_0(){
	cout << "-------------- I'm inline void " <<  __func__  << "()" << endl;
	
	auto ret_val	=	coroutine<int, Future>(21);
	cout << type_name_as_str<decltype(ret_val)>() << endl;
}
*/
/*
	Компилятор переписывает тело сопрограммы, превращая ее в 'машину состояний'.
	
	Под данные сопрограммы выделяется память при помощи оператора new.
*/
template <typename X, typename Y>
Future<X> 	coroutine_c(X x) 
{
	struct CoroutineState {
		Future<X>::promise_type 	promise;
		X 		x;
		Y 		y;
		int 	state = 0;

		void run() {
			switch (state) {
			case 0:
				//...
				state = 1;  // приостановка
				return;
			case 1:       	// точка возобновления
				//...
				break;
			default:
				cout << "state is undefined" << "\n";
				break;
			}
		}
	};
	auto* 	state_ptr 		= 	new CoroutineState;
	
	auto 	result 	= 	state_ptr->promise.get_return_object();
	state_ptr->x = x;
	state_ptr->y = Y{};
	state_ptr->run();
	
	return 	result;
}
void test_1(){
	cout << "-------------- I'm inline void " <<  __func__  << "()" << endl;
	
	auto ret_val	=	coroutine_c<int, double>(21);
	cout << type_name_as_str<decltype(ret_val)>() << endl;
}
/*
	При этом, хотя тело сопрограммы изменяется, она остается функцией.

	**********************************************************************************************************
	promise_type (ч.1)

	Компилятор добавляет неявную переменную с типом Future::promise_type.
	
	Эта переменная используется для:
	-	создания результата функции-сопрограммы (promise.get_return_object), 
	-	обработки исключений, 
	-	реализации co_return. 
	
	Также компилятор добавляет 'точки приостановки' 
	в начале и конце сопрограммы, 
	оборачивая тело сопрограммы в следующий код:
*/
template <typename X, typename Y = Awaiter<X>>
Future<X>	coroutine_c_1(X x) 
{
	cout << "--- I'm template <typename X, typename Y = Awaiter<X>> Future<X> " <<  __func__  << "(X x)" << endl;
	using	promise_type	=	Future<X>::promise_type;
	promise_type	promise;
	
	co_await promise.initial_suspend();
	
	try {
		// тело сопрограммы
		
		Y 	y = co_await 	f(x);
		co_return y;
				
		// конец тела сопрограммы
	} catch(...) { 
		promise.unhandled_exception(); 
	}
final_suspend:
	co_await 	promise.final_suspend();
};
void test_2(){
	cout << "-------------- I'm inline void " <<  __func__  << "()" << endl;
	
	auto ret_val	=	coroutine_c_1<int>(21);
	cout << type_name_as_str<decltype(ret_val)>() << endl;
}
/*
	-------------------------------------------------------------------------------------------
	co_return
	
	В сопрограмме нельзя использовать 
	
	return y;, 
	
	вместо него используется 
	
	co_return y;, 
	
	который заменяется на

p.return_value(y);
goto final_suspend;

	Если сопрограмма не предусматривает возврат значения по завершении, 
	то используется co_return; (без выражения) 
	и соответствующая ему функция promise.return_void();.
	
	При этом не обязательно писать co_return; в конце сопрограммы.

	-------------------------------------------------------------------------------------
	co_await
	
	Приостановка сопрограммы происходит в операторе co_await.

	Код 
	
	Y y = co_await f(x); 
	
	заменяется на

auto e = f(x);
if (!e.await_ready()) {
  ... приостановка ...
  
  std::coroutine_handle<> 	h 	= ...;
  
  if (e.await_suspend(h)) 
	return;
resume:  // точка возобновления для h.resume()
	... возобновление ...
}
Y 	y 	= 	e.await_resume();

	Стандартная библиотека предоставляет класс 
	
	coroutine_handle, 
	
	который позволяет 'возобновить приостановленную сопрограмму'.

	Функция f() принимает его через 
	
	e.await_suspend(h). 
	
	Когда значение 'y' будет вычислено, 
	она должна вызвать 
	
	h.resume(), 
	
	и вернуть вычисленное значение через 
	
	e.await_resume().

	Для stackless сопрограммы, функия f() может быть написана следующим образом:
	
	Смотри заголовочный файл 	CPP20_Coroutine_from_StackOverflow.hpp

	Для stackful сопрограммы (например Fibers в Windows), 
	await_suspend() должна сама замораживать поток (SwitchToFiber). 
	
	Точка возобновления будет внутри await_suspend(), 
	поэтому она должна возвращать false.

	**********************************************************************************************************
	promise_type (ч.2)
	
	Минимальный тип возвращаемого значения сопрограммы выглядит так:
	
	Future::promise_type 
	
	должен иметь 
	
	get_return_object(), 
	initial_suspend(), 
	final_suspend() 
	и 
	либо return_void() либо return_value().

	Для реализации initial_suspend() и final_suspend() 
	можно использовать стандартные 
	std::suspend_never 
	и 
	std::suspend_always, которые 
	возвращают в await_ready() 
	значения true и false соответственно.

	Такая сопрограмма будет всегда засыпать в конце.

	От самого Future требуется только,
	чтобы он удалил сопрограмму через h.destroy().

	Future может (но не обязан) повторять интерфейс Awaiter, 
	чтобы быть совместимым с co_await.

	---------------------------------------------------------------------------------------
	await_transform() и operator co_await
	
	Для выражения в co_await могут применяться дополнительные преобразования:

	если выражение 
	
	promise.await_transform(a) 
	
	валидно, 
	
	то 'a' заменяется на 
	
	promise.await_transform(a);

	если для типа 'a' есть оператор operator co_await, 
	то 'a' заменяется на operator co_await(a);

	если в результате получилось prvalue, 
	то оно копируется во временную переменную, 
	иначе используется как есть.

	Таким образом возможен вариант

	auto& e = operator co_await(promise.await_transform(f(x)));
	
	if (!e.await_ready()) { ... }

	Например можно определить operator co_await(std::chrono::duration) и писать co_await 10ms;.

	---------------------------------------------------------------------------------------------
	Выделение памяти

	Объект CoroutineState создается при помощи new. 
	
	Однако, если есть функция 
	
	promise.get_return_object_on_allocation_failure(), 
	
	то будет сгенерирован следующий код:

	auto* s = new(std::nothrow) CoroutineState;
	if (!s) {
		return promise.get_return_object_on_allocation_failure();
	}
	auto result = s->promise.get_return_object();

	Это позволяет обрабатывать ошибки выделения памяти.

	Также, аргументы сопрограммы могут участвовать в выделении памяти.

	Для сопрограммы 
	
	Future coro(A1 a1, A2 a2), 
	
	если есть функция operator new(std::size_t, A1, A2), 
	то она будет вызвана вместо оператора new по умолчанию.

	*****************************************************************************************************
	promise_type (ч.3), coroutine_traits
	
	Компилятор использует класс 
	
	coroutine_traits 
	
	для получения 
	
	promise_type.

	Для сопрограммы 
	
	Future coro(A1 a1, A2 a2) будет использован тип 
	
	std::coroutine_traits<Future, A1, A2>::promise_type.

	Реализация по-умолчанию выдает 
	
	Future::promise_type, 
	
	однако это может быть переопределено пользователем.

	------------------------------------------------------------------------------------------
	co_yield

	co_yield 	e; 
	
	эквивалентен 
	
	co_await promise.yield_value(e); 
	
	и используется в генераторах - специальных сопрограммах,
	которые предназначены для выдачи последовательности значений.
*/
/*
	
*/
/*	
	**************************		main()		***************************************************
*/
int main(){
	cout << "-------------- I'm int " << __func__ << "() of CPP20_Coroutine_from_StackOverflow.cpp" << endl;
	
	cout << endl;
	//test_0();	cout << endl;
	test_1();	cout << endl;
	test_2();	cout << endl;
	
};	