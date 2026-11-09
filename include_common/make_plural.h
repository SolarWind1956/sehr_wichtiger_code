/*
 
*/ 

#include <cstddef>
using std::size_t;

#include <string>
using std::string;

#include <iostream>
using std::cout; using std::endl;

#ifndef MAKE_PLURAL_H
#define MAKE_PLURAL_H

// return the plural version of word if ctr is greater than 1
inline
string make_plural(size_t ctr, const string &word, 
                               const string &ending)
{
	return (ctr > 1) ? word + ending : word;
}

// Лямбда формирует правильное слово 
auto time_or_times	= [](int int_val) -> string {
	set<int>	time_of_int{2, 3, 4};
	return (time_of_int.contains(int_val % 10)) ? " раза" : " раз";			
};
/*
	Проги для формирования правильных окончаний слов русского языка
	для всех родов и падежей
*/
#endif
