//	Heckel_histogramm_builder.cpp
//
//  	Sergej Edgar Heckel 
//  	17.02.2026
//
// 	Demonstration program: build and print a histogram for a normal distribution
// 	Cleaned & commented version (<=100 lines)
//	
//	author: Sergej Edgar Heckel
//	          with insightful support from Gemini (AI)
//  	                "Дорогу осилит идущий, а вдвоем идти спорее"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
using std::cout; using std::endl;
#include <iterator>
#include <random>
#include <vector>
//	------------------------  «Nice Numbers for Graph Labels» - Paul Heckbert algorithm, Graphics Gems (1990) 
// 	Normalization behaviour was discussed and mathematically verified during technical review.
double 	nice_step(double diff)											//	narrow contract: diff > 0.0
{																		//	diff = max_smpl - min_smpl
    double 	exp 		= 	std::floor(std::log10(diff));	
	/*
		lim(t→0+) floor(log(10^n+t) =  n
		lim(t→0-) floor(log(10^n-t) =  n-1
	*/
    double 	base 	= 	diff / std::pow(10.0, exp);						// 	Normalized number from 1 to 10
	/*
		lim(t→0+) (10^n / 10^(floor(log(10^n+t)) = 10^n / 10^n = 1;		
		lim(t→0-) (10^n / 10^(floor(log(10^n-t)) = 10^n / 10^(n-1) = 10;
	*/
    double 	nice 	= 	(base < 1.5) ? 1 :
						(base < 3)   ? 2 :
						(base < 7)   ? 5 : 10;

    return 	nice * std::pow(10.0, exp);
}
//	-----------------------------------
void 	print_histogramm	(	const std::vector<int>& 	histogram
						,	double 					min_smpl				//	For histogram axis mark
						,	int						bins
						,	double 					step
						)	{
	//	What counter is max
    int 		max_count = *std::max_element(histogram.begin(), histogram.end());
    	
	int		max_bar_height	= 100;										//	Max chart bar height  (or width)
	double 	scale 			= double(max_bar_height) / max_count;			//	Scale to fit chart bars all on the screen

	for (int bin = 0; bin < bins; ++bin) 
	{
		double value = min_smpl + bin * step;								//	Value as histogram axis mark
		cout << std::setw(10) << std::setprecision(5) << value << "\t| ";
		
		std::fill_n	(	std::ostream_iterator<char>(std::cout)			//	Viewing chart bars
					,	int(histogram[bin] * scale)
					, 	'*'
					);
		cout << ' ' << histogram[bin] << '\n';							//	Chart bar numeric marker
	}	
}
//	----------------------------------- 	Build and print histogram for numeric forward range 
template<class It>
	requires std::random_access_iterator<It>								//	std::vector, std::deque, std::array - ✅, 
																		//	but not std::list, std::map, std::set -❌
void histogram(It first, 	It last)
{
	auto 	[min_it, max_it] 	= std::minmax_element(first, last);		//	Max and min samples ​​in signal
    double 			min_smpl		= *min_it;
	double 			max_smpl		= *max_it;

    const auto 		samples_n 	= std::distance(first, last);				//	Samples number in random signal
	 
	int 				bins 		= int(1 + 3.322 * std::log10(samples_n));	//	Number of bins (Sturges rule)
	if (max_smpl == min_smpl) {
		cout << "All samples are equal: ";
		cout << std::setw(10) << max_smpl << "\t| " << std::string(50, '*') << " " << samples_n << '\n';
		return;
	}
    double 			step 		= nice_step((max_smpl - min_smpl) / bins);//	To Paul Heckbert algorithm
		
    std::vector<int> histogram(bins, 0);									//	Histogram vector for display

	for (auto it = first; it != last; ++it) 								//	Distribution elem counters into histogram
	{
        int 	idx 	= 	int((*it - min_smpl) / step);						//	Conditions for the distribution element 
		idx 		= 	std::clamp(idx, 0, bins - 1);						//	to fall into particular bin   
		++histogram[idx];												//	Count numbers of distribution in each bin
    }
	
	print_histogramm	(histogram,	min_smpl	,	bins	,	step	);
}
void histogramm_test_1()
{
	constexpr int 					samples_n 		= 10'000;			//	Number of samples in random signal
    std::vector<double> 				signal(samples_n);			
	
	std::mt19937 					rng(std::random_device{}());			//	Mersenne-Twister would be better
	
	auto 							Mu{1.0};							//	Mean value
	auto 							Sigma{3.0};							//	Standard deviation
    std::normal_distribution<double> 	distr(Mu, Sigma);

	for (auto& v : signal) 												//	Generate random signal
		v = distr(rng);

    histogram(signal.begin(), signal.end());								//	Build and print histogramm
}
// Псевдокод для вашего эксперимента
void Simulation_Experiment() {
    int 		total_trials 		= 1000;    // 1000 подбрасываний в одной серии
    int 		num_simulations 		= 10000; // 10 000 повторений всего эксперимента
    double 	p 					= 0.5;

    // Вектор или массив для сбора статистики (например, счетчик выпавших гербов)
    // Размер 1001, чтобы поместить результаты от 0 до 1000
    std::vector<int> results(1001, 0);

	std::mt19937 					rng(std::random_device{}());			//	Mersenne-Twister would be better
	
	std::uniform_real_distribution<double> 	coin_flip(0.0, 1.0);
	
	std::fill(results.begin(), results.end(), 0);
	
    for (int i = 0; i < num_simulations; ++i) {
        int heads = 0;
        for (int j = 0; j < total_trials; ++j) {
            // Генератор случайных чисел: если выпало меньше p, считаем гербом
            if (coin_flip(rng) < p) {
                heads++;
            }
        }
        results[heads]++;
    }

	// Поиск реального центра и разброса
	int max_val = 0;
	int peak_index = 0;
	for(int i = 0; i < results.size(); ++i) {
		if(results[i] > max_val) {
			max_val = results[i];
			peak_index = i;
		}
	}
	cout << ">>> Пик распределения (реальный центр) сейчас на: " << peak_index << endl;
	
	// 1. Проверим, сколько всего экспериментов у нас записано
	int total_sum = 0;
	for(int count : results) total_sum += count;
	cout << "Всего учтено экспериментов: " << total_sum << endl;

	// 2. Найдем, где реально лежат данные
	int min_idx = -1, max_idx = -1;
	for(int i = 0; i < results.size(); ++i) {
		if(results[i] > 0) {
			if(min_idx == -1) min_idx = i;
			max_idx = i;
		}
	}
	cout << "Данные найдены в диапазоне от " << min_idx << " до " << max_idx << endl;
	
	// 1. Укажите диапазон, который хотите видеть (например, от 400 до 600)
    int start_bin = 400;
    int end_bin = 600;
    int num_bins = end_bin - start_bin;

    // 2. Вызываем функцию печати напрямую
    // Передаем срез вектора, начиная с 400
    // min_smpl = 400, step = 1.0 (так как каждый индекс - это 1 герб)
    print_histogramm(std::vector<int>(results.begin() + start_bin, results.begin() + end_bin), 
                     400.0, num_bins, 1.0);
}
void Simulation_Experiment_2() 
{
    int 		total_trials 	= 1'000;		//	Общее количество испытаний в эксперименте
    int 		num_simulations 	= 10'000;		//	Количество повторений всего эксперимента - моделирование
    double 	p 				= 0.5;			//	Вероятность выпадания "орла" (или "решки") при бросании монеты

    // ВМЕСТО вектора счетчиков (results), создаем вектор ВСЕХ результатов
    std::vector<double> 		all_experiments;
    all_experiments.reserve(num_simulations); 

    std::mt19937 							rng(std::random_device{}());
	//	Используем равномерное распределение вероятностей резултатов испытаний
    std::uniform_real_distribution<double> 	coin_flip(0.0, 1.0);
	  
	//	Цикл формирования гистограммы результатов испытаний - в каждом элементе вектора
	//	случайное количество выпаданий "орла" в одном эксперименте бросания монеты `total_trials` раз
	//	при нормальном (равномерном, равновероятном) распределении
    for (int i = 0; i < num_simulations; ++i) {
        int heads = 0;
        for (int j = 0; j < total_trials; ++j) {
            if (coin_flip(rng) < p) heads++;			//	Сколько было "орлов" (или "решек") в эксперименте
        }
        // Записываем результат каждого эксперимента в общий массив
        all_experiments.push_back(static_cast<double>(heads));
    }

    // И ТЕПЕРЬ — БИНГО! 
    // Передаем весь массив в нашу умную функцию. 
    // Она сама применит правило Стерджеса, сама вызовет Хекберта
    // и сама нарисует идеальную гистограмму!
    histogram(all_experiments.begin(), all_experiments.end());
}
//  	---------------------------------------------		main()
int main()
{
	//histogramm_test_1();
	//Simulation_Experiment();
	Simulation_Experiment_2();
}
