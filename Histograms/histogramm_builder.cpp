//	histogramm_builder.cpp
//
//  	Sergii Edgar Hekkel (Heckel)
//  	14.02.2026
//
#include <iostream>
using std::cout; using std::endl; using std::boolalpha;
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <iterator>
#include <limits>
#include <map>
#include <random>
#include <string>
#include <vector>
/*
	This program was developed while studying existing random number distributions.

	Only the Normal (or Gaussian) random number distribution. 

	The program generates an array of random numbers,
	and their normal distribution is visualized
	on the alphanumeric console.
*/

//	Histogramm builder
template<typename FwdIter>
	requires std::forward_iterator<FwdIter>
void 	histogram	(	FwdIter 			begin_of_signal
					, 	FwdIter 			end_of_signal
					, 	int 				rows 		= 	25
					, 	int 				cols 		= 	160
					) {
    cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	
	//	The numbers we got to build histogram
	std::copy	(begin_of_signal, begin_of_signal + 20, std::ostream_iterator<double>(std::cout, " "));
	cout << endl;			
	
	//	Maximum and minimum values ​​of the samples in histogram signals container
	auto 	minmax 	= 	std::minmax_element(begin_of_signal, 	end_of_signal);
	cout << "min == " <<  *minmax.first <<  "  max == " <<  *minmax.second << endl;
	
	//	Histogramm's parameters
	const int			histogramm_binns{rows};	// 	binns (buckets) number
	std::map<int, int> 	histogramm_map;			//	key - binn's index, value - samples falling into the bucket
	
	
	
	//	Optimal number of segments (intervals) - Sturges method
	auto SturgesBinns	=	(int)(1.0 + 3.322 * std::log(std::distance(begin_of_signal, end_of_signal)));

	//	Optimal width of a samples' group 	-	// http://ru.stackoverflow.com/a/487842/177684
	auto bucket_width = std::numeric_limits<double>::epsilon();
	for 	(	auto n = -30
		; 	bucket_width < (*minmax.second - *minmax.first) / SturgesBinns; ++n)
	        bucket_width = "\x1\x2\x5"[(30 + n) % 3] * pow(10, (30 + n) / 3 - 10
		);
		
	//	Histogram' associative array
    std::for_each	(	begin_of_signal
					, 	end_of_signal
					, 	[&](auto sample) { ++histogramm_map[int(sample / bucket_width)];	}
					);
	
	for (const auto& [key, value] : histogramm_map)
        cout << '[' << key << "] = " << value << "; ";
	cout << endl;
	
	//	Maximum  value of the frequency of occurrence of samples 
	int 		max_n 	= 	std::max_element	(	std::begin(histogramm_map)
										, 	std::end(histogramm_map)
										, 	[](auto& a, auto& b) { return a.second < b.second; }
										)	->second;
	cout << "Maximum value of histogram element - " << max_n << endl;								
	
	//	Scaling factor for adjusting the sizes of the chart bars to fit them all on the monitor screen
	double 	n_scale 	= 	std::min	(1.0, 	double(cols - 21) / max_n);
	
    // 	Print the histogramm
    for (int i = histogramm_map.begin()->first; i <= histogramm_map.rbegin()->first; ++i) 
	{
        cout << std::setw(4) << i * bucket_width << " |";
		
        std::fill_n	(	std::ostream_iterator<char>(std::cout)
					, 	int(histogramm_map[i] * n_scale)
					, 	'*'
					);
					
        cout << ' ' << histogramm_map[i] << '\n'; 	//	Numeric marker
    }	
}			
/*
	----------------------------------------------------------------------------------------------------------------------------------------------------
	|	main()
	----------------------------------------------------------------------------------------------------------------------------------------------------
*/	
int main()
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << "of histogramm_builder" << endl;
	
	const int							samples_number			{10'000};	
	double								signal[samples_number]	{0.0};		
		
    std::random_device 					rd; 								// 	Receiving unpredictable seed
	std::default_random_engine			deflt_rand_eng			{rd()};		// 	Engine' initializing with a seed
	
	auto 								Mu						{1.0};		//	Mean value
	auto 								Sigma					{3.0};		//	Standard deviation
	std::normal_distribution<double> 		norm_distr				{Mu, Sigma};	
	
	//	Generate signal
	for (int i{};	i < samples_number; ++i) {
		signal[i]	= norm_distr(deflt_rand_eng);
	}
	//	Call histogramm_builder
	histogram	(std::begin(signal),	std::end(signal));
}


