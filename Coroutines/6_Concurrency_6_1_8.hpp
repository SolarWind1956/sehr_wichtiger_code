#pragma once
/*
	6_Concurrency_6_1_8.hpp
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
	
	********************************************************************************************************
	6.1.8 	co_await
	
	co_await вызывает приостановку или продолжение выполнения сопрограммы. 
	
	Выражение exp в 
	
	co_await exp 
	
	должно быть ожидаемым выражением (awaitable expression), 
	т.е. должно реализовывать конкретный интерфейс, 
	состоящий из трех методов: 
	await_ready(), await_suspend() и await_resume().
	
	Типичным примером использования co_await является сервер, 
	ожидающий событий.
	
	Блокирующий сервер
	
Acceptoracceptor{443};

while(true){
	Socket	socket		=	acceptor .accept(); 			//blocking
	auto	request		=	socket .read(); 				//blocking
	auto	response	=	handleRequest(request);
	
	socket .write(response); 								//blocking
}
 
	Сервер довольно прост, 
	поскольку он по очереди отвечает на каждый запрос в одном и том же потоке. 
	
	Сервер слушает порт 443, 
	подтверждает соединение, 
	читает входные данные от клиента
	и посылает ответ клиенту. 
	
	Вызовы в строках 3, 4 и 6 являются блокирующими.
	
	Благодаря co_await блокирующие вызовы 
	могут быть приостановлены и возобновлены.
	
	Ожидающий сервер
	
Acceptoracceptor{443};

while(true){
	Socket	socket		=	co_await		acceptor .accept();
	auto	request		=	co_await		socket.read();
	auto	response	=	handleRequest(request);
	
	co_await		socket .write(response);
}
	Прежде чем я перейду к действительно интересному примеру 
	с синхронизацией потоков при помощи сопрограмм, 
	я хотел бы начать с чего-то, достаточно простого: 
	начала нового задания (job) по запросу.
 
	---------------------------------------------------------------------------------------
	6.1.8.1 Создание задания по запросу
 
	Сопрограмма в следующем примере настолько проста, 
	насколько это возможно. 
	
	Она ожидает предопределенного ожидаемого объекта std::suspend_never().
	
	Создание задания по запросу
*/
struct 	Job 
{ 
    struct 	promise_type;
    using 	handle_type = std::coroutine_handle<promise_type>;
	
    handle_type 	coro;
	
    Job(handle_type hndl)	: coro(hndl)
	{}
	
    ~Job() {
        if ( coro ) 
			coro.destroy();
    }
	
    void start() {
        coro.resume();
    }

    struct 	promise_type 
	{
        auto get_return_object() { 
            return Job{handle_type::from_promise(*this)};
        }
		
        std::suspend_always initial_suspend() { 
            cout << "    Preparing job" << '\n';
            return {}; 
        }
        
		std::suspend_always final_suspend() noexcept { 
            cout << "    Performing job" << '\n'; 
            return {}; 
        }
		
        void return_void() 
		{}
        
		void unhandled_exception() 
		{}
    };
};
 
Job prepareJob() {
    co_await std::suspend_never();
}
 
void	test_7(){
	cout << "-------------- I'm void " <<  __func__  << "()" << endl;

    cout <<  "Before job" << '\n';

    auto job = prepareJob();
    
	job.start();

    cout <<  "After job" <<  '\n';
}
/*
	Вы можете подумать, что сопрограмма prepareJob() бессмысленна, 
	поскольку ожидаемый объект всегда приостанавливается. 
	
	Это не так. 
	
	Функция prepareJob() – это фабрика сопрограмм, 
	использующая co_await и возвращающая объекты-сопрограммы. 
	
	Функция prepareJob() создает сопрограмму типа Job. 
	
	Эта сопрограмма сразу же приостанавливается, 
	поскольку метод объекта-обещания возвращает std::suspend_always.
	
	Именно по этой причине необходим вызов job.start() 
	для возобновления сопрограммы. 
	
	Метод final_suspend() также возвращает std::suspend_always.
	
	
*/
