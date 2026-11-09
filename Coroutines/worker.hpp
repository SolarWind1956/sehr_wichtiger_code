/*
    Coroutines article.
    Class for worker thread. Creates thread and queue for data safe usage.
*/
#pragma once

#include <condition_variable>
#include <coroutine>
#include <queue>
#include <thread>

#include "assingleton.hpp"

enum class WorkerType { Coroutine, Function };

/// @brief Class for thread of all multithread sensetive data modification.
template <WorkerType Type = WorkerType::Coroutine>
class WorkThread : public AsSingleton<WorkThread<Type>> {

#if defined(__clang__)
	std::thread 	m_Worker;
#else	
	std::jthread 	m_Worker;
#endif

	std::condition_variable 	m_condVar;
	
	std::queue	<	std::conditional_t	<	Type == WorkerType::Coroutine
										,	std::coroutine_handle<>
										, 	std::function<void()>
										>
				>				m_queue;
	
	std::mutex 					m_mutex;

public:
	void 	start() {
		if (m_Worker.joinable())
			return;
	
	#if defined(__clang__)
		m_Worker = std::thread	{	[this]() {
	#else	
		m_Worker = std::jthread	{	[this]() {
	#endif
				auto stoken = m_Worker .get_stop_token();
			
				std::mutex 	mutex;
				
				while (true) {
					std::unique_lock 	lk{mutex};
					
					m_condVar .wait	(lk, 	[this, stoken] {
												std::lock_guard 	lg{m_mutex};
												return m_queue .size() || stoken .stop_requested();
											}
									);
					if (stoken .stop_requested())
						break;
					{
						std::lock_guard 	lg{m_mutex};
					
						if constexpr (Type == WorkerType::Coroutine)
							m_queue .front().resume();
						else
							m_queue .front()();
					
						m_queue .pop();
					}
				}
			}
		};
	}
	void 	finish() {
		if (m_Worker  .joinable()) {
			m_Worker  .get_stop_source() .request_stop();
			m_condVar .notify_one();
			m_Worker  .join();
		}
	}
	void 	addJob	(std::conditional_t	<	Type == WorkerType::Coroutine
										,   std::coroutine_handle<>
										, 	std::function<void()>
										>   handle
					) 
	{
		std::lock_guard 	lg{m_mutex};
		
		m_queue   .emplace(handle);
		m_condVar .notify_one();
	}
	virtual ~WorkThread() { finish(); }
};

template <WorkerType 	Type> 
struct 	WorkerHelper {
	WorkerHelper() 	{ WorkThread<Type>::get().start(); }
	~WorkerHelper() { WorkThread<Type>::get().finish(); }
};

template <typename T> 
struct returnType 
{
	struct promise_type {

		auto 					get_return_object	() 			{
			return returnType{std::coroutine_handle<promise_type>::from_promise(*this)};
		}
		
		std::suspend_never 		initial_suspend		() 			{ return {}; }
		std::suspend_always 	final_suspend		() noexcept { return {}; }
		void 					unhandled_exception	() 			{}
		void 					return_value		(T value) 	{
			m_value = value;
			m_promise .set_value(value);
		}
		T 						get_direct			() 			{ return m_value; }
		T 						get_async			() 			{ return m_promise.get_future().get(); }

	private:
		T 						m_value;
		std::promise<T> 		m_promise;
	};

	returnType(std::coroutine_handle<promise_type> handle) : m_handle(handle) {}

	std::coroutine_handle<> 	handle				() const 	{ return m_handle; }

	T 							get					() const 	{ return m_handle.promise().get_direct(); }
	T	 						get_async			() const 	{ return m_handle.promise().get_async(); }
private:
	std::coroutine_handle<promise_type> 	m_handle;
};

struct awaitExecution {
	bool 	await_ready								() 									const 	{ return false; }
	void 	await_suspend							(std::coroutine_handle<> handle) 	const 	{
		WorkThread<WorkerType::Coroutine>::get().addJob(handle);
	}
	void 	await_resume							() 									const 	{}
};
