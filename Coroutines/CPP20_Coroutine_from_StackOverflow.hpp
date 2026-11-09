#pragma once
/*
	CPP20_Coroutine_from_StackOverflow.hpp
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
/*
	***************************************************************************************************
	Сопрограммы в С++
	https://ru.stackoverflow.com/questions/702454/Сопрограммы-в-С
	--------------------------------------------------------------------------------------------
	
	--------------------------------------------------------------------------------------------
	В С++ сопрограммой называется функция, 
	в которой используются co_await, co_yield, co_return.
*/
template <typename Y>
struct Future {
	struct promise_type {
		Future 					get_return_object	() 				{ return {this}; }
		std::suspend_never 		initial_suspend		() 				{ return {}; }
		std::suspend_always 	final_suspend		() noexcept 	{ return {}; }
		void 					return_value		(Y& y) 			{ y_ptr = &y; }
		
		std::atomic<Y*> 		y_ptr = nullptr;
	};

	bool	await_ready		()										{ return true; }
	auto 	await_suspend	(std::coroutine_handle<> handle)       	{ std::coroutine_handle<>{};   };
	void 	await_resume	()                						{}

	std::shared_ptr<promise_type> 	sh_ptr_promise;

	static void 	Deleter(promise_type* ptr_promise_type) 
	{
		auto handle = std::coroutine_handle<promise_type>::from_promise(*ptr_promise_type);
		handle.destroy();											// удаляет CoroutineState
	}

	Future(promise_type* ptr_promise_type) : sh_ptr_promise(ptr_promise_type, Deleter) {}

	Y 	BlockingGet() {
		while (sh_ptr_promise->y_ptr == nullptr) 
			std::this_thread::sleep_for(1s);						// ждем
		
		return *sh_ptr_promise->y_ptr;  							// дождались вызова p.return_value(y)
	}
};

// Общие данные фонового потока и Awaiter.
template <typename Y>
struct SharedState {
	std::coroutine_handle<> 	handle;
	Y 							value;
	std::atomic<bool> 			is_ready;
};

// Тип результата f()
template <typename Y>
struct Awaiter {
	std::shared_ptr<SharedState<Y>> sh_ptr;

	bool 	await_ready() { return false; }
  
	bool 	await_suspend(std::coroutine_handle<> hndl) {
		sh_ptr->hndl = hndl;
		return !sh_ptr->is_ready.exchange(true);  	// True если фоновый поток уже завершился
													// и у нас есть sh_ptr->value
	}
	Y 		await_resume() { return sh_ptr->value; }
};

template <typename X>
Awaiter<X> f(X x) {
	auto 	sh_ptr 	= 	std::make_shared<SharedState<X>>();
  
	std::thread	(	[=]	{  // Запуск фонового потока для вычислений
							sh_ptr->value = x * x;
							if (sh_ptr->is_ready.exchange(true)) {  // True, если await_suspend уже была вызвана
																	// и у нас есть sh_ptr->hndl
								sh_ptr->hndl.resume();
							}
					}
				).detach();
	return 	Awaiter{sh_ptr};
}
