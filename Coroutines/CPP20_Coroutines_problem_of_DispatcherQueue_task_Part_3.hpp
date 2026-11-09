#pragma once
/*
	CPP20_Coroutines_problem_of_DispatcherQueue_task_Part_3.hpp
*/
#include	<chrono>
using namespace std::chrono_literals;
#include	<thread>
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
//#include <ucontext.h>
// Для clang и gcc математические библиотеки в разных заголовках описаны
#if defined(__clang__)
	#include <math.h>
#else	
	#include <cmath>
#endif

#include "..\type_name_as_str.h"
#include	"CPP20_Coroutines_problem_of_DispatcherQueue_task.hpp"
/*	
	***************************************************************************************************
	C++ coroutines: 		The problem of the DispatcherQueue task that runs too soon
	Сопрограммы C++: 	проблема задачи DispatcherQueue, которая запускается слишком рано, часть 3
	https://devblogs.microsoft.com/oldnewthing/20191225-00/?p=103265
	--------------------------------------------------------------------------------------------
	Раймонд Чен			23 декабря 2019 г.
	Raymond Chen		December 24th, 2019
	--------------------------------------------------------------------------------------------
*/
inline auto resume_foreground(DispatcherQueue const& 	dispatcher)
{
	struct awaitable
	{
		DispatcherQueue 	m_dispatcher;
		
		bool 				m_queued 	= 	false;
		slim_event 			ready;

		bool 	await_ready()		{
			return false;
		}

		bool await_suspend(coroutine_handle<> handle)	
		{
			bool 	result 	= 	m_dispatcher.TryEnqueue([this, handle]
			{
				ready.wait();
				handle();
			});
			m_queued 	= 	result;
			ready.signal();
		  
			return 	result;
		}

		bool 	await_resume()
		{
			return 	m_queued;
		}
	};
	return awaitable{dispatcher};
}

inline void any_code_part_3()
{
	cout << "-------------- I'm void " <<  __func__  << "()" << endl;
}
