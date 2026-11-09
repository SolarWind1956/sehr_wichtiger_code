#pragma once
/*
	6_Concurrency_6_1_4.hpp
*/
#include	<iostream>
using std::cout; using std::endl; using std::boolalpha;
#include <numeric>
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
#include <deque>
#include <list>
#include <forward_list>
#include <set>
#include<unordered_map>
#include <map>

#include <typeinfo> 
#include <type_traits> 
#include <variant>
#include <algorithm>
#include <cmath>
#include <memory>
#include <utility>
#include <future>
#include <ranges>
#include <algorithm>
#include <functional>
#include <coroutine>
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
	6.		Concurrency
	6. 		Параллельность
	6.1 		Корутины
	6.1.1 	Функция-генератор
	6.1.2 	Характеристики
	6.1.3 	Фреймворк
	6.1.4 	Ожидаемые и ожидающие объекты
	6.1.5 	Исполняемый поток процессов
	6.1.6 	co_return
	--------------------------------------------------------------------------------------------
	6.1 	Корутины
	
	Корутины (coroutines, сопрограммы) 	– 	это функции, 
											которые могут приостанавливать и продолжать свое выполнение, 
											сохраняя при этом свое состояние. 
											
	
	**********************************************************************************************************
	6.1.4 Ожидаемые и ожидающие объекты
 
	Три метода объекта-обещания – 
	yield_value(), initial_suspend() и final_suspend() – 
	возвращают 'ожидаемый объект'.
	
	---------------------------------------------------------------------------------------------
	6.1.4.1 Ожидаемые объекты
 
	'Ожидаемый объект' (awaitable) 	– 	это что-то, что вы можете ожидать. 
	
	Такой объект определяет, приостанавливается сопрограмма или нет.
 
	Используя объект-ожидание 'prom' и оператор co_await, 
	компилятор генерирует три вызова функций.
	
	---------------------------------------------------------------------------
	Вызов					 		Генерируемый компилятором вызов
	---------------------------------------------------------------------------
	yield value						co_await 	prom .yield_value(value)
	
	prom .initial_suspend()				co_await 	prom .initial_suspend()
	
	prom .final_suspend()				co_await 	prom .final_suspend()
	---------------------------------------------------------------------------
	
	Оператор co_await требует ожидаемый объект как аргумент. 
	
	'Ожидаемые объекты' должны реализовывать концепт Awaitable.
	
	-----------------------------------------------------------------------------------------------
	6.1.4.2 Концепт Awaitable
	
	Концепт Awaitable требует реализации трех методов.
	----------------------------------------------------------------------------------------------------------
	Метод				Описание
	----------------------------------------------------------------------------------------------------------
	await_ready()			Обозначает, готов ли результат. 
						Когда возвращает false, то вызывается await_suspend()
 
	await_suspend()		Управляет (schedule) сопрограммой: 
						выполнить операцию возобновления или уничтожения
 
	await_resume()		Возвращает результат для выражения co_await exp
	----------------------------------------------------------------------------------------------------------
	
	В стандарте С++20 уже определены два базовых ожидаемых объекта: 
	
		-	std::suspend_always 
		
		и 	
		-	std::suspend_never.
		
	-----------------------------------------------------------------------------------------------------------	
	6.1.4.3 std::suspend_always и std::suspend_never
	
	Как подсказывает его название, объект 
	
	suspend_always 	всегда приостанавливает выполнение. 
	
	Поэтому вызов await_ready() возвращает false.
 
	'Ожидаемый объект' std::suspend_always
*/
struct suspend_always {
	constexpr 	bool 	await_ready() 							const noexcept { return false; }
	constexpr 	void 	await_suspend(std::coroutine_handle<>) 	const noexcept {}
	constexpr 	void 	await_resume() 							const noexcept {}
};
/*
	Противоположное справедливо для std::suspend_never. 
	
	Он никогда не приостанавливает выполнение, 
	и вызов await_ready() всегда возвращает true.
 
	Ожидаемый объект std::suspend_never
*/
struct 	suspend_never {
	constexpr 	bool 	await_ready() 							const noexcept 	{ return true; }
	constexpr 	void 	await_suspend(std::coroutine_handle<>) 	const noexcept 	{}
	constexpr 	void 	await_resume() 							const noexcept 	{}
};
/*
	'Ожидаемые объекты' 
	std::suspend_always 
	и 
	std::suspend_never 	– 	это базовые строительные блоки для функций, 
							таких как initial_suspend() 
							и  
							final_suspend().  
							
	Обе функции автоматически выполняются, 
	когда осуществляется выход из сопрограммы: 
	
	initial_suspend() 	-	в начале 
	и 
	final_suspend() 	-	в конце сопрограммы.
	
	------------------------------------------------------------------------------------------------------
	6.1.4.4 initial_suspend()
	
	Когда метод initial_suspend() возвращает std::suspend_always, 
	то сопрограмма приостанавливается в самом начале. 
	
	При возвращении std::suspend_never сопрограмма не приостанавливается.
 
	-	Отложенная (lazy) сопрограмма, которая приостанавливается немедленно.
 
		Отложенная сопрограмма
*/
std::suspend_always 	initial_suspend() {
	return {};
};
/*
	-	Неотложенная (eager) сопрограмма, которая выполняется немедленно.
		
		Неотложенная сопрограмма
*/
std::suspend_never 		initial_suspend_() {
	return 	{};
}
/*
	-------------------------------------------------------------------------------------------------
	6.1.4.5 final_suspend
 
	Когда метод final_suspend() возвращает std::suspend_always, 
	сопрограмма приостанавливается в конце. 
	
	При возвращении std::suspend_never 
	сопрограмма не приостанавливается.
	
	-	Отложенная сопрограмма приостанавливается в конце.
		
		Отложенная сопрограмма, которая приостанавливается в конце своего выполнения
*/
std::suspend_always 	final_suspend()  noexcept {
	return {};
}
/*
	-	 Неотложенная сопрограмма, которая не приостанавливается в конце своего выполнения
*/
std::suspend_never 		final_suspend_() noexcept {
	return {};
}
/*
	До сих пор у нас были только ожидаемые (awaitable) объекты. 
	
	Еще нужен такой объект, который будет ожидать какого-то события. 
	
	-------------------------------------------------------------------------------------------------
	6.1.4.6 Ожидающие объекты
 
	Есть два различных способа получения ожидающего (awaiter) объекта:
 
		-	оператор co_await определен; 
		
		-	ожидаемый объект становится ожидающим.
 
	Когда вызывается co_await expression, 
	то выражение expression является ожидаемым. 
	
	Само выражение expression 	– 	это вызов объекта-обещания (ожидаемого): 
	
	prom.yield_value(value), 
	
	prom.initial_suspend() 
	
	или 
	
	prom.final_suspend(). 

	Для читаемости 
	я переименовал в следующих строках 
	объект-обещание из 
							prom 
	в 
							awaitable.
 
	Далее компилятор выполняет следующий алгоритм для получения ожидающего объекта:
	
	1.  	Он ищет оператор co_await для объекта-обещания 
		и возвращает ожидающий объект:
	
		awaiter 	= 	awaitable.operator 	co_await();
 
	2.  	Он ищет самостоятельный оператор co_wait 
		и возвращает ожидающий объект:
		
		awaiter 	= 	operator 			co_await();
 
	3.  	Если нет определенного оператора co_wait, 
		то ожидаемый объект становится ожидающим:
		
		awaiter = awaitable;
		awaiter = awaitable
 
	Когда вы изучите мою реализацию сопрограмм в этой главе, 
	то заметите, 
	что я использую почти все время случай, 
	когда ожидаемый объект неявно становится ожидающим. 
	
	Только пример в разделе по синхронизации потоков 
	использует оператор co_await для получения ожидающего объекта.
 
	После всех этих статических аспектов сопрограмм я хочу перейти к их динамическим аспектам. 
	
	*************************************************************************************************************
	6.1.5 Исполняемый поток процессов
	
	Компилятор преобразует сопрограмму 
	и выполняет 
	два 'исполняемых потока процессов' (workflow): 
	
		-	внешний процесс объекта-обещания 
		
		и 
		
		-	внутренний процесс ожидающего объекта.
		
	----------------------------------------------------------------------------------------------------	
	6.1.5.1 	Процесс объекта-ожидания
 
	Когда вы используете 
	
	co_yield, 
	co_await 
	или 
	co_return 
	
	в функции, то функция становится сопрограммой, 
	и компилятор преобразует ее тело во что-то, 
	эквивалентное следующим строкам кода:
 
	Преобразованная сопрограмма

{
	Promise		prom;
	co_await	prom .initial_suspend();
	try{
		<function 	body 	having		co_return, co_yield, or	co_wait>
	}
	catch(...){
		prom .unhandled_exception();
	}
	FinalSuspend:
		co_await		prom .final_suspend();
}

	Компилятор автоматически выполняет преобразованный код, 
	используя методы объекта-обещания. 
	
	Я называю такой процесс 'процессом объекта-обещания'. 
	
	Вот его основные этапы.
	
	-	Сопрограмма начинает выполнение:
	
			-	выделяет фрейм сопрограммы, если необходимо;
	
			-	копирует все параметры во фрейм сопрограммы;
		
			-	создает объект prom;
	
			-	вызывает для prom .get_return_object() создание дескриптора сопрограммы 
				и сохраняет его в локальной переменной. 
				
				Результат вызова будет возвращен при первой приостановке сопрограммы;
				
			-	вызывает prom.initial_suspend() 
				и ожидает его результат при помощи co_awaits. 
				
				Объект-обещание обычно возвращает 
				suspend_never 
				для «жадных» (eagerly-started) сопрограмм 
				и 
				suspend_always 
				для отложенных (eagerly-started) сопрограмм;
	
			-	тело сопрограммы выполняется тогда, 
				когда co_await prom.initial_suspend() возобновляет выполнение.
	
	-	Сопрограмма достигает точки приостановки:
	
			-	возвращаемый объект (prom.get_return_object()) возвращается вызывающей стороне, 
				которая возобновляет сопрограмму.
				
	-	Сопрограмма достигает co_return:
	
			-	вызывает prom.return_void() для co_return или co_return expression, 
				где expression имеет тип void;
				
			-	вызывает prom.return_value(expression) для co_return expression, 
				где expression имеет тип, отличный от void;
				
			-	уничтожает все связанные со стеком переменные;
	
			-	вызывает prom.final_suspend() и  ожидает его результат при помощи co_await.
			
	-	Сопрограмма уничтожается (уничтожая через co_return непойманное исключение 
		или через дескриптор сопрограммы):
		
			-	вызывает уничтожение объекта-обещания;
			
			-	вызывает деструктор параметров функции;
			
			-	освобождает память, используемую фреймом сопрограммы;
			
			-	передает управление обратно вызывающей стороне.
			
	Когда сопрограмма завершается с необработанным исключением, 
	то происходит следующее:
	
		-	исключение перехватывается, и вызывается prom.unhandled_exception() из блока catch;
		
		-	вызывается prom.final_suspend(), и ожидается результат через co_await.
		
	Когда происходит использование co_await expr в сопрограмме 
	или 
	компилятор неявно вызывает 
											co_await prom.initial_suspend(), 
											co_await prom.final.suspend() 
											или 
											co_await prom.yield_value(value), 
	то второй внутренний процесс – 
	процесс ожидающего объекта – 
	запускается.
	
	-----------------------------------------------------------------------------------------------------
	6.1.5.2 Поток ожидающих процессов
	
	Использование co_await expr заставляет компилятор преобразовать код, 
	основанный на функциях 
	await_ready(), await_suspend() и await_resume(). 
	
	Соответственно, я называю выполнение преобразованного кода 
	'потоком ожидающих процессов' (awaiter workflow).
	
	Компилятор генерирует примерно следующий код, использующий awaitable. 

	Для простоты я игнорирую обработку исключений 
	и описываю процесс при помощи комментариев.
	
	Созданный поток ожидающих процессов

{
	awaitable .await_ready()returns false:

	suspend		coroutine;

	awaitable .await_suspend(coroutine Handle) returns:

	void:
	awaitable .await_suspend(coroutine Handle);

	coroutine keeps suspended
		return	tocaller

	bool:
	bool	result	= awaitable .await_suspend(coroutine Handle);
		if	result:
			coroutine	keep 	suspended
		return	tocaller
	else:
	goto	resumptionPoint

	another	coroutine handle:

	bool:
	bool	result	= awaitable .await_suspend(coroutine Handle);
	if result:
		coroutine	keep suspended
		return	tocaller
	else:
		goto resumptionPoint

	anothercoroutineHandle:
	auto		anotherCoroutineHandle	=	awaitable .await_suspend(coroutine Handle);
	anotherCoroutineHandle .resume();
	return	tocaller

	resumptionPoint:
		return	awaitable .await_resume();
}	
	
	Процесс выполняется, 
	только если awaitable.await_ready() возвращает false. 
	
	В случае когда возвращается true, 
	сопрограмма готова и возвращает результат вызова awaitable.await_resume().
	
	Давайте предположим, что awaitable.await_ready() возвращает false. 
	
	Тогда сопрограмма приостанавливается 
	и немедленно обрабатывает значение вызова awaitable.await_ready(). 
	
	Возвращаемым типом может быть 
	void, 
	bool 
	или же 
	дескриптор другой сопрограммы anotherCoroutineHandle. 
	
	В зависимости от типа возвращаемого значения 
	происходит возврат управления или же выполняется другая сопрограмма.
	
	Возвращенное значение вызова awaitable.await_suspend()
	
	-----------------------------------------------------------------------------------------------------------------
	Тип 						Описание
	-----------------------------------------------------------------------------------------------------------------
	void 						Сопрограмма остается приостановленной, и управление возвращается вызывающей стороне
	
	bool 						bool == true 	сопрограмма остается приостановленной, 
												и управление возвращается вызывающей стороне
											
								bool == false 	возобновляется выполнение сопрограммы, 
												и не происходит возврат управления
									
	anotherCoroutineHandle 		Возобновляется выполнение другой сопрограммы, 
								и потом происходит возврат управления вызывающей стороне
	
	-----------------------------------------------------------------------------------------------------------------	
	
	Что происходит, когда происходит исключение? 
	
	Важно, где оно происходит 	– 
	в await_read(), await_suspend() или await_resume().
	
	-	await_ready(): 	сопрограмма не приостанавливается, 
						и не выполняются вызовы await_suspend и await_resume.
	
	-	await_suspend(): 	исключение перехватывается, 
						возобновляется выполнение сопрограммы, 
						и 
						исключение вызывается повторно. 
						
						await_resume() не вызывается.
	
	-	await_resume(): 	выполняются await_ready() и await_suspend(), 
						и все значения возвращаются. 
						
						Вызов await_resume() не возвращает результат.
						
	Давайте теперь применим теорию к практике.
*/	
/*
	*****************************************************************************************************
	6.1.6 co_return
	
	Сопрограмма использует co_return в качестве своего оператора возврата результата работы.
	
	----------------------------------------------------------------------------------------------
	6.1.6.1 	Future
	
	Сопрограмма, приведенная ниже, – это простейшая сопрограмма, 
	которую я только могу себе представить. 
	
	При этом она делает полезную работу: 
	
	автоматически запоминает результат своего вызова.
 
	Неотложенный объект 
*/
template<typename T>
struct MyNewFuture 
{
    std::shared_ptr<T> 	sh_ptr_value;
    
	MyNewFuture(std::shared_ptr<T> 	sh_ptr) : sh_ptr_value(sh_ptr) 	{
		cout << "---COTOR--- I'm " <<  __func__  << "(std::shared_ptr<T> sh_ptr) - *sh_ptr == " << *sh_ptr << endl;
	}
	
    ~MyNewFuture() 													{}
	
    T 	get() {
        return *sh_ptr_value;
    }
	
	// 	Связанный объект-обещание
    struct promise_type 
	{
        std::shared_ptr<T> 	sh_ptr = std::make_shared<T>();
		
        ~promise_type() 	{}
		
        MyNewFuture<T> 		get_return_object() 		{
            return sh_ptr;
        }
        void 				return_value(T v) 			{
            *sh_ptr = v;
        }
        std::suspend_never 	initial_suspend() 			{
            return {};
        }
        std::suspend_never 	final_suspend() noexcept 	{
            return {};
        }
        void 				unhandled_exception() 		{
            std::exit(1);
        }
    };
};
/*
	Класс MyNewFuture ведет себя как объект 'future', 
	который выполняется немедленно. 
	 
	Вызов сопрограммы createFuture() возвращает объект future, 
	и вызов fut.get() берет значение связанного объекта-обещания. 

	Но есть одна особенность для объекта future: 
	
	возвращенное значение сопрограммы createFuture() доступно после вызова. 
	
	Благодаря особенностям времени жизни 
	возвращаемое значение управляется при помощи std::shared_ptr. 
	
	Сопрограмма всегда использует std::suspend_never() 
	и поэтому ни до, ни после выполнения не приостанавливает своего выполнения. 
	
	Это значит, что сопрограмма выполняется тогда, 
	когда вызывается createFuture(). 
	
	Метод get_return_object() создает и запоминает дескриптор сопрограммы, 
	и return_value() запоминает результат работы сопрограммы, 
	который был возвращен строчкой 
	
	co_return 2021. 
	
	Клиент вызывает fut.get() и использует объект future как дескриптор объекта-обещания. 
	
	Метод get() возвращает результат клиенту.
*/
MyNewFuture<int> 	createFuture() {
	cout << "-------------- I'm MyNewFuture<int> " <<  __func__  << "()" << endl;
    co_return 	2021;
}

void	test_5(){
	cout << "-------------- I'm void " <<  __func__  << "()" << endl;

    auto 	fut = createFuture();
	/*
		Вызов сопрограммы createFuture() возвращает объект future, 
		и вызов fut.get() берет значение связанного объекта-обещания. 
	*/
    cout << "fut.get(): " << fut.get() << '\n';
}
/*
	Можно подумать, что вряд ли стоило показывать реализацию сопрограммы, 
	которая ведет себя как функция. 
	
	Но эта простая сопрограмма – идеальная отправная точка 
	для разработки различных реализаций объектов future. 
	
	Об этом рассказывается в разделе «Варианты future» далее в книге.
	
	******************************************************************************************************
	6.1.7 	co_yield
 
	Благодаря co_yield 
	вы можете реализовать генератор, создающий бесконечный поток данных, 
	из которого вы можете брать значения. 
	
	Возвращаемый тип генератора 
	
	generatorForNumbers(int begin, int inc=1) 	– 	это generator<int>, 
	
	при этом сам генератор содержит внутри себя специальный объект-обещание рromise, 
	такой что вызов 
	
	co_yield 	i ;
	
	эквивалентен вызову 
	
	co_await 	рromise.yield_value(i). 
	
	Оператор 	'co_yield 	i '	
	может быть вызван произвольное число раз. 
	
	Сразу после каждого вызова выполнение сопрограммы приостанавливается.
	
	--------------------------------------------------------------------------------
	6.1.7.1 Бесконечный поток данных
	
	Код ниже создает 'бесконечный поток данных' (infinite data stream). 
	
	Сопрограмма getNext() использует co_yield для того, 
	чтобы создать поток данных, 
	который начинается со start 
	и при получении очередного запроса увеличивает значение на step.
	
	Бесконечный поток данных
*/

template<typename T>
struct NewGenerator 
{
    struct 	promise_type;	//	Объявление - определение ниже по коду, но встроенное
	
    using 	handle_type = std::coroutine_handle<promise_type>;
    
    NewGenerator(handle_type 	hndl): coro(hndl) {}          // 	Создаем генератор.
	
    handle_type 		coro;
    
	std::shared_ptr<T> 	sh_ptr_value;
    
    ~NewGenerator() {
        if ( coro ) coro.destroy();
    }
    NewGenerator(const NewGenerator&) 				= delete;
    NewGenerator& operator= (const NewGenerator&) 	= delete;
	
    NewGenerator(NewGenerator&& other): coro(other.coro) {
        other.coro = nullptr;
    }
    NewGenerator& operator = (NewGenerator&& other) {
        coro 		= other.coro;
        other.coro 	= nullptr;
        return *this;
    }
    int getValue() {
        return coro.promise().current_value;
    }
	//	Запрашивает следующее значение и возвращает его.
    bool next() {        
		cout << "-------------- I'm bool NewGenerator::" <<  __func__  << "()" << endl;
        coro.resume();
        return not coro.done();
    }
    struct promise_type {
		//	 Создаем объект-обещание.
        promise_type()  = default;                              
          
        ~promise_type() = default;
        
		//	Вызываем promise.initial_suspend(). 
		//	Генератор отложенный (lazy) и поэтому всегда приостанавливает свое выполнение.
        auto initial_suspend() {                                 
            return std::suspend_always{};
        }
        auto final_suspend() noexcept {
            return std::suspend_always{};
        }
		//	Вызываем promise.get_return_object() и  сохраняем результат в локальной переменной.
        auto get_return_object() {                              
            return NewGenerator{handle_type::from_promise(*this)};
        }
        auto return_void() {
            return std::suspend_never{};
        }
		//	Запускается вызовом co_yield. После этого доступно следующее значение.
        auto yield_value(int value) {                          
            current_value = value;
            return std::suspend_always{};
        }
        void unhandled_exception() {
            std::exit(1);
        }
        T current_value;
    };
};

NewGenerator<int> 	getNext(int start = 0, int step = 1) 
{
	cout << "-------------- I'm NewGenerator<int> " <<  __func__  << "(int start = 0, int step = 1)" << endl;
    auto value = start;
	
    for (int i = 0;; ++i) {
        co_yield value;
        value += step;
    }
}

void	test_6(){
	cout << "-------------- I'm void " <<  __func__  << "()" << endl;
  
    cout << "getNext():\n" ;
	
    auto gen = getNext();
	
    cout << "getNext?(0, 10):\n";
	for (int i = 0; i <= 10; ++i) {
        gen.next();
        cout << " " << gen.getValue();          	//	Получает следующее значение.
    }
    cout << "\n\n";
    
    cout << "getNext(100, -10):\n";
    auto gen2 = getNext(100, -10);
    for (int i = 0; i <= 20; ++i) {
        gen2.next();
        cout << " " << gen2.getValue();				//	Получает следующее значение.
    }
}
/*
	Функция  test_6() создает две сопрограммы. 
	
	Первая сопрограмма gen() 
	возвращает значения от 0 до 10, 
	вторая сопрограмма gen2() 
	возвращает значения от 100 до –100. 
	
	Прежде чем я более детально опишу процесс работы программы благодаря онлайн-компилятору Wandbox, 
	изучите вывод этой программы.
*/
