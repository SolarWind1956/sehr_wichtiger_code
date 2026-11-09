#ifndef TimerBase_H
#define TimerBase_H

#include <chrono>
#include <time.h>
using namespace std::chrono;

/*
	TimerBase: timer base class for C++03 and eariler
	Start() sets a start time
	GetMs() returns the delta time in integer milliseconds
*/

// !!! ********************** Мирим ляпы std: отключим на время этот макрос:
#undef min		// min(a, b) ((a) < (b) ? (a) : (b))

class TimerBase 
{
public:
	//	clears the timer - Сброс таймера
	TimerBase() : m_start(system_clock::time_point::min()) 
	{}

	//  clears the timer - Сброс таймера
	void Clear() { 
		m_start = system_clock::time_point::min(); 
	}

	//	returns true if the timer is running - Возвращает true, если таймер работает
	bool IsStarted() const 
	{
		return (m_start != system_clock::time_point::min());
		//return (m_start.time_since_epoch() != system_clock::duration(0));
	}

	//	start the timer
	void Start()            
	{ 
		//std::cout << "\nI'm void Start() of the class TimerBase " << std::endl;
		m_start = std::chrono::system_clock::now(); 
	}

	// get the number of milliseconds since the timer was started
	// Получение количества миллисекунд с момента запуска таймера
	unsigned long GetMs() 
	{
		if (IsStarted()) {
			system_clock::duration	diff = system_clock::now() - m_start;
			return (unsigned)(duration_cast<milliseconds>(diff).count());
		}
		return 0;
	}
//private:
	// Представляет точку во времени относительно эпохи часов.
	std::chrono::system_clock::time_point 		m_start; 
}; 

// !!! ********************** Мирим ляпы std - снова подключим отключенный на время макрос
#define min(a, b) ((a) < (b) ? (a) : (b))

# endif