//StopwatchGitHub_test.cpp
// Copyright Ingo Proff 2017.
// https://github.com/CrikeeIP/Stopwatch
// Distributed under the MIT Software License (X11 license).
// (See accompanying file LICENSE)
// https://github.com/ManWithABike/Stopwatch/blob/master/include/stopwatch/Stopwatch.hpp
#include <locale>
#include <iostream>
using std::cout; using std::endl;
#include <thread>
#include <chrono>
using namespace std::chrono_literals;

#include "StopwatchGitHub.hpp"
#include "StopwatchSEH.hpp"

void stopwatch_test()
{
	cout << "void stopwatch_test()" << endl;
	{
		//Namespace alias
		namespace sw = stopwatch;

		//Create and start a stopwatch
		sw::Stopwatch 		my_watch;
		my_watch.start();

		//Do something time-consuming
		for(std::size_t i = 1; i <= 2'000'000'000; i++){
			if( i%400'000'000 == 0){
				cout << i << endl;
			}
			if( i % 200'000'000 == 0){
				my_watch.lap_start();
			}
			
			if( i % 200'000'000 == 0){
				my_watch.distance_end();
			}
		}

		//	Get elapsed time..
		// .. in nanoseconds
		uint64_t 	elapsed_ns 			= my_watch.elapsed_stopwatch_time_ticks<sw::nanoseconds>();
		// .. in microseconds
		uint64_t 	elapsed_mus 		= my_watch.elapsed_stopwatch_time_ticks<sw::microseconds>();
		// .. in milliseconds (default template argument, therefore not needed)
		uint64_t 	elapsed_ms 			= my_watch.elapsed_stopwatch_time_ticks();
		// .. in seconds
		uint64_t 	elapsed_s 			= my_watch.elapsed_stopwatch_time_ticks<sw::seconds>();

		//Print to screen
		cout << elapsed_ns 	<< " ns" 	<< endl;
		cout << elapsed_mus << " mus" 	<< endl;
		cout << elapsed_ms 	<< " ms" 	<< endl;
		cout << elapsed_s 	<< " s" 	<< endl;
		
		cout << "---------------" 		<< endl;
		//	Get lap times
		auto 		laps_time_ticks 	= my_watch.laps_elapsed_time_ticks();
		
		cout << "---------------" 		<< endl;
		cout << "Laps Total, ms: " << laps_time_ticks.first 	<< endl;
		
		auto laps_times = laps_time_ticks.second;
		cout << "Laps Times, ms: " ; sw::show_times(laps_times);
		cout << "\n---------------" 		<< endl;
		
		//	Get distance times
		auto 		distances_time_ticks 	= my_watch.distances_elapsed_time_ticks();
			
		cout << "---------------" 		<< endl;
		cout << "Distances Total, ms: " << distances_time_ticks.first 	<< endl;
		
		auto distances_times = distances_time_ticks.second;
		cout << "Distances Times, ms: " ; sw::show_times(distances_times);
		cout << "\n---------------" 	<< endl;
		
		cout << "1234567890  ------------  " << 1234567890 << endl;
	}
	cout << "1234567890  ------------  " << 1234567890 << endl;
}

void stopwatch_test_2()
{
	cout << "void stopwatch_test_2()" << endl;
	{
		//Namespace alias
		namespace sw = stopwatch;

		//Create and start a stopwatch
		sw::Stopwatch 		my_watch;
		my_watch.start();

		//Do something time-consuming
		for(std::size_t i = 1; i <= 2'000'000'000; i++)
		{}

		//	Get elapsed time..
		// .. in nanoseconds
		uint64_t 	elapsed_ns 			= my_watch.elapsed_stopwatch_time_ticks<sw::nanoseconds>();
		// .. in microseconds
		uint64_t 	elapsed_mus 		= my_watch.elapsed_stopwatch_time_ticks<sw::microseconds>();
		// .. in milliseconds (default template argument, therefore not needed)
		uint64_t 	elapsed_ms 			= my_watch.elapsed_stopwatch_time_ticks();
		// .. in seconds
		uint64_t 	elapsed_s 			= my_watch.elapsed_stopwatch_time_ticks<sw::seconds>();

		//Print to screen
		cout << elapsed_ns 	<< " ns" 	<< endl;
		cout << elapsed_mus << " mus" 	<< endl;
		cout << elapsed_ms 	<< " ms" 	<< endl;
		cout << elapsed_s 	<< " s" 	<< endl;
		
		cout << "---------------" 		<< endl;
		
	}
}
void stopwatchSEH_test()
{
	cout << "void stopwatchSEH_test()" << endl;
	
	//Namespace alias
	namespace sw = stopwatchSEH;

	//Create and start a stopwatch
	sw::StopwatchSEH 		my_watch;
	
	//	Секундомер общий
	my_watch.start();

	//	Секундомер кругов
	my_watch.lap_start();
	
	//	Секундомер дистанций
	my_watch.distance_start();
	
	for(std::size_t i = 1; i <= 4'000'000'000; i++){
		if( i % 800'000'000 == 0){
			my_watch.lap_start();
			my_watch.distance_start();
		}
	}
	
	my_watch.stop();	
	my_watch.distances_stop();
	std::this_thread::sleep_for(4'700ms); 	// Это чтобы получился еще один круг
	my_watch.laps_stop();
	
}

//---------------------------------------------------------
// main
//---------------------------------------------------------

int main()
{
	cout << "I'm acquiring_high_resolution_time_stamps.cpp" << endl;
	
	//stopwatch_test();
	//stopwatch_test_2();
	stopwatchSEH_test();
	
    return 0;
}
