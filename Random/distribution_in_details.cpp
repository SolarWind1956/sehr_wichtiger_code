//	distribution_in_details.cpp
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
#include <iomanip>

#include "distribution_in_details.hpp"

int main()
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << " of distribution_in_details.cpp" << endl;
	/*
		Какой-то конгруэнтный линейный генератор случайных чисел
	*/
    std::knuth_b 	knuth_b_engine;

    std::uniform_real_distribution<> 	uniform_distrib(0, 10);
    distribution_in_details	(	uniform_distrib
							,	knuth_b_engine
							,	"uniform_real_distribution"
							);

    std::normal_distribution<> 		normal_distrib;
    distribution_in_details	(	normal_distrib
							,	knuth_b_engine
							,	"normal_distribution"
							);

    std::exponential_distribution<> 	exponential_distrib;
    distribution_in_details	(	exponential_distrib
							,	knuth_b_engine
							,	"exponential_distribution"
							);

    std::gamma_distribution<> 		gamma_distrib;
    distribution_in_details	(	gamma_distrib
							,	knuth_b_engine
							,	"gamma_distribution"
							);
}



//	*******************************************************************************************************************
#if 0	

int main()
{
	const unsigned int Start{ 1 };
	const unsigned int End{ 99 };
	const unsigned int Iterations{ 1'000'000 };

	// Normal distributed Mersenne Twister.
	random_device seeder;
	const auto seed{ seeder.entropy() ? seeder() : time(nullptr) };
	mt19937 engine{ static_cast<mt19937::result_type>(seed) };
	normal_distribution<double> distribution{ 50, 10 };
	auto generator{ bind(distribution, engine) };
	map<int, int> histogram;
	for (unsigned int i{ 0 }; i < Iterations; ++i) {
		int randomNumber{ static_cast<int>(generator()) };
		// Search map for a key=randomNumber. If found, add 1 to the value associated
		// with that key. If not found, add the key to the map with value 1.
		++(histogram[randomNumber]);
	}

	// Write to a CSV file.
	ofstream of{ "res.csv" };
	for (unsigned int i{ Start }; i <= End; ++i) {
		of << i << ";" << histogram[i] << endl;
	}
}

#endif	