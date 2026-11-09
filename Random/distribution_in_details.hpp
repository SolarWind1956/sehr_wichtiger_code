//	distribution_in_details.hpp
#ifndef INCLUDE_GUARDS_distribution_in_details_HPP
#define INCLUDE_GUARDS_distribution_in_details_HPP

/* 
 * The following code example is taken from the book
 * "The C++ Standard Library - A Tutorial and Reference, 2nd Edition"
 * by Nicolai M. Josuttis, Addison-Wesley, 2012
 *
 * (C) Copyright Nicolai M. Josuttis 2012.
 * Permission to copy, use, modify, sell and distribute this software
 * is granted provided this copyright notice appears in all copies.
 * This software is provided "as is" without express or implied
 * warranty, and with no claim as to its suitability for any purpose.
 */
#include <random>
#include <map>
#include <string>
#include <iostream>
using std::cout; using std::endl; using std::boolalpha;
#include <iomanip>

template 	<	typename 	Distribution
			, 	typename 	Engine
			>
void 	distribution_in_details	(	Distribution 		distribution
								, 	Engine 				engine
								, 	const std::string& 	name
								)
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	
    // 	print min, max and four example values
    cout << name << ":" << endl;
    cout << "- min():  " << distribution.min() 	<< endl; 
    cout << "- max():  " << distribution.max() 	<< endl; 
    cout << "- values: " << distribution(engine) 	<< ' ' 		//	Каждый раз новое случайное число
						<< distribution(engine) 	<< ' '
						<< distribution(engine) 	<< ' ' 
						<< distribution(engine) 	<< endl; 

    // 	count the generated values (converted to integral values)
    std::map<long long, int> 		value_counter;
    for (int i = 0; i < 200000; ++i) {
        value_counter[distribution(engine)]++;
    }

    // 	and print the resulting distribution
    cout << "====" << endl;
    for (auto elem : value_counter) {
        cout << std::setw(3) 	<< elem.first 	<< ": "
                                << elem.second 	<< endl;
    }
    cout << "====" << endl;
    cout << endl;
}
//	-----------------------------------------------------------------------------------------------------------------------------------------------------------------
#endif	//	#ifndef INCLUDE_GUARDS_distribution_in_details_HPP
