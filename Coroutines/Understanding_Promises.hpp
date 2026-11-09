#pragma once
/*
	Understanding_Promises.hpp
*/
// Example code for blog post 'Understanding Promises'
//
// Copyright (c) Lewis Baker

#include <coroutine>
#include <atomic>

struct task_structure
{
	struct promise_type
	{
		task_structure		get_return_object	() { return {}; }
	
		std::suspend_never 	initial_suspend		() { 
			cout << "+++ I'm std::suspend_never " <<  __func__  << "()" << endl;
			return {}; 
		}
		
		std::suspend_always 	you_can_suspend_me		() { 
			cout << "+++ I'm std::suspend_always " <<  __func__  << "()" << endl;
			return {}; 
		}
		std::suspend_never 		you_cannot_suspend_me		() { 
			cout << "+++ I'm std::suspend_never " <<  __func__  << "()" << endl;
			return {}; 
		}
		std::suspend_never 	final_suspend		() noexcept { 
			cout << "+++ I'm std::suspend_never " <<  __func__  << "()" << endl;
			return {}; 
		}
		
		void 				return_void			() {}
		
		void 				unhandled_exception	() {
			cout << "+++ I'm void " <<  __func__  << "()" << endl;
		}
	};
};
inline task_structure 	first_promis_using()
{
	cout << "-------------- I'm inline task_structure " <<  __func__  << "()" << endl;
	
	task_structure::promise_type	promise;
	
	{
		cout << "--- into first_promis_using() --- co_await 	promise.initial_suspend();" << endl;
		
		co_await 	promise.you_can_suspend_me();
		//co_await 	promise.initial_suspend();
		
		int int_var	=	21;
		cout << "int int_var	=	21; " << int_var << endl;
		
		//co_await 	promise.you_can_suspend_me();
		cout << "first_promis_using() sleeps" << endl;
		std::this_thread::sleep_for(1000ms);
		//co_await 	promise.you_cannot_suspend_me();
		try
		{
			// <body-statements>
			int_var	*=	21;
			cout << "int_var	*=	21; " << int_var << endl;
		}
		catch (...)
		{
			promise.unhandled_exception();
		}
	FinalSuspend:
		cout << "--- into first_promis_using() --- co_await 	promise.final_suspend();" << endl;
		co_await 	promise.final_suspend();
	}
}	
/*
	--------------------------------------------------------------------------------------------------------
	Настройка распределения памяти кадров сопрограммы
*/
void* 	my_custom_allocate	(std::size_t size)				{ return {}; };
void 	my_custom_free		(void* ptr, std::size_t size)	{};

struct my_promise_type
{
	void* operator new(std::size_t size)
	{
		void* ptr = my_custom_allocate(size);
		if (!ptr) 
			throw std::bad_alloc{};
		return ptr;
	}

	void operator delete(void* ptr, std::size_t size)
	{
		my_custom_free(ptr, size);
	}

	//	...
};

template<typename ALLOCATOR>
struct my_promise_type_1
{
	template<typename... ARGS>
	void* 	operator new	(	std::size_t 	sz
							, 	std::allocator_arg_t
							, 	ALLOCATOR& 		allocator
							, 	ARGS&... args
							)
	{
		// Round up sz to next multiple of ALLOCATOR alignment
		std::size_t 	allocatorOffset =	(sz + alignof(ALLOCATOR) - 1u) & ~(alignof(ALLOCATOR) - 1u);

		// Call onto allocator to allocate space for coroutine frame.
		void* 			ptr 			= 	allocator.allocate(allocatorOffset + sizeof(ALLOCATOR));

		// Take a copy of the allocator (assuming noexcept copy constructor here)
		new 	(((char*)ptr) + allocatorOffset) ALLOCATOR(allocator);

		return 	ptr;
	}

	void 	operator delete(void* ptr, std::size_t sz)
	{
		std::size_t 	allocatorOffset =	(sz + alignof(ALLOCATOR) - 1u) & ~(alignof(ALLOCATOR) - 1u);

		ALLOCATOR& 		allocator 		= 	*reinterpret_cast<ALLOCATOR*>(((char*)ptr) + allocatorOffset);

		// Move allocator to local variable first so it isn't freeing its own memory from underneath itself.
		// Assuming allocator move-constructor is noexcept here.
		ALLOCATOR 		allocatorCopy 	= 	std::move(allocator);

		// But don't forget to destruct allocator object in coroutine frame
		allocator.~ALLOCATOR();

		// Finally, free the memory using the allocator.
		allocatorCopy .deallocate(ptr, allocatorOffset + sizeof(ALLOCATOR));
	}
};
/*
namespace std::experimental
{
	template<typename ALLOCATOR, typename... ARGS>
	struct 	coroutine_traits	<	my_return_type
								, 	std::allocator_arg_t
								, 	ALLOCATOR
								, 	ARGS...
								>
	{
		using 	promise_type 	= 	my_promise_type<ALLOCATOR>;
	};
}
*/
// Pretend there's a compiler-generated structure called 'coroutine_frame'
// that holds all of the state needed for the coroutine. It's constructor
// takes a copy of parameters and default-constructs a promise object.
struct coroutine_frame { 
	task_structure::promise_type	promise;
//... 
};

template<typename T, typename P>
T 	some_coroutine(P 	param)
{
	typename T::promise_type		promise;
	
	auto* 							f 				= 	new 	coroutine_frame(std::forward<P>(param));

	auto 							returnObject 	= 	f->promise.get_return_object();

	// Start execution of the coroutine body by resuming it.
	// This call will return when the coroutine gets to the first
	// suspend-point or when the coroutine runs to completion.
	std::coroutine_handle<decltype(f->promise)>::from_promise(f->promise).resume();

	// Then the return object is returned to the caller.
	return returnObject;
}

