#pragma once
/*
	CPP20_Coroutines.hpp
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
	C++20. Coroutines
	https://habr.com/ru/articles/519464/
	--------------------------------------------------------------------------------------------
	
	
	--------------------------------------------------------------------------------------------
	Корутины
	
	Корутины (coroutines, сопрограммы) 	– 	это функции, 
											которые могут приостанавливать и продолжать свое выполнение, 
											сохраняя при этом свое состояние. 
											
	
*/
inline void bar_1(int a, int b){
	cout << "-------------- I'm inline void " <<  __func__  << "(int a, int b)" << endl;
}
inline void foo_1()
{
	cout << "-------------- I'm inline void " <<  __func__  << "()" << endl;
    int a = 1;
    int b = 2;
    bar_1(a, b);
}
/*
	Без каких-либо оптимизаций будет сгенерирован следующий код 
	(x86-64 clang 10.0.0 -m32, код сгенерирован в 32х битном окружении просто чтобы продемонстрировать работу стека. 
	
	По соглашению о вызовах для 64х битных систем, при передачи аргументов в функцию, 
	в таком простом случае, стек участвовать не будет, 
	аргументы будут переданы напрямую через регистры):


bar(int, int):
        push    ebp
        mov     ebp, 					esp
        mov     eax, 					dword ptr [ebp + 12]
        mov     ecx, 					dword ptr [ebp + 8]
        pop     ebp
        ret
foo():
        push    ebp 
        mov     ebp, 					esp
        sub     esp, 					24 		; 	8 байт для выравнивания, 
											;	2 х 4 для двух локальных int и 2 х 4 для двух int-аргументов для bar()
        mov     dword ptr [ebp - 4], 	1
        mov     dword ptr [ebp - 8], 	2
        mov     eax, 					dword ptr [ebp - 4]
        mov     ecx, 					dword ptr [ebp - 8]
        mov     dword ptr [esp], 		eax
        mov     dword ptr [esp + 4], 	ecx
        call    bar(int, int)
        add     esp, 					24
        pop     ebp
        ret
main:
        push    ebp							;	esp := esp - 4
        mov     ebp, 					esp		;	ebp := esp
        sub     esp, 					8  		;	esp := esp - 8
        call    foo()
        xor     eax, 					eax		;	Код возврата 0 для 	return	0;
        add     esp, 					8		;	esp := esp + 8
        pop     ebp							;	esp := esp + 4
        ret									;	return	0;
*/
/*
	Проиллюстрируем работу стека:

	Адреса оперативной памяти ростут снизу вверх от 0x0000 0000 до 0xFFFF FFFF
	
	0xFFFF FFFF
	0xFFFF FFFE
	....
	0xFFFF FFF0
	....
	0x0000 0011
	0x0000 0010
	0x0000 000F
	....
	0x0000 0002
	0x0000 0001
	0x0000 0000

	Стековый сегмент выделяется где-то в верхних адресах памяти,
	и стрек растет книзу.
	
	Например, команда помещения некоторого операнда в стек push 
	уменьшает значение регистра указателя стека sp
	на размер операнда (2, 4, 8 байт) 
	и копирует содержимое операнда в память 
	по адресу ss:sp.
	
	Например, комнада помещения в стек регистра указателя базы стекового кадра ebp 
	(это для 32-разрядной архитектуры)
	
	push	ebp
	
	уменьшает значение регистра указателя стека esp на четыре 
	
	Начало работы функции main, 
	на стеке лежит адрес возврата из функции, 
	пушим на стек значение регистра ebp (указатель базы стекового кадра) 
	т.к. дальше значение регистра 
	будет меняться на базу текущего стекфрейма, 
	сохраняем в ebp значение регистра esp (адрес вершины стека)

| ...            	  |
+----------------+
| return address   |
+----------------+
| saved rbp        |     <-- ebp, esp
+----------------+

	Выделяем необходимую память под локальные переменные и аргументы для вызова функции foo_1(). 
	
	Локальных переменных и аргументов у нас нет. 
	
	Но т.к. стек имеет выравнивание в 16 байт, 
	и на стеке уже лежит 8 байт (4 для адреса возврата и 4 для сохраненного ebp), 
	выделяем дополнительные 8 байт, смещая указатель стека.

	Вызываем функцию foo. 
	
	Команда call сохраняет на стеке адрес возврата 
	и передает управление вызываемой функции. 
	
	На стороне функции foo пушим на стек значение регистра ebp (указатель базы стекового кадра) 
	и сохраняем в ebp значение регистра esp (адрес вершины стека), 
	инициализируем новый стекфрйем.

	Выделяем необходимую память под локальные переменные и аргументы для вызова функции bar. 
	
	У нас две локальные переменные типа int — это 8 байт, 
	два аргумента для функции bar типа int — это 8 байт. 
	
	И т.к. у нас уже есть на стеке 8 байт (адрес возврата и сохраненный ebp), 
	нужно выделить еще 8 байт, 
	чтобы соблюсти требования к выравниванию. 
	
	Таким образом всего выделяем 8 + 8 + 8 = 24 байта, смещая указатель стека.

	Вызываем функцию bar. 
	
	Все работает так же, как и при вызове функции foo. 
	
	Команда call сохраняет на стеке адрес возврата и передает управление вызываемой функции. 
	
	На стороне функции bar пушим на стек значение регистра ebp 
	и сохраняем в ebp значение регистра esp, 
	инициализируем новый стекфрейм.

	Функция bar ничего не делает. 
	
	Восстанавливаем значение указателя базы предыдущего стекового кадра ebp (вызывающей стороны, функция foo) 
	и удаляем сохраненное значение со стека, 
	смещая указатель вершины стека на 4 байта вверх. 
	
	Забираем со стека адрес возврата 
	и удаляем сохраненное значение со стека 
	таким же смещением указателя вершины стека на 4 байта вверх. 
	
	Передаем управление обратно функции foo.

	Функция foo после вызова bar завершает свою работу. 
	
	Удаляем локальные переменные и аргументы предыдущего вызова, 
	смещаем указатель вершины обратно на 24 байта вверх.

	Восстанавливаем значение указателя базы предыдущего стекового кадра ebp 
	(вызывающей стороны, функция main) 
	и удаляем сохраненное значение со стека. 
	
	Забираем со стека адрес возврата 
	и удаляем сохраненное значение со стека. 
	
	Передаем управление обратно функции main.

	main завершает свою работу, 
	выполняя ровно те же действия, 
	что и предыдущие вызовы и мы возвращаемся в исходное состояние.

	Мы видим, что функции между вызовами 
	никак 'не сохраняют свое локальное состояние', 
	'каждый следующий вызов' 
	создаёт 'новый кадр' 
	и удаляет его по завершению работы.
*/
/*
static 	ucontext_t 	caller_context;
static 	ucontext_t 	coroutine_context;

inline 	void print_hello_and_suspend()
{
	cout << "----ВХОД------ I'm void " <<  __func__  << "()" << endl;
    // выводим Hello 
    cout << "Hello";
    // точка передачи управления вызывающей стороне, 
    // переключаемся на контекст caller_context
    // в контексте сопрограммы coroutine_context сохраняется текущая точка выполнения,
    // после возвращения контроля, выполнение продолжится с этой точки.
    swapcontext(&coroutine_context, &caller_context);
	cout << "----ВЫХОД----- I'm void " <<  __func__  << "()" << endl;
}
inline 	void 	simple_coroutine()
{
	cout << "----ВХОД------ I'm void " <<  __func__  << "()" << endl;
    // точка первой передачи управления в coroutine_context
    // чтобы продемонстрировать преимущества использование стека
    // выполним вложенный вызов функции print_hello_and_suspend().
    print_hello_and_suspend();
    // функция print_hello_and_suspend() приостановила выполнение сопрограммы
    // после того как управление вернётся мы выведем Coroutine! и завершим работу,
    // управление будет передано контексту,
    // указатель на который хранится 
	// в 
	// coroutine_context.uc_link, 
	// т.е. caller_context
    cout << "Coroutine!" << endl;
	cout << "----ВЫХОД------ I'm void " <<  __func__  << "()" << endl;
}
Эта архитектура имеет место только под POSIX OS
*/
/*
	***************************************************************************************************
	C++20.

	Техническая спецификация сопрограмм в новом C++ 
	носит название 'Coroutine TS'. 
	
	Coroutine TS 	-	предоставляет собой низкоуровневые средства,
						обеспечивающие характерную возможность передачи управления, 
						описывает обобщенный механизм взаимодействия и настройки сопрограмм 
						и набор вспомогательных высокоуровневых типов стандартной библиотеки,
						задача которых сделать разработку сопрограмм более доступной и безопасной.

	--------------------------------------------------------------------------------------------------
	User types.

	С сопрограммами ассоциировано несколько 'интерфейсных типов', 
	позволяющих настраивать поведение сопрограммы и контролировать семантику операторов.
	----------------------------
	Promise.

	Шаблон std::coroutine_traits определен следующем образом:
*/
template 	<	typename 	Ret
			, 	typename 	= std::void_t<>
			>
struct 	coroutine_traits_base
{};
template <typename Ret>
struct 	coroutine_traits_base	<	Ret
								, 	std::void_t<typename Ret::promise_type>
								>
{
	using 	promise_type 	= 	typename Ret::promise_type;
};

template <typename  Ret, 	typename... Ts>
struct 	coroutine_traits : coroutine_traits_base<Ret>
{};
/*
	Тип должен иметь строгое имя 'promise_type'. 
	
	Из определения std::coroutine_traits следует, 
	что существует как минимум одна специализация, 
	которая ищет определение типа 'promise_type' 
	в пространстве имен типа возвращаемого результата. 
	
	'promise_type' может быть как именем типа, так и псевдонимом.
	
	Самый простой способ определения типа Promise для сопрограммы.
*/
struct Task_coroutine	
{
    struct Promise    {
        // ...
    };
    
	using promise_type = Promise;
};
//...
Task_coroutine 	foo_3()
{
    //	...
	return {};
}
/*
	Также определение подобного типа 'Task' 
	полезно в более сложных ситуациях, 
	тип может определять 
	'дополнительную семантику' внешнего оперирования сопрограммой: 
		-	передавать и получать данные, 
		-	передавать поток управления (пробуждать) 
		или 
		-	уничтожать сопрограмму.

	Другой способ определить тип Promise — 
	это явно специализировать шаблон std::coroutine_traits. 
	
	Это удобно, например, для сопрограмм представленных методом пользовательского типа
*/
class 	Coroutine	{
public:
    void call(int);
};
namespace std
{
    template<>
    struct coroutine_traits	<	void
							, 	Coroutine
							, 	int
							>
    {
        using 	promise_type = Coroutine;
    };
}
/*
	---------------------------------------------------------------------------------------
	Awaitable.
*/
void 	sleeping(std::chrono::seconds 	peroid){
	cout << "-------------- I'm void " <<  __func__  << "() - I will sleep for " << peroid << endl;
	std::this_thread::sleep_for(peroid);
	cout << "-------------- I'm void " <<  __func__  << "() - I've waked up" << peroid << endl;
}
//Task_coroutine 	
void foo3()
{
	cout << "----СТАРТ---------- I'm void " << __func__ << "()" << endl;
    using 	namespace 	std::chrono_literals;

    // выполнить некоторый набор операций
    // вернуть управление
	cout << "----Вернем управление вызывающей стороне---------- I'm void " << __func__ << "()" << endl;
    // co_await sleeping(2s);
	sleeping(2s);
	cout << "----Снова получила управление---------- I'm void " << __func__ << "()" << endl;
    // через 10 секунд выполнить еще один набор операций.
	cout << "----КОНЕЦ РАБОТЫ---------- I'm void " << __func__ << "()" << endl;
};
/*
	--------------------------------------------------------------------------------
	https://en.cppreference.com/w/cpp/language/coroutines 
*/	
#if defined(__clang__)
inline	auto switch_to_new_thread(std::thread& out)	{
#else	
inline	auto switch_to_new_thread(std::jthread& out)	{
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
inline	task_coroutine 	resuming_on_new_thread(std::thread& out)
#else	
inline	task_coroutine 	resuming_on_new_thread(std::jthread& out)
#endif
{
	cout << "-------------- I'm task_coroutine " <<  __func__  << "(std::jthread& out)" << endl;
    cout << "Coroutine started on thread: " << std::this_thread::get_id() << '\n';
    co_await 	switch_to_new_thread(out);
    // awaiter destroyed here
    cout << "Coroutine resumed on thread: " << std::this_thread::get_id() << '\n';
};
/*
	В примере выше выше 
	выражение переданное в качестве операнда имеет тип std::chrono::duration<long long>, 
	чтобы скомпилировать этот код 
	нужно определить перегрузку оператора co_await для выражений такого типа.
*/
template<typename Rep, typename Period>
auto 	operator 	co_await(std::chrono::duration<Rep, Period> duration) noexcept
{
    struct Awaitable
    {
        explicit Awaitable(std::chrono::duration<Rep, Period> 	duration) : duration_(duration)
        {}
        //...
	private:
        std::chrono::duration<Rep, Period>  	duration_{};
    };

    return Awaitable{duration};
}
/*
	Внутри перегрузки мы описываем тип Awaitable, 
	задача которого запланировать и вернуть управление сопрограмме 
	через заданный промежуток времени, 
	и возвращаем объект данного типа как результат.

	Нам осталось определить интерфейс типа Awaitable и, 
	чтобы это сделать, 
	рассмотрим подробнее вызов оператора 
	
	co_await <expr> 
	
	и код, 
	который компилятор генерирует в месте вызова.
*/
/*
	Определен выше
template<typename T, typename ... Args>
using 	coroutine_traits_ 	= std::coroutine_traits<std::future<T>, Args...>;
*/
inline void any_co_await()
{
    // в начале мы определили тип Promise
	
	struct Promise    {
        // ...
    };
    
	using 	promise_type = Promise;
	
    // using 	promise_type 		= typename coroutine_traits::promise_type;

    // 	...
    // 	вызов co_await <expr> в рамках сопрограммы

    // 	1.
    // 	Создаем объект типа Awaitable, 
	struct Awaitable    {	
		bool 		await_ready() 												{ return false; }
		bool 		await_suspend(std::coroutine_handle<> h)    				{ return true;  }
        void 		await_resume()												{}
	};
	struct	Frame{
		Awaitable	awaitable;
		Awaitable	create_awaitable()	{ return{}; }
	};
	Frame*	frame;
	//	находим подходящую перегрузку оператора co_await,
    // 	результат сохраняем во фрейме сопрограммы 
	//	(как создается фрейм мы рассмотрим в рамках описания типа Promise), 
	// это необходимо,
	// 	т.к. с помощью Awaitable мы вернем результат вычисления, после возобновления работы.
    frame->awaitable 	= 	frame->create_awaitable();

    // 2.
    // Вызываем метод await_ready().
    // Основная задача метода позволить нам избежать остановки сопрограммы
    // в случаях когда вычисления могут быть завершены синхронно
    // или уже завершены, сохранив вычислительные ресурсы.
    if (!frame->awaitable.await_ready())
    {
        // 3.
        // Если вызов await_ready() вернул false,
        // то сопрограмма приостанавливает свое выполнение,
        // сохраняет состояние: состояние локальных переменных, точку остановки
        // (это идентификатор состояния, на которое сопрограмма перейдет
        // после возобновления своей работы, 
        // достаточная информация что бы перейти в точку <resume-point>)

		//	<suspend-coroutine>
		frame->awaitable.await_suspend(std::coroutine_handle<>{});
        
        // 4.
        // Определяем тип coroutine_handle
        // corotine_handle - это дескриптор фрейма сопрограммы.
        // он обеспечивает низкоуровневую функциональность оперирования сопрограммой:
        // передача управления (возобновление выполнения) и удаление.

        using 	handle_type 				= std::coroutine_handle<promise_type>;
        using 	await_suspend_result_type 	=
								decltype(frame->awaitable.await_suspend(std::coroutine_handle<promise_type>{}));

        // 5.
        // Вызов метода await_suspend(handle), 
        // задача метода await_suspend выполнить некоторую логику
        // на клиентской стороне после приостановления выполнения сопрограммы
        // для дальнейшего планирования возобновления ее работы (если необходимо). 
        // Метод принимает один аргумент - дескриптор сопрограммы.
        // Тип возвращаемого результата, определяет семантику передачи управления

        if constexpr (std::is_void_v<await_suspend_result_type>)
        {
            // Тип возвращаемого результата void,
            // мы безусловно передаем управление вызывающей стороне
            // (под вызывающей стороной здесь понимается сторона,
            // которая передала управление сопрограмме)
            frame->awaitable.await_suspend(std::coroutine_handle<promise_type>{});
            //<return-to-caller-or-resumer>;
        }
        else if constexpr (std::is_same_v<await_suspend_result_type, bool>)
        {
            // Тип возвращаемого результата bool,
            // если метод вернул false, то управление не передается вызывающей стороне
            // и сопрограмма возобновляет свое выполнение
            // Это полезно, например, когда асинхронная операция
            // инициированная объектом Awaitable завершилась синхронно
            if (frame->awaitable.await_suspend(std::coroutine_handle<promise_type>{}))
                ;//<return-to-caller-or-resumer>;
        }
        else if constexpr (std::coroutine_handle<await_suspend_result_type>{}.address() != nullptr)
        {
            // Тип возвращаемого результата std::coroutine_handle<OtherPromise>,
            // т.е. вызов возвращает дескриптор другой сопрограммы,
            // то мы передаем управление этой сопрограмме, это семантика позволяет
            // эффективно реализовывать симметричный механизм передачи потока 
            // управления между сопрограммами
            auto&& other_handle = frame->awaitable.await_suspend(std::coroutine_handle<promise_type>{});
			
            frame->awaitable.await_resume();
        }
        else
        {
            static_assert(true);
        }
    }

    // 6.
    // Точка возобновления выполнения (пробуждения)
    // Вызов метода await_resume(). Задача метода получить результат вычисления.
    // Возвращаемое значение рассматривается как результат вызова оператора co_await.
resume_point:
    return frame->awaitable.await_resume();
}
