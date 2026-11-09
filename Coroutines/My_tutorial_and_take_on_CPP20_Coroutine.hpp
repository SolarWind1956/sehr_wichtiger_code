#pragma once
/*
	My_tutorial_and_take_on_CPP20_Coroutine.hpp
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
	
	--------------------------------------------------------------------------------------------
	Основные структуры:
	--------------------------------------------------------------------------------------------
	Возвращаемый объект		-	ReturnObject			-	структура управления сопрограммой
	
	Объект ожидания 			-	Awaiter				-
	
	Объект обещания				-	Promise				-	std::coroutine_traits::promise_type
															
															Тип должен иметь строгое имя 'promise_type'.
	
	Дескриптор сопрограммы 		-	std::coroutine_handle	-	
	
*/
#include <concepts>
#include <coroutine>
#include <exception>
#include <iostream>
//	Возвращаемый объект	-	структура управления сопрограммой
struct 	ReturnObject 
{
	struct promise_type 
	{
		ReturnObject 			get_return_object			() 			{ return {}; }
		std::suspend_never 		initial_suspend				() 			{ return {}; }
		std::suspend_never 		final_suspend				() noexcept { return {}; }
		void 					unhandled_exception			() 			{}
	};
};

//	Объект ожидания 
struct 	Awaiter 
{
	std::coroutine_handle<> *	hp_;
	
	constexpr bool 		await_ready							() const noexcept 			{ return false; }
	void 				await_suspend						(std::coroutine_handle<> h) { *hp_ = h; }
	constexpr void 		await_resume						() const noexcept 			{}
};

//	Возвращаемый объект	-	структура управления сопрограммой
struct ReturnObject2 
{
	struct promise_type {
		ReturnObject2 	get_return_object() {
			return {
				// Uses C++20 designated initializer syntax
				.h_ 	= 	std::coroutine_handle<promise_type>::from_promise(*this)
			};
		}
		std::suspend_never 		initial_suspend		() 				{ return {}; }
		std::suspend_never 		final_suspend		() noexcept 	{ return {}; }
		void 					unhandled_exception	() 				{}
	};

	std::coroutine_handle<promise_type> 	h_;
	
	operator 	std::coroutine_handle<promise_type>	() 	const 		{ return h_; }
  
	// A coroutine_handle<promise_type> converts to coroutine_handle<>
	operator 	std::coroutine_handle<>				() 	const 		{ return h_; }
};
//	Объект обещания
template<typename PromiseType>
struct 	GetPromise 
{
	PromiseType *	ptr_PromiseType;
	
	// says yes call await_suspend()
	bool 			await_ready		() 												{ return false; } 
	
	// says no don't suspend coroutine after all
	bool 			await_suspend	(std::coroutine_handle<PromiseType> handle) 	{
    	ptr_PromiseType = &handle.promise();
    	return 		false;     					
	}
  
	PromiseType*	await_resume	() 												{ return ptr_PromiseType; }
};

//	Возвращаемый объект	-	структура управления сопрограммой
struct 	ReturnObject3 
{
	struct promise_type {
		unsigned 	value_;

		ReturnObject3 		get_return_object	() {
			return 	ReturnObject3 { .handle_ = std::coroutine_handle<promise_type>::from_promise(*this) };
		}
		std::suspend_never 	initial_suspend		() 			{ return {}; }
		std::suspend_never 	final_suspend		() noexcept { return {}; }
		void 				unhandled_exception	() 			{}
	};

	std::coroutine_handle<promise_type> 	handle_;
	
	operator 	std::coroutine_handle<promise_type>() const { return handle_; }
};

//	Возвращаемый объект	-	структура управления сопрограммой
struct ReturnObject4 
{
	struct promise_type 
	{
		unsigned value_;

		ReturnObject4 				get_return_object		() 					{
			return {.handle_ 		= 	std::coroutine_handle<promise_type>::from_promise(*this)  };
		}
		std::suspend_never 			initial_suspend			() 					{ return {}; }
		std::suspend_never 			final_suspend			() 	noexcept 		{ return {}; }
		void 						unhandled_exception		() 					{}
		std::suspend_always 		yield_value				(unsigned value) 	{
			value_ 	= 	value;
			return 	{};
		}
	};

	std::coroutine_handle<promise_type> 	handle_;
};

//	Возвращаемый объект	-	структура управления сопрограммой
struct 	ReturnObject5 
{
	struct 	promise_type 
	{
		unsigned value_;
		
		~promise_type() {	
			cout << "promise_type destroyed" << endl;   
		}
		ReturnObject5 			get_return_object	() 					{	
			return { .handle_ = std::coroutine_handle<promise_type>::from_promise(*this) };
		}
		std::suspend_never 		initial_suspend		() 					{ return {}; }
		std::suspend_always 	final_suspend		() noexcept 		{ return {}; }
		void 					unhandled_exception	() {}
		std::suspend_always 	yield_value			(unsigned value) 	{
			value_ = value;
			return {};
		}
		void 					return_void			() 					{}
	};

	std::coroutine_handle<promise_type> 	handle_;
};

//	Возвращаемый объект	-	структура управления сопрограммой
struct 	ReturnObject5Even 
{
	struct 	promise_type 
	{
		unsigned value_;
		
		~promise_type() {	
			cout << "promise_type destroyed" << endl;   
		}
		ReturnObject5Even 			get_return_object	() 					{	
			return { .handle_ = std::coroutine_handle<promise_type>::from_promise(*this) };
		}
		std::suspend_never 		initial_suspend		() 					{ return {}; }
		std::suspend_never 		final_suspend		() noexcept 		{ return {}; }
		void 					unhandled_exception	() {}
		std::suspend_always 	yield_value			(unsigned value) 	{
			value_ = value;
			return {};
		}
		void 					return_void			() 					{}
	};

	std::coroutine_handle<promise_type> 	handle_;
};

// Универсальный генератор

template<typename T>
struct Generator 
{
	struct 	promise_type;
  
	using 	handle_type 	= 	std::coroutine_handle<promise_type>;

	struct 	promise_type 
	{
		T 						value_;
    
		std::exception_ptr 		exception_;

		Generator 				get_return_object				() 			{
			return Generator(handle_type::from_promise(*this));
		}
		std::suspend_always 	initial_suspend					() 			{ return {}; }
		std::suspend_always 	final_suspend					() noexcept { return {}; }
		void 					unhandled_exception				() 			{ exception_ = std::current_exception(); }
		
		template<std::convertible_to<T> From> 	// C++20 concept
		std::suspend_always 	yield_value						(From &&from) 	{
			value_ = std::forward<From>(from);
			return {};
		}
		void 					return_void						() 				{}
	};

	handle_type 				handle_;

	Generator			(handle_type handle) : handle_(handle) {}
	Generator			(const Generator &) 	= delete;
	~Generator			() 										{ handle_.destroy(); }
	
	explicit 	operator bool	() 			{
		fill();
		return !handle_.done();
	}
	T 			operator		()() 		{
		fill();
		full_ 	= false;
		return 	std::move(handle_.promise().value_);
	}

private:
	bool 	full_ 	= 	false;

	void 	fill				() 			{
		if (!full_) {
			handle_();
			if (handle_.promise().exception_)
				std::rethrow_exception(handle_.promise().exception_);
			full_ = true;
		}
	}
};

/*
	**************************************************************************************************************
	Редакционная статья


template<typename T>
struct 	Yield 
{
	T *		target_;
	Yield(T &t) : target_(&t) {}
	
	std::suspend_always 	operator()(const T &t) 	{ *target_ = t; }
	std::suspend_always 	operator()(T &&t) 		{ *target_ = std::move(t); }
};

template<typename T, bool Suspend = true>
struct ResumeWith 
{
	T 	value_;
	
	ResumeWith(const T &v) : value_(v) {}
	ResumeWith(T &&v) : value_(std::move(v)) {}
  
	constexpr 	bool 	await_ready		() 						const noexcept 	{ return !Suspend; }
	void 				await_suspend	(std::coroutine_handle) 					{}
	T 					await_resume		() 										{ return std::move(value_); }
};

struct 	HypotheticalReturnObject 
{
	std::coroutine_handle 	handle;
	bool 				done 	= 	false;
	unsigned 			val;

	ResumeWith<Yield<unsigned>> 	operator 	co_init	(std::coroutine_handle hh) 		{
		handle 		= hh;
		return 	ResumeWith(Yield(val));
	}
	std::suspend_always 	operator 	co_return() {
		done = true;
		return {};
	}
};

inline HypotheticalReturnObject&	hypothetical_counter()
{
	auto yield = co_init HypotheticalReturnObject{};

	for (unsigned i = 0; i < 3; ++i)
		co_await yield(i);
}
*/
