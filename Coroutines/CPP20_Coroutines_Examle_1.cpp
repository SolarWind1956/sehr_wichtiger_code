/*
	CPP20_Coroutines_Examle_1.cpp
*/	
#include	<iostream>
#include	<chrono>
using namespace std::chrono_literals;
#include <coroutine>
#include <stdexcept>
#include <thread>
//#include <ucontext.h>
using std::cout; using std::endl; using std::boolalpha;

/*
	**********************************************************************************************
	https://en.cppreference.com/w/cpp/language/coroutines 
*/
#include <coroutine>
#include <iostream>
#include <stdexcept>
#include <thread>
/* 
auto switch_to_new_thread(std::jthread& out)
{
    struct awaitable
    {
        std::jthread* p_out;
        bool await_ready() { return false; }
        void await_suspend(std::coroutine_handle<> h)
        {
            std::jthread& out = *p_out;
            if (out.joinable())
                throw std::runtime_error("Output jthread parameter not empty");
            out = std::jthread([h] { h.resume(); });
            // Potential undefined behavior: accessing potentially destroyed *this
            // std::cout << "New thread ID: " << p_out->get_id() << '\n';
            std::cout << "New thread ID: " << out.get_id() << '\n'; // this is OK
        }
        void await_resume() {}
    };
    return awaitable{&out};
}
 
struct task
{
    struct promise_type
    {
        task get_return_object() { return {}; }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() {}
    };
};
 
task resuming_on_new_thread(std::jthread& out)
{
    std::cout << "Coroutine started on thread: " << std::this_thread::get_id() << '\n';
    co_await switch_to_new_thread(out);
    // awaiter destroyed here
    std::cout << "Coroutine resumed on thread: " << std::this_thread::get_id() << '\n';
}
*/

#if defined(__clang__)
	auto switch_to_new_thread(std::thread& out)	{
#else	
	auto switch_to_new_thread(std::jthread& out)	{
#endif
	cout << "-------------- I'm auto " <<  __func__  << "(std::jthread& out)" << endl;
    struct awaitable
    {	
		#if defined(__clang__)
			std::thread* 	p_out;
		#else	
			std::jthread* 	p_out;
		#endif

        bool await_ready() 	{ return false; }
		
        void await_suspend(std::coroutine_handle<> h)
        {
			#if defined(__clang__)
				std::thread& 	out = *p_out;
			#else	
				std::jthread& 	out = *p_out;
			#endif
            if (out.joinable())
                throw std::runtime_error("Output jthread parameter not empty");
			#if defined(__clang__)
				out = std::thread([h] { h.resume(); });
			#else	
				out = std::jthread([h] { h.resume(); });
			#endif
            
            // Potential undefined behavior: accessing potentially destroyed *this
            // std::cout << "New thread ID: " << p_out->get_id() << '\n';
            cout << "New thread ID: " << out.get_id() << '\n'; // this is OK
        }
        void await_resume() {}
    };
    return awaitable{&out};
}
 
struct task_coroutine
{
    struct promise_type
    {
        task_coroutine 		get_return_object		() 				{ return {}; }
        std::suspend_never 	initial_suspend			() 				{ return {}; }
        std::suspend_never 	final_suspend			() noexcept 	{ return {}; }
        void 				return_void				() 				{}
        void 				unhandled_exception		() 				{}
    };
};
#if defined(__clang__)
	task_coroutine 	resuming_on_new_thread(std::thread& out)
#else	
	task_coroutine 	resuming_on_new_thread(std::jthread& out)
#endif
{
	cout << "-------------- I'm task_coroutine " <<  __func__  << "(std::jthread& out)" << endl;
    cout << "Coroutine started on thread: " << std::this_thread::get_id() << '\n';
    co_await 	switch_to_new_thread(out);
    // awaiter destroyed here
    cout << "Coroutine resumed on thread: " << std::this_thread::get_id() << '\n';
};

/*
	**************************		main()	***************************************************
*/ 
int main()
{
	#if defined(__clang__)
		std::thread 	out;
		resuming_on_new_thread(out);
		out.join();
	#else	
		std::jthread 	out;
		resuming_on_new_thread(out);
	#endif
}
