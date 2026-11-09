#pragma once
/*
	CPP20_Coroutines_problem_of_DispatcherQueue_task.hpp
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

/*	
	***************************************************************************************************
	C++ coroutines: The problem of the DispatcherQueue task that runs too soon
	
	--------------------------------------------------------------------------------------------
	
	
	--------------------------------------------------------------------------------------------
*/


inline void any_code()
{
	cout << "-------------- I'm void " <<  __func__  << "()" << endl;
}
