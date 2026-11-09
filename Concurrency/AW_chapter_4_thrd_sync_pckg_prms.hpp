#ifndef GUARD_AW_chapter_4_thrd_sync_pckg_prmshpp
#define GUARD_AW_chapter_4_thrd_sync_pckg_prmshpp
/*
Энтони Уильямс. C++. Практика многопоточного программирования. 2-е издание. 2020 год.
Anthony Williams. C++ Concurrency in Action. Second Edition. 2020.

"Глава 4. Синхронизация конкурентных операций"
Chapter 4.
 */
 // Стандартная библиотека
#include "my_std_library.h"
#include "my_process_of_exeptions.h"
#include "Polygon.h"

namespace ns_AW_chapter_4
{
	/*
		*************************************************************************************************
		Глава 4. Синхронизация конкурентных операций
		*************************************************************************************************
	В большинстве случаев какому-нибудь потоку следует дожидаться наступления конкретного 'события'
или создания определенного 'условия'.
	B стандартной библиотеке C++ имеются соответствующие средства
в форме 'условных переменных' и 'будущих результатов' ('фьючерсов'), позволяющие справиться с данной задачей,  
а также новые средства синхронизации в форме 'защелок' (latches) и 'барьеров' (barriers).
		*************************************************************************************************
		Все разделы главы 4. Все виды запуска вторичных (дополнительных) потоков
		*************************************************************************************************
		Виды потоков в терминологии стандартов С++

		Основной поток - поток, в котором инициирована функция main();

*/
	void void_func_without_param()
	{
		std::cout << "I'm <void void_func_without_param()>  в потоке " << std::this_thread::get_id() << std::endl;
	}

	typedef int result_type;
	result_type ret_func_with_param(double dbl_par)
	{
		std::cout << "I'm <result_type ret_func_with_param(double dbl_par)>  в потоке " << std::this_thread::get_id() << std::endl;
		
		return static_cast<result_type>(dbl_par);
	}

	typedef double ftrs_par_type;
	
	result_type ret_func_with_prms(std::promise<ftrs_par_type>* prms)
	{
		std::cout << "I'm <result_type ret_func_with_prms(std::promise<ftrs_par_type>* prms)>  в потоке " << std::this_thread::get_id() << std::endl;
		ftrs_par_type promis_val = 3.14;

		prms->set_value(promis_val);

		return static_cast<result_type>(42);
	}
	//	************************* std::thread	***************************************
	void	start_with_thread()
	{
		std::cout << "I'm <void	start_with_thread()>  в потоке " << std::this_thread::get_id() << std::endl;

		std::thread	thrd( void_func_without_param );
		thrd.join();

		std::thread	thrd1( ret_func_with_param, 2.7 );
		thrd1.join();
	}

	//	************************* std::async	***************************************
	void	start_with_async()
	{
		std::cout << "I'm <void	start_with_async()>  в потоке " << std::this_thread::get_id() << std::endl;

		std::future ftrs = std::async(std::launch::async, ret_func_with_param, 7.54);

		ftrs.wait();								// Дополнительная гибкость: можно дождаться завершения здесь,
													// а можно и 
		result_type result = ftrs.get();	// здесь, если не будет использована wait()

		std::cout << "I'm <void	start_with_async()>; result =  " << result << std::endl;
	}

	//	************************* std::packaged_task	***************************************
	void	start_with_packaged_task()
	{
		std::cout << "I'm <void	start_with_packaged_task()>  в потоке " << std::this_thread::get_id() << std::endl;

		std::packaged_task<result_type(double)> task(ret_func_with_param);

		std::future<result_type>  ftrs = task.get_future();
	
		task(5.55);		// Запуск в этом же потоке

		result_type result = ftrs.get();
		std::cout << "I'm <void	start_with_packaged_task()>; result =  " << result << std::endl;
		std::cout << "I'm <void	start_with_packaged_task()>; Функция в текущем потоке завершила работу" << std::endl;
	
		// Запуск в дополнитьельном потоке
		std::packaged_task<result_type(double)> task2(ret_func_with_param);

		std::future<result_type>  ftrs2 = task2.get_future();

		std::thread thrd(std::move(task2), 7.77);

		thrd.join();

		result_type result2 = ftrs2.get();
		std::cout << "I'm <void	start_with_packaged_task()>; result2 =  " << result2 << std::endl;
	}

	//	************************* std::promis	***************************************
	void	start_with_promis()
	{
		std::cout << "I'm <void	start_with_promis()>  в потоке " << std::this_thread::get_id() << std::endl;

		std::promise<ftrs_par_type> 	prms;

		std::future<ftrs_par_type> 	ftrs 		= prms.get_future();

		// ret_func_with_prms(&prms);

		std::thread 					thrd(ret_func_with_prms, &prms);

		ftrs_par_type 				prms_result 	= ftrs.get();

		thrd.join(); 

		std::cout << "I'm <void	start_with_promis()>; prms_result =  " << prms_result << std::endl;
	}

	void thread_async_packaged_promis_test()
	{
		std::cout << "I'm <void	thread_async_packaged_promis_test()>  в потоке " << std::this_thread::get_id() << std::endl;
		//start_with_thread();			// std::thread
		start_with_async();				// std::async
		start_with_packaged_task();		// std::packaged_task
		//start_with_promis();			// std::promis
	}
};	// End of the namespace 'ns_AW_chapter_4'
/*
*	*********************************************************************************
*	РЕЗЮМЕ
*	*********************************************************************************


*/
//		#ifndef GUARD_AW_chapter_4_thrd_sync_pckg_prmshpp
#endif
