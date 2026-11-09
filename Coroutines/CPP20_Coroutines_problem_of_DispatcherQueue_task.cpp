/*
	CPP20_Coroutines_problem_of_DispatcherQueue_task.cpp
*/	
#include	<iostream>
#include	<chrono>
using namespace std::chrono_literals;
#include <coroutine>
#include <stdexcept>
#include <thread>
//#include <ucontext.h>
using std::cout; using std::endl; using std::boolalpha;
#include	"CPP20_Coroutines_problem_of_DispatcherQueue_task.hpp"
#include	"CPP20_Coroutines_problem_of_DispatcherQueue_task_Part_1.hpp"
#include	"CPP20_Coroutines_problem_of_DispatcherQueue_task_Part_3.hpp"
/*
	**********************************************************************************************
	C++ coroutines: The problem of the DispatcherQueue task that runs too soon
	Сопрограммы C++: проблема задачи DispatcherQueue, которая запускается слишком рано
	https://devblogs.microsoft.com/oldnewthing/20191223-00/?p=103255
	--------------------------------------------------------------------------------------------
	Раймонд Чен			23 декабря 2019 г.
	Raymond Chen		December 24th, 2019
	--------------------------------------------------------------------------------------------
	
*/
/*	
	**************************		main()		***************************************************
*/
int main(){
	cout << "-------------- I'm int " << __func__ << "() of CPP20_Coroutines_problem_of_DispatcherQueue_task.cpp" << endl;
	
	cout << endl;
	any_code();
	any_code_part_1();
	// test_1();	cout << endl;
	
	/*
	*/
	any_code_part_3();
};	