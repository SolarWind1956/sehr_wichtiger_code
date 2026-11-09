#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <iostream>
#include <windows.h>
#include <time.h>
#include "TimerBase.h"

/*  class Stopwatch -- simple stopwatch timer for optimization

    Секундомер выводит информацию об истекшем времени для измерения времени работы длительных задач. 
    
    Секундомер также может возвращать истекшее время без вывода на печать.

    Секундомер предназначен для использования с идиомой RAII. 
	В своем простейшее использование, блок 
    {
        Stopwatch sw("TheThing()");
        TheThing();
    }
    производит вывод двух строк

        TheThing(): start
        // anything printed by TheThing()
        TheThing(): stop 63mS

    Секундомер призван заменить что-то более сложное, например 
    {
        timeval     t1, t2;
        long        dt;

        gettimeofday(&t1, NULL);
        TheThing();
        gettimeofday(&t2, NULL);
        
        dt = (t2.tv_sec * 1'000'000 + t2.tv_usec) - (t1.tv_sec * 1'000'000 + t1.tv_usec);
        
        cout << "TheThing() took " << dt / 1'000 << " mS" << endl;
    }

    которое выводит на печать:
	
        ctor():							"Stopwatch: start"
        ctor(false):					nothing printed
        Show():							"Stopwatch: show at xxxx mS"
                                        sets lap time. Get with LapGet()
        Stop():							"Stopwatch: stop xxxx mS"
                                        sets lap time. Get with LapGet()
        Stop() when not running:		"Stopwatch: not started"
        Start():						"Stopwatch: start"
                                        clears lap time
        Start() when running:			"Stopwatch: start xxxx mS"
                                        sets lap time. Get with LapGet()

    Что печатает при указании названия активности 
	
        ctor("activity"):				"activity: start"
        Show("theEvent"):				"activity: theEvent at xxxx mS"
        Stop("theEvent"):				"activity: theEvent xxxx mS"
                                        sets lap time. Get with LapGet()
        Stop("theEvent") not running:	"activity: not started"
        Start("theEvent") running:		"activity: theEvent xxxx mS"
                                        sets lap time. Get with LapGet()
        Start("theEvent") not running: "activity: theEvent"
                                        clears lap time

   печать может быть подавлена (получить время с помощью LapGet () )
   
        ctor("") or ctor(nullptr)		prints nothing
        Show("") or Show(nullptr)		prints nothing, sets lap time
        Start("") or Start(nullptr):	prints nothing, sets lap time if running
        Stop("") or Stop(nullptr):		sets lap time. Get with LapGet()
*/
//  ******************  basic_stopwatch  -    КЛАСС ПРОСТОГО СЕКУНДОМЕРА *****************************************
template <typename T> 
class basic_stopwatch : public T 
{
public:
    typedef T 				BaseTimer;
	typedef ULONGLONG 	    tick_t;

    //  Создание и необязательный запуск тайминга некоторого действия - create, optionally start timing an activity
    explicit 	basic_stopwatch(bool start);

    explicit 	basic_stopwatch(char const* activity = "Stopwatch", bool start = true);

				basic_stopwatch(std::ostream& log, char const* activity = "Stopwatch", bool start = true);

    // Остановка и уничтожение секундомера  -   stop and destroy a stopwatch
    ~basic_stopwatch();

    // Получение времени последнего этапа (время последней остановки)  -   get last lap time (time of last stop)
    tick_t 		LapGet() const;

    // Предикат : возвращает true, если секундомер работает   -   predicate: return true if the stopwatch is running
    bool 		IsStarted() const;

    // Показывает накопленное время, продолжая работать   -   show accumulated time, keep running, set/return lap
    tick_t 		Show(char const* event = "show");

    // (Пере)запуск секундомера, установка/возврат времени этапа     -   (re)start a stopwatch, set/return lap time
    tick_t 		Start(char const* event_name = "start");
    
    std::chrono::system_clock::time_point 		get_m_start()
    {
        return T::m_start;
    };
	// Получить истекшее время в тиках  -   get elapsed time in ticks
	tick_t 		GetTicks() 
	{
		if ( IsStarted() ) 
		{
            /*
			tick_t now 	= clock();
			tick_t dt  	= now - m_start;
            */
            auto now 	= std::chrono::system_clock::now();
            auto dt 	= now - BaseTimer::m_start;
            // std::cout << type_name_as_str<decltype(dt)>() << std::endl;
			return dt.count() / 10'000;  	// Переведем в миллисекунды
		}
		return 0;
	}
	// Получить количество миллисекунд с момента запуска таймера - get the number of milliseconds since the timer was started
	unsigned long GetMs() 
	{
		return GetMs(GetTicks());
	}

	static unsigned long GetMs(tick_t dt) 
	{
		return (unsigned long)( ( dt + (500 / CLOCKS_PER_SEC) ) * (1000 / CLOCKS_PER_SEC) );
	}
	
    //  Остановка работающего секундомера, установка/возврат времени этапа - stop a running stopwatch, set/return lap time
    tick_t 		Stop(char const* event_name = "stop");

private:    //  members
    char const*     	m_activity; 		// Строка названия "деятельности" -   "activity" string
    tick_t          	m_lap;				// Время этапа lap time (Время последнего останова - time of last stop or 0)
    std::ostream&   	m_log;				// Поток д.11Я записи журнала   -   stream on which to log events
	//tick_t 				m_start;            // ??? - такая же переменная-член определена и инициализирована в базовом классе
};
//  ************************************************************************************************************
//  Определение встроенных функций-членов, включая конструкторы и деструктор
// 
// КОНСТРУКТОРЫ
// 
// Выполняет запуск таймера функцией-членом Start(), если start_now == true  -   performs a Start() if start_now == true
template <typename T> 
inline basic_stopwatch<T>::basic_stopwatch(bool start_now) : 
                                                m_activity("Таймер -Stopwatch-"), 
                                                m_lap(0), 
                                                m_log(std::cout)
{
    if (start_now)
        Start();                                // Функция-член базового класса                          
}

// Выполняет запуск таймера функцией-членом Start(), если start_now == true  -   performs a start if start_now == true, 
// подавить печать, если создание секундомера в форме ctor("") -  suppress print by ctor("")
template <typename T> 
inline basic_stopwatch<T>::basic_stopwatch(char const* activity, bool start_now) : 
												m_activity(activity && activity[0] ? activity : nullptr), 
												m_lap(0), 
												m_log(std::cout)
{
    if (start_now) 
	{
        if (m_activity)
            Start();                            // Функция-член базового класса            
        else
            Start(nullptr);                     // Функция-член производного класса
    }
}

//	set log output, optional printout, optional start
template <typename T> 
inline basic_stopwatch<T>::basic_stopwatch(std::ostream& log, char const* activity, bool start_now) : 
												m_activity(activity && activity[0] ? activity : nullptr), 
												m_lap(0), 
												m_log(log)
{
    if (start_now) 
	{
        if (m_activity)
            Start();                            // Функция-член базового класса                            
        else
            Start(nullptr);                     // Функция-член производного класса
    }
}
//  ДЕСТРУКТОР
//	stop/destroy stopwatch, print message if activity was set in ctor
template <typename T> 
inline basic_stopwatch<T>::~basic_stopwatch() 
{
    if ( IsStarted() ) 
	{
        if (m_activity)
            Stop();
        else
            Stop(nullptr);
    }
}

//   predicate: return true if the stopwatch is running
template <typename T> 
inline bool basic_stopwatch<T>::IsStarted() const
{
    return BaseTimer::IsStarted();
}

//	get the last lap time (time of last stop)
template <typename T> 
inline typename basic_stopwatch<T>::tick_t 		basic_stopwatch<T>::LapGet() const
{
    return m_lap;
}

//   show accumulated time, keep running, get/return lap time
template <typename T> 
inline typename basic_stopwatch<T>::tick_t 		basic_stopwatch<T>::Show(char const* event_name) 
{
    if (IsStarted()) 
	{
        m_lap = GetTicks();
        if (event_name && event_name[0]) 
		{
            if (m_activity)
                m_log << m_activity << ": ";
			
            m_log << event_name << " at " << GetMs(m_lap) << " mS" << std::endl << std::flush;
        }
    }
    else 
	{
        if (m_activity)
            m_log << m_activity << ": not started" << std::endl << std::flush;
    }
    return m_lap;
}

//   (re)start a stopwatch, set/return lap time
template <typename T> 
inline typename basic_stopwatch<T>::tick_t 		basic_stopwatch<T>::Start(char const* event_name) 
{
    if ( IsStarted() ) 
	{
        Stop(event_name);
    }
    else 
	{
        if (event_name && event_name[0]) 
		{
            if (m_activity)
                m_log << m_activity << "--: ";
			
            m_log << event_name << std::endl << std::flush;
        }
    }
    BaseTimer::Start();                         // Функция-член базового класса
    return 	    m_lap;
}

// Остановить работающий секундомер и распечатать накопленное время - stop a running stopwatch and print the accumulated time
template <typename T> 
inline typename basic_stopwatch<T>::tick_t 		basic_stopwatch<T>::Stop(char const* event_name) 
{
    if ( IsStarted() ) 
	{
        m_lap = GetTicks();
        if (event_name && event_name[0]) 
		{
            if (m_activity)
                m_log << m_activity << "++: ";
            m_log << event_name << " " << GetMs(m_lap) << " mS" << std::endl << std::flush;
        }
    }
    BaseTimer::Clear();                         // Функция-член базового класса
    return 	    m_lap;
}
/*
    Этот код просто воспроизводит определение класса. 
    
    Функции-члены для максимальной производительности являются встраиваемыми.

    Класс, который является значением параметра шаблона 'T', 
	представляет собой еще более простой вид таймера, 
	который зависит от операционной системы и используемого стандарта С++, 
	для доступа к счетчику тактов. 
	
    В книге приводится несколько версий класса 'TimerBase', 
	чтобы проверить различные реализации счетчика тактов. 
    
    В зависимости от того, насколько современный компилятор С++ используется, 
	класс T может использовать библиотеку С++ <chrono> 
	или получать такты непосредственно от операционной системы. 
	
    В заголовочном файле "TimerBase.h" показан класс TimerBase, 
	который использует библиотеку <chrono>, 
	доступную в С++ 11 и более поздних стандартах.
*/
using Stopwatch = basic_stopwatch<TimerBase>;

# endif