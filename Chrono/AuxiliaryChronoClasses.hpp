#pragma once
/*
	AuxiliaryChronoClasses.hpp
*/
#include <chrono>
#include <time.h>
#include <stdio.h>
#include <ctime>
#include <cmath>

#include "AuxiliaryClasses.hpp"
/*
	Structure holding a calendar date and time broken down into its components.
	
 	struct tm
	{
		//	Member objects
		int 	tm_sec			//	seconds after the minute 		– [0, 60]
		int 	tm_min			//	minutes after the hour 			– [0, 59]
		int 	tm_hour			//	hours since midnight 			– [0, 23]
		int 	tm_mday			//	day of the month 				– [1, 31]
		int 	tm_mon			//	months since January 			– [0, 11]
		int 	tm_year			//	years since 1900
		int 	tm_wday			//	days since Sunday 				– [0, 6]
		int 	tm_yday			//	days since January 1 			– [0, 365]
		int 	tm_isdst		//	Daylight Saving Time flag. 
								//	The value is positive if DST is in effect, 
								//	zero if not and negative if no information is available
*/
class	CurrentTime
{
public:	
	CurrentTime(){
		//cout << "-------------- I'm " << __func__ << endl;
		make_current_tm_structure();
	}
	void make_current_tm_structure()
	{
		std::chrono::time_point<std::chrono::system_clock>		sysTimePoint;
		sysTimePoint						=	std::chrono::system_clock::now();
		std::time_t		current_time_point 	= 	std::chrono::system_clock::to_time_t(sysTimePoint);
			
		__time64_t		time_tp = static_cast<__time64_t>(current_time_point);
		
		_time64(&time_tp);							// Get time in seconds.
		_localtime64_s(&t_m, &time_tp);	// Convert time to struct tm form.
		
		// 
		t_m.tm_mon	+=	1;
		t_m.tm_year	+=	1900;
	}
	
	struct tm	t_m;
};

void test_tm_form(struct tm&	curr_time){
	cout << "-------------- I'm " << __func__ << endl;
	
	cout << "curr_time.tm_sec     == " << curr_time.tm_sec << endl;
	cout << "curr_time.tm_min     == " << curr_time.tm_min << endl;
	cout << "curr_time.tm_hour    == " << curr_time.tm_hour << endl;
	cout << "curr_time.tm_mday    == " << curr_time.tm_mday << endl;
	cout << "curr_time.tm_mon     == " << curr_time.tm_mon << endl;
	cout << "curr_time.tm_year    == " << curr_time.tm_year << endl;
	cout << "curr_time.tm_wday    == " << curr_time.tm_wday << endl;
	cout << "curr_time.tm_yday    == " << curr_time.tm_yday << endl;
	cout << "curr_time.tm_isdst   == " << curr_time.tm_isdst << endl;
}

//	
