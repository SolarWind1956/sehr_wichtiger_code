//	histogramm_building.cpp
/*

*/
#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <random>
#include <string>
#include <vector>
#include "..\include_common\type_name_as_str.h"
#include <iostream>
using namespace std;
#include "histogramm_building.hpp"
#include "histogramm_building_cont_1.hpp"
#include "histogramm_building_cont_2.hpp"

static void any_histogramm_building_test()	{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	
	const int									samples				{10'000};	//	Количество отсчетов
	double										signal[samples]		{0.0};
	
	std::default_random_engine					d_r_e		{42};
	std::normal_distribution<double> 				norm_distr	{1.0, 3.0};
	
	for (int i{};	i < samples; ++i)
		signal[i]	= norm_distr(d_r_e);
	
	for (int i{0}; i < 20; ++i)
		cout << signal[i] << "  ";
	cout << endl;
	
	histogram	(	std::begin(signal)
				,	std::end(signal)
				);
}
//	--------------------------------------------------------------------------------------------------------------------------------------------------
static	void		remainder_from_division()	{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	cout << "Остаток от деления -7 на 2 == " << -7%2 << endl; 
}	
/*
	**************************************************************************************************************
*/	
int main()
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << "of histogramm_building" << endl;
	//	--------------------------------------------	histogramm_building.hpp 	--------------------------------------
	any_histogramm_building_test();
	//remainder_from_division();
	return 0;
}