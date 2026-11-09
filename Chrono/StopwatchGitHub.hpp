// StopwatchGitHub.hpp
//Copyright Ingo Proff 2017.
// https://github.com/CrikeeIP/Stopwatch
// Distributed under the MIT Software License (X11 license).
// (See accompanying file LICENSE)
// https://github.com/ManWithABike/Stopwatch/blob/master/include/stopwatch/Stopwatch.hpp

#pragma once

#include <vector>
#include <string>
#include <chrono>

namespace stopwatch
{
class Stopwatch{
public:
	enum TimeFormat{ NANOSECONDS, MICROSECONDS, MILLISECONDS, SECONDS };
	//	************* Конструкторы	и деструктор	******************************************************************
	Stopwatch(): start_time_point(), laps_time_pnts({}) 
	{
		start();
	}
	// 	Методы 
	//	--------------------------------------------------------------------------------------------------------------
	// 	Инициализация объекта моментом запуска секундомера
	void start(){
		start_time_point 	= std::chrono::high_resolution_clock::now();
		// Инициализация вектора моментов времени кругов секундомера моментом времени запуска секундомера
		laps_time_pnts 		= {start_time_point};
		// Инициализация вектора  моментов времени дистанций секундомера моментом времени запуска секундомера
		distances_time_pnts	= {start_time_point};
	}
	//	--------------------------------------------------------------------------------------------------------------
	// Добавляет текущий (натоящий) момент времени в вектор кругов секундомера
	// и возвращает счетчик тактов предыдущего круга по отношению к настоящему моменту времени
	template	<	TimeFormat time_fmt = TimeFormat::MILLISECONDS
				>
	std::uint64_t 	lap_start()
	{
		// Запоминаем момент времени последнего круга в векторе моментов времени кругов секундомера
		const auto last_lap_time_pnt 	= laps_time_pnts.back();
		// Добавляем текущий (настоящий) момент времени очередного круга
		const auto now_time_pnt 		= std::chrono::high_resolution_clock::now();
		laps_time_pnts.push_back(now_time_pnt);
		
		return 	ticks<time_fmt>(last_lap_time_pnt, now_time_pnt);
	}
	//	--------------------------------------------------------------------------------------------------------------
	// Добавляет текущий (натоящий) момент времени в вектор моментов времени дистанций секундомера
	// и возвращает счетчик тактов предыдущей дистанции по отношению к настоящему моменту времени
	template	<	TimeFormat time_fmt = TimeFormat::MILLISECONDS
				>
	std::uint64_t 	distance_end()
	{
		// Запоминаем момент времени последней дистанции в векторе моментов времени кругов секундомера
		const auto last_distance_time_pnt 	= distances_time_pnts.back();
		
		// Добавляем текущий (настоящий) момент времени очередной дистанции
		const auto now_time_pnt 		= std::chrono::high_resolution_clock::now();
		distances_time_pnts.push_back(now_time_pnt);
		
		return 	ticks<time_fmt>(last_distance_time_pnt, now_time_pnt);
	}
	//	--------------------------------------------------------------------------------------------------------------
	// Метод формирования счетчика тактов для секундомера независимо от того, 
	// целое количество кругов в него уложилось, или нет
	template	<	TimeFormat time_fmt = TimeFormat::MILLISECONDS
				>
	std::uint64_t 	elapsed_stopwatch_time_ticks()
	{
	    const auto end_time_point 	= std::chrono::high_resolution_clock::now();
		return ticks<time_fmt>(start_time_point, end_time_point);
	}
	//	--------------------------------------------------------------------------------------------------------------
	// Метод формирования счетчика тактов для кругов секундомера в отдельности и всех вместе
	template	<	TimeFormat time_fmt_total 			= TimeFormat::MILLISECONDS
				, 	TimeFormat time_fmt_lap_time_pnt 	= time_fmt_total
				>
	std::pair<std::uint64_t, std::vector<std::uint64_t>> 	laps_elapsed_time_ticks()
	{
		// Вспомогательный вектор счетчиков тактов кругов секундомера
		std::vector<std::uint64_t> 	laps_time_ticks;
		// В соответствии с текущим размером вектора моментов времени кругов
		laps_time_ticks.reserve(laps_time_pnts.size()-1);
		// Цикл заполнения вспомогательного вектора счетчиками тактов кругов секундомера
		for(std::size_t idx = 0; idx <= laps_time_pnts.size()-2; idx++){
			const auto lap_end_time_pnt 		= laps_time_pnts[idx+1];
			const auto lap_start_time_pnt 		= laps_time_pnts[idx];
	
			laps_time_ticks.push_back(ticks<time_fmt_lap_time_pnt>(lap_start_time_pnt, 	lap_end_time_pnt));
		}

		return  {	ticks<time_fmt_total>	(	start_time_point
											, 	laps_time_pnts.back()
											)
											, 	laps_time_ticks
				};
	}
	//	--------------------------------------------------------------------------------------------------------------
	// Метод формирования счетчика тактов для дистанций секундомера в отдельности и всех вместе
	template	<	TimeFormat time_fmt_total 			= TimeFormat::MILLISECONDS
				, 	TimeFormat time_fmt_lap_time_pnt 	= time_fmt_total
				>
	std::pair<std::uint64_t, std::vector<std::uint64_t>> 	distances_elapsed_time_ticks()
	{
		// Вспомогательный вектор счетчиков тактов дистанций секундомера
		std::vector<std::uint64_t> 	distances_time_ticks;
		// В соответствии с текущим размером вектора моментов времени дистанций
		distances_time_ticks.reserve(distances_time_pnts.size()-1);
		// Цикл заполнения вспомогательного вектора счетчиками тактов дистанций секундомера
		for(std::size_t idx = 0; idx <= distances_time_pnts.size()-1; idx++){
			distances_time_ticks.push_back(ticks<time_fmt_lap_time_pnt>(start_time_point, 	distances_time_pnts[idx]));
		}

		return  {	ticks<time_fmt_total>	(	start_time_point
											, 	distances_time_pnts.back()
											)
											, 	distances_time_ticks
				};
	}
	//	--------------------------------------------------------------------------------------------------------------
private:
	typedef 	std::chrono::time_point<std::chrono::high_resolution_clock> 	time_pnt;
	
	time_pnt 				start_time_point;		// Момент времени для часов с высоким разрешением
	std::vector<time_pnt> 	laps_time_pnts;			// Вектор моментов времени для начал кругов секундомера
	std::vector<time_pnt> 	distances_time_pnts;	// Вектор моментов времени для завершений дистанций секундомера
	//	--------------------------------------------------------------------------------------------------------------
	// 	Возвращает счетчик тактов в заданных периодах между двумя моментами времени
	template<TimeFormat 	time_fmt = TimeFormat::MILLISECONDS>
	static std::uint64_t 	ticks(const time_pnt& 	start_time_point, const time_pnt& 	end_time_point)
	{
		// Разность моментов времени - суть длительность в терминах библионтеки std::chrono
		const auto 				duration = end_time_point - start_time_point; 		// В миллисекундах
		// Теперь приведем длительность в наносекунды и сосчитаем такты, очевидно в предположении
		// один такт равен одной наносекунде для современного процессора настольного компьютера
		const std::uint64_t 	ns_count = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();

		switch(time_fmt){
			case TimeFormat::NANOSECONDS:
			{
				return ns_count;
			}
			case TimeFormat::MICROSECONDS:
			{
				std::uint64_t 	up 			= ((ns_count / 100)%10 >= 5) ? 1 : 0;			// Разряд округления
				const auto 		mus_count 	= ( ns_count / 1'000) + up;
				return 			mus_count;
			}
			case TimeFormat::MILLISECONDS:
			{
				std::uint64_t 	up 			= ((ns_count / 100'000)%10 >= 5) ? 1 : 0;		// Разряд округления
				const auto 		ms_count 	= ( ns_count / 1'000'000) + up;
				return 			ms_count;
			}
			case TimeFormat::SECONDS:
			{
				std::uint64_t 	up 			= ((ns_count / 100'000'000)%10 >= 5) ? 1 : 0;	// Разряд округления
				const auto 		s_count 	= ( ns_count / 1'000'000'000) + up;
				return s_count;
			}
		}	// End of switch
    }
};


constexpr Stopwatch::TimeFormat ns 				= Stopwatch::TimeFormat::NANOSECONDS;
constexpr Stopwatch::TimeFormat mus 			= Stopwatch::TimeFormat::MICROSECONDS;
constexpr Stopwatch::TimeFormat ms 				= Stopwatch::TimeFormat::MILLISECONDS;
constexpr Stopwatch::TimeFormat s 				= Stopwatch::TimeFormat::SECONDS;

constexpr Stopwatch::TimeFormat nanoseconds 	= Stopwatch::TimeFormat::NANOSECONDS;
constexpr Stopwatch::TimeFormat microseconds 	= Stopwatch::TimeFormat::MICROSECONDS;
constexpr Stopwatch::TimeFormat milliseconds 	= Stopwatch::TimeFormat::MILLISECONDS;
constexpr Stopwatch::TimeFormat seconds 		= Stopwatch::TimeFormat::SECONDS;


std::string 	show_times(const std::vector<std::uint64_t>& time_ticks)
{
	struct space_out : std::numpunct<char> {
		char			do_thousands_sep()		const { return 0x27; }  // separate with apostrophe
		std::string		do_grouping()			const { return "\3"; }	// groups of 1 digit
	};
	cout.imbue(std::locale(std::cout.getloc(), new space_out));
	
    std::string 	result("{");
    for(const auto& t : time_ticks){
        result += std::to_string(t) + ", ";
    }
    result.back() = static_cast<char>('}');
    return result;
}


}	// namespace stopwatch
