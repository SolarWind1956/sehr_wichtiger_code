#pragma once
/*
	CPP20_Coroutine_from_scratch.hpp
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
#include <future>
#include "..\type_name_as_str.h"
#include "worker.hpp"
/*
	***************************************************************************************************
	Корутина с нуля
	https://www.linkedin.com/pulse/корутина-с-нуля-андрей-святовец-c7ybf/

	Андрей Святовец
	Software Developer – Luxoft
	--------------------------------------------------------------------------------------------
	Coroutine (сопрограмма, корутина) 	— 	функция, 
											выполнение которой можно явно прерываться 
											методами языка программирования. 
	
	Минимальная корутина.
	
	Создадим корутину с именем firstCoroutine, 
	которая ничего не делает, и ничего не возвращает.
*/
struct coroutine_control_struct
{
	struct promise_type
	{
		void 					return_void			() 			{}
		std::suspend_always 	initial_suspend 	() 			{ return {}; }
		std::suspend_always 	final_suspend 		() noexcept	{ return {}; }
		void 					unhandled_exception	() 			{}
		auto 					get_return_object	() 			{ return coroutine_control_struct{}; }
	};
};
inline coroutine_control_struct 	firstCoroutine(){
	cout << "-------------- I'm inline coroutine_control_struct " <<  __func__  << "()" << endl;
    co_return;
};
/*
	Если корутина является не статичным методом класса, 
	тогда первый параметр будет тип этого класса. 
	
	Например:
*/
struct A
{
	struct coroutine_control_struct 
	{
		struct promise_type
		{
			promise_type(const A& a, int b) {
				cout << "----COTOR---------- I'm " <<  __func__  << "(const A& a, int b) b == " << b << endl;
				cout << type_name_as_str<decltype(a)>() << endl;
			}
        
			auto 					get_return_object	() 			{ return coroutine_control_struct{}; }
			std::suspend_always 	initial_suspend 	() 			{ return {}; }
			std::suspend_always 	final_suspend 		() noexcept { return {}; }
			void 					unhandled_exception	() 			{}
			void 					return_void			() 			{}
		};
	};
	coroutine_control_struct 	firstCoroutine(int a)	{
		cout << "-------------- I'm coroutine_control_struct " <<  __func__  << "(int a)" << endl;
		co_return;
	}
};
/*
	------------------------------------------------------------------------------------
	Управление точкой останова

	Точка останова в корутине 
	определяется операторами co_wait и co_yield. 
	
	Такая точка останова управляется структурой, 
	в которой требуется обязательное присутствие следующих методов:
*/
struct awaiter {
	bool 	await_ready		() 							const {return false;}
	void 	await_suspend	(std::coroutine_handle<>) 	const {}
	void 	await_resume	() 							const {}
};
//	Такая структура не имеет требований к имени и поэтому ее можно назвать, как угодно, например, awaiter.

/*
	********************************************************************************************
	--------------------------------------------------------------------------------------
	Примеры использования корутин
	--------------------------------------------------------------------------------------
	Пример ленивого генератора
*/
//	Структура управления корутиной (сопрограммой)'
class Generator {
public:
    struct promise_type 
	{
    	auto get_return_object						() 			{ 
			cout << "-------------- I'm auto " <<  __func__  << "()" << endl;
			/*
				Получаем хандлер нашей корутины и тут же сохраняем его в нашем объекте Generator 
				с помощью специального конструктора.
			*/
			return Generator{std::coroutine_handle<promise_type>::from_promise(*this)};
		}
        std::suspend_never 		initial_suspend		()			{ return {}; }
        std::suspend_always 	final_suspend		() noexcept { return {}; }
        void 					unhandled_exception	()			{}
        std::suspend_always 	yield_value			(int value) { 
			cout << "\n-------------- I'm std::suspend_always " <<  __func__  << "(int value)" << endl;
			m_value 	= 	value;
			return {};
		}
		int 					getValue			() const 	{ return m_value; }
	private:
		int m_value;
    };

	Generator(std::coroutine_handle<promise_type> handle) : m_handle(handle) {
		cout << "---COTOR--- I'm " <<  __func__  << "(std::coroutine_handle<promise_type> handle)" << endl;
	}
	void 	next() 	const {
		cout << "-------------- I'm void " <<  __func__  << "() const" << endl;
		m_handle(); 
	}
	int 	value() const {
		cout << "----- I'm int " <<  __func__  << "() const   ";
		auto 	promise 	= 	m_handle.promise(); 
		return 	promise .getValue(); 
	}
private:
	std::coroutine_handle<promise_type> 	m_handle;
};
// 	Собственно сопрограмма 
inline Generator 	getSequence(int step) {
	cout << "--------------------------------------------- I'm inline Generator " <<  __func__  << "(int step)" << endl;
    for (int i = 0; ; i += step) {
        co_yield i;
    }
}
/*
	Разберем построчно что добавлено и для чего. 
	
	Для начала самая интересная строчка создания нашего управляющего класса 
	в методе auto get_return_object()
	
	return 	Generator{std::coroutine_handle<promise_type>::from_promise(*this)};

	Здесь с помощью, предопределенной в заголовочном файле <coroutine> 
	и описанной в разделе "Language support library, Coroutines, Class template coroutine_handle, Construct/reset" 
	статичной функции, 
	получаем хандлер нашей корутины и тут же сохраняем его в нашем объекте Generator 
	с помощью специального конструктора.

Generator(std::coroutine_handle<promise_type> handle) : m_handle(handle){}

	Теперь с помощью этого хандлера 
	мы можем управлять корутиной. 
	
	Об этом чуть позже, 
	а сейчас еще нужно сохранить полученное значение в co_yield 
	и получить его из promise_type. 
	
	Для этого добавим переменную для хранения и геттер в promise_type.
{	
	....
	int 		getValue() const { return m_value; }
private:
    int 		m_value;
}	
	Теперь остается только сохранить в m_value полученное значение.
	
std::suspend_always	yield_value(int value) { 		// std::suspend_always остановить выполнение и
												// отдать управление в вызывающую функцию
	m_value = value;
	return {};
}	
	Обратите внимание на возвращаемое значение. 
	
	Сейчас мы возвращаем стандартный объект управление точкой ожидания std::suspend_always, 
	но это может быть и собственный объект 
	со своей дополнительной функциональностью. 
	
	Например он может указывать корутине информацию с помощью await_resume(), 
	что значение получено, 
	и можно генерировать следующее значение, 
	а до этого пропускать все вызовы восстановления resume().

	Следующая интересная строчка — это получение объекта нашего promise_type. 
	
	Здесь как раз и понадобится хандлер и специальный метод promise(); 
	описан в стандарте, раздел "Language support library, Coroutines, Class template coroutine_handle, Promise access". 
	
	Это дает нам возможность доступа к объекту promise_type 
	и соответственно реализованного там геттера текущего значения.
	
int value() const {
   auto 	promise = m_handle.promise(); 		// Получить тип promise_type
										// Реализованный собственный метод для получения значения
   return 	promise .getValue();
}
	Теперь есть доступ к текущему значению. 
	
	Осталось реализовать возможность продолжения выполнения корутины. 
	
	В этом тоже поможет полученный ранее хандлер. 
	
	У хандлера есть оператор выполнения, 
	который по сути вызывает метод resume(). 
	
	"Language support library, Coroutines, Class template coroutine_handle, Resumption"
	
void next() const {
	m_handle(); 					// Продолжить выполнение корутины до следующей точки ожидания
}
	Что делает сама корутина? 
	
	Она в вечном цикле генерирует значения, 
	но на самом деле корутина прекращает свое выполнение 
	каждый раз на операторе co_yield, 
	в который передает текущее значение 
	и возвращает управление в вызывающую функцию. 
	
	Соответственно значение будет передано в управляющую структуру в метод yield_value,
	и мы его сохраним в m_value. 
	
	Само увеличение значения в цикле думаю и так понятно. 
	
	Следует только уточнить, 
	что корутина сохраняет каждый раз новое состояние, 
	и индекс всегда будет увеличиваться на указанную в параметре величину.
	
Generator getSequence(int step) {
	for (int i = 0;; i += step) {
		co_yield 	i; 					// Отдать управление в вызывающую функцию со значением 'i'
	}
}
*/
/*
	Такой же генератор можно с легкостью реализовать на обычных классах штатными и, 
	наверное, более понятными и простыми методами. 
	
	Нет ничего сложного в сохранении некого состояния и изменения его по требованию. 
*/
class LazyGenerator {
	int 	m_counter 	= 0;
	int 	m_step 		= 0;
public:
	LazyGenerator(int step) : m_step(step) {}

	int 	value() const 	{
		return 	m_counter;
	}
	void 	next() 			{
		m_counter += m_step;
	}
	int 	reset() 		{
		m_counter 	= 0;
		m_step 		= 0;
		return 0;
	}
};
/*
	***********************************************************************************************************************
	Однопоточное выполнение задач
	----------------------------------------------------
*/	
	
//	Для примера возьмем некоторую гипотетическую функцию обновления данных:	
class SomeData {
	uint64_t 		m_id 		= 0;
	const char 		*m_name 	= nullptr;
	uint64_t 		m_value 	= 0;

public:
	SomeData() 	= default;
	~SomeData() = default;
	
	SomeData(const SomeData &) 				= default;
	SomeData &operator=(const SomeData &) 	= delete;
	
	SomeData(SomeData &&) 					= delete;
	SomeData &operator=(SomeData &&) 		= delete;
  
	SomeData(uint64_t id, const char *name) : m_id{id}, m_name(name) 
	{}
  
	uint64_t 		id		() const { return m_id; }
	std::string 	name	() const { return m_name; }
};

// Fake operation
enum class OpType { Update, Insert };
// Normal function
inline bool 	updateData(const SomeData &data, OpType type) {
	cout << "\n----- I'm inline bool " <<  __func__  << "(const SomeData &data, OpType type)" << endl;
  
	// Data is not correct
	if (data.id() < 3)
		return false;
	// Do something
	cout << "----- data id: " << data.id() << ", data name: " << data.name() << " - I return : ";
	return true;
}
/*
	Копирование объекта данных здесь не очень продуктивно, 
	но это гарантирует неизменность и наличие данных 
	до момента выполнения функции в потоке. 
	
	Фрагмент несколько утрирован, но будем считать, 
	что данные приходят в неком независимом хандлере. 
	
	Тут, конечно, можно подождать до завершения выполнения функции 
	и передать данные по ссылке, 
	но это зависит от того, 
	что выгоднее копирование данных или ожидание их обновления. 
	
	Считаем, что копирование выгоднее.

	Возвращаемое значение не проверяется, так как это не имеет смысла. 
	
	Разве что можно что-то вывести на экран или лог, 
	но предпринять какие-то действия в этой точке уже поздновато.
*/
/*
	Пример с корутиной
	
	Выполним все то же только с использованием корутины:
*/	
// Result of updateDataCoro coroutine
enum 	class 	CoExecutionResult { None, ImmediateError, Success, Error };
 
// Coroutine
returnType<CoExecutionResult> 	updateDataCoro(const SomeData &data,  OpType type) {
	cout << "-------------- I'm inline void " <<  __func__  << "(const SomeData &data,  OpType type)" << endl;
	// Data is not correct
	if (data.id() < 3)
		co_return 	CoExecutionResult::ImmediateError;

	// Store data before exit from thread
	SomeData store_data{data};
	
	co_await awaitExecution{};

	// Do something
	cout << "data id:" << store_data.id() << ", data name:" << store_data.name() << std::endl;
	
	if (store_data.id() == 4)
		co_return CoExecutionResult::Error;
	co_return CoExecutionResult::Success;
}
/*	
	В корутине потребовалось дополнительное действие копирования данных:
	
	SomeData 	store_data{data};

	Если этого не делать, то придется позаботится о времени жизни данных в вызываемом коде.
*/
