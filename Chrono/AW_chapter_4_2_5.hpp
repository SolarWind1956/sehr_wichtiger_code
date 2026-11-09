#ifndef GUARD_AW_chapter_4_2_5_hpp
#define GUARD_AW_chapter_4_2_5_hpp
/*
	Энтони Уильямс. C++. Практика многопоточного программирования. 2-е издание. 2020 год.
	Anthony Williams. C++ Concurrency in Action. Second Edition. 2020.

	Глава	4.	Синхронизация конкурентных операций
	Chapter 4.	Synchronizing concurrent operations
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
	в форме 'условных переменных' и 'будущих результатов' ('фьючерсов'),
	позволяющие справиться с данной задачей,  а также новые средства
	синхронизации в форме 'защелок' (latches) и 'барьеров' (barriers).
	*************************************************************************************************
	Раздел 4.2. Ожидание единичных событий с помощью фьючерсов
	*************************************************************************************************
	Параграф 4.2.5. Ожидание сразу в нескольких потоках
	*************************************************************************************************

	Если наступления одного и того же события нужно дожидаться сразу из нескольких потоков, 
	следует вместо этого объекта воспользоваться объектом std::shared_future.
	/*
		******************************************************************************
		std::promise<> - std::shared_future<> - std::thread 
	*/
	std::mutex	mutx_4_2_5;
	struct calc_functors_4_2_5
	{
		int		index;
		void operator()(std::promise<double>& prms)
		{
			cout << "I'm functor calc_functors в потоке с ID == " 
				 << std::this_thread::get_id() << endl;

			// Здесь промис устанавливает свое шаблонное значение и тем самым
			// приводит связанный фьючерс в состояние готовности
			try {
				cout << "I'm functor calc_functors - все хоккей? :)" << endl;
				// calc() выполняет собственно вычисления или генерирует исключение throw
				prms.set_value(calc());	// Результат вычисления или исключение передается во фьючерс
			}
			catch (...)
			{
				cout << "I'm functor calc_functors - что-то пошло не так :(" << endl;
				prms.set_exception(std::current_exception());
			}
		}

		double calc()	const
		{
			bool cond = true;
			double result;
			if (index != 0)
				result = 3.14 * 1000.0 / static_cast<double>(index) / 1956.0;
			else {
				cout << "I'm double calc() of calc_functors - Отрицательный делитель:(" 
					 << endl;
				throw std::out_of_range("ИСКЛЮЧЕНИЕ!!! Деление на ноль невозможно! :(\n");
			}
			return result;
		}
	};

	struct print_shared_futures_4_2_5
	{
		int		index;
		void operator()(std::shared_future<double> sh_ftrs)
		{
			double result = 0.0;
			// Ждем сигнала от промиса
			sh_ftrs.wait();

			std::lock_guard		lk(mutx_4_2_5);	//	Это чтобы трассировочная печать шла красиво
			cout << "\n--- I'm print_shared_futures_4_2_5; index == " << std::to_string(index) 
				 << " в потоке с ID == " << std::this_thread::get_id() << endl;
			try {
				result = sh_ftrs.get();
			}
			catch (const std::out_of_range& e) {
				cout << e.what();
			}
			catch (...) {
				cout << "Не угадал тип исключения :(" << endl;
			};
			
			cout << "Result == " << result * index<< "; Index == " << index << endl;
		}
	};
	void promis_shared_futures_master_4_2_5()
	{
		cout << "\n****************  Я - void promis_shared_futures_master() в потоке с ID == " 
			 << std::this_thread::get_id() << endl;
		// Создадим промис
		std::promise<double>			prms;
		// ...и получим связанный с ним разделяемый фьючерс
		auto sh_ftrs = prms.get_future().share();

		// Запускаем функтор промиса
		calc_functors_4_2_5		clc_fnctr{};
		clc_fnctr.index			= 10;
		std::thread thrd_prms(clc_fnctr, ref(prms));

		// Этот функтор будет ждать завершения работы другого функтора, 
		// в котором промис по завершении взведет в готовность связанный разделяемый фьючерс sh_ftrs
		// компонентной функцией set_value()
		print_shared_futures_4_2_5	prn_sh_fnctr1{};
		prn_sh_fnctr1.index = 1;
		std::thread thrd_ftrs_1(prn_sh_fnctr1, ref(sh_ftrs));

		// Этот функтор будет ждать завершения работы другого функтора, 
		// в котором промис по завершении взведет в готовность связанный разделяемый  фьючерс sh_ftrs
		// компонентной функцией set_value()
		print_shared_futures_4_2_5		prn_sh_fnctr2{};
		prn_sh_fnctr2.index = 2;
		std::thread thrd_ftrs_2(prn_sh_fnctr2, ref(sh_ftrs));

		thrd_prms.join();

		thrd_ftrs_1.join();
		thrd_ftrs_2.join();
	};	
	/*
		***********************************************************************************************
		Раздел 4.3. Ожидание с ограничением по времени - Waiting with a time limit
		***********************************************************************************************
		
		Все рассмотренные до сих пор блокирующие вызовы 
		выполняли блокировку на неопределенный период времени, 
		приостанавливая поток в ожидании наступления события. 
		
		Во многих случаях этого вполне достаточно, 
		но иногда требуется установить "лимит ожидания".
		
		Есть две разновидности указываемого 'срока ожидания' (timeout): 
		
			-	'продолжительность'	-	'duration-based timeout'	-	
										ожидание длится указанный 'период времени', например 30 мс
				или 
			
			-	'абсолютный срок'	-	'absolute timeout'	-	
										ожидание длится до 'конкретного момента времени'	-
										'specific point in time', 
										например до 7:30:15.045987023 UTC 30 ноября 2011 года. 
										
		Большинство функций ожидания предоставляют варианты,
		соответствующие обеим этим разновидностям.
		
		У вариантов с указанием 'продолжительности' имеется суффикс _for, 
		а 
		у вариантов с 'абсолютным сроком ожидания' -		суффикс _until.

		Поэтому у std::condition_variable 
		есть два переопределения компонентной функции wait_for() 
		и два переопределения компонентной функции wait_until(), 
		соответствующих двум переопределениям wait(). 
		
		Первое переопределение ожидает 
		поступления сигнала, 
		или истечения срока ожидания, 
		или наступления фиктивного пробуждения, 
		
		а второе станет при пробуждении проверять предоставленный предикат 
		и возвратит управление, 
		только когда он равен true (и условной переменной поступил сигнал) 
		или истек срок ожидания.
	*/
	/*
		***********************************************************************************************
		Параграф 4.3.1. Часы (clock) - встречается термин 'таймеры'
		***********************************************************************************************
		Что касается стандартной библиотеки C++, 
		часы являются источником информации о времени (time information).

		Часы являются классом, предоставляющим четыре различных элемента информации:
	
		-	текущее время (time now, current time);

		-	тип значения, используемого для представления показаний времени, получаемых из часов;
	
		-	продолжительность такта (тика) часов (tick period);

		-	одинаковая ли тактовая частота у часов, и считаются ли они в силу этого 'ста­бильными' 
			(steady).

		Текущее время часов (time now, current time) можно получить, 
		вызвав для данного класса часов статическую компонентную функцию now(). 
		
		Например, в результате вызова std::chrono::system_clock::now() 
		будет возвращено текущее время "системных" часов. 

		"Тип момента времени" (type of the time point) для конкретных часов 
		указывается с помощью typedef-компонента time_point, 
		поэтому возвращаемым типом some_clock::now() является some_clock::time_point.

		'Продолжительность такта часов' (tick period of the clock) 
		указывается в виде 'дробного числа секунд' (fractional number of seconds), 
		ко­торое задается typedef-компонентом часов 'period'.

		Поэтому используются также термины 'период' (period) и 'такт' (tick).

		Значение переменной period можно указать как среднюю продолжительность такта часов, 
		"наименьшую возможную продолжительность такта часов" или какое-то иное значение, 
		которое создатели библиотеки посчитали подходящим.

		Нет никакой гарантии, что наблюдаемая в ходе текущего выполнения программы 
		продолжительность такта часов 
		совпадет с продолжительностью, 
		указанной для них в компоненте period.
	
		Если часы ходят с постоянной частотой (ticks at a uniform rate) 
		независимо от того, соответствует эта частота значению period или нет,
		и не допускают подведения (can’t be adjusted), 
		значит, можно считать их стабильными (steady). 
		
		Статический компонент данных класса часов is_steady имеет значение true, 
		если часы стабильны, и false — в противном случае. 
		
		Обычно часы std::chrono::system_clock нестабильны, 
		поскольку допускают подстройку, 
		даже если она выполняется автоматически, 
		чтобы учесть "уход" (дрейф) вперед "локальных часов" (local clock drift). 
		
		Такая подстройка может стать причиной того, 
		что при вызове now() будет возвращено время, 
		предшествующее возвращенному предыдущим вызовом now(), 
		что является нарушением требования к "равномерности хода часов" (uniform tick rate). 
		
		Стабильные часы играют в вычислениях срока ожидания весьма важную роль, 
		поэтому стандартная библиотека C++ предоставляет такие часы 
		в виде std::chrono::steady_clock. 
		
		Другими часами, имеющимися в стандартной библиотеке C++, 
		являются упомянутые ранее std::chrono::system_clock, 
		представляющие "часы реального времени" системы 
		и предлагающие функции для преобразования их моментов времени 
		в значения time_t и обратно, 
		а также std::chrono::high_resolution_clock, 
		которые дают наименьшую возможную продолжительность такта часов 
		(а следовательно, и самое высокое возможное разрешение) среди всех часов, 
		предоставляемых библиотекой. 
		
		Возможно, будет еще и определитель типа typedef для каких-либо других часов. 
		Эти часы наряду с другими средствами работы со временем 
		определены в библиотечном заголовке <chrono>.
		***********************************************************************************************
		Параграф 4.3.2. Продолжительность (duration), можно встретить также термин "длительность"
		***********************************************************************************************
	
		Еще встречается термины 'интервал времени', "длительность".
	
		Продолжительность - это самая простая часть поддержки времени, 
		она регламентируется шаблоном класса std::chrono::duration<T, F>.
	
		Первым параметром шаблона является тип представления (количество единиц продолжительности), 
		например, int, long или double, 
		а вторым - дробное значение "количества секунд", 
		представляющее каждую единицу продолжительности std::ratio<sec_number/sec_number>. 
	
		"Количество единиц продолжительности" можно получить с помощью компонентной функции count().

		Ожидания, основанные на продолжительности, 
		задаются с помощью экземпляра класса std::chrono::duration<>. 
	
		Для удобства представления продолжительности 
		в пространстве имен std::chrno_literals 
		есть несколько предопределенных операторов литеральных суффиксов (появились в C++11). 
	
		Они позволяют упростить код, 
		применяющий жестко запро­граммированные значения продолжительности, 
		например:
	*/
	void	test_4_3_1() {
		cout << "\nI'm promise_futures_4_2_5() в потоке ID == " << std::this_thread::get_id() << endl;

		std::chrono::system_clock::time_point	now_is_4_3_1 = std::chrono::system_clock::now();
		// 'Продолжительность такта часов' - период (period) - 
		// указывается в виде 'дробного числа секунд', 
		// ко­торое задается typedef - компонентом часов period.
		// using std::chrono::system_clock::period	= std::ratio_multiply<std::hecto, std::nano> 
		//											= std::ratio<1, 10 000 000>
		// 
		// Приблизительно 100 наносекунд
		// 
		/*
			https://forum.dlang.org/post/mailman.10326.1563764501.29801.digitalmars-d-bugs@puremagic.com
			https://omegaup.com/docs/cpp/en/cpp/locale/locale/locale.html
			https://omegaup.com/docs/cpp/en/cpp/header/locale.html
			https://stdcxx.apache.org/doc/stdlibug/24-3.html
			de_DE.UTF-8:		«1.234.56.789.000»;
			fr_FR.UTF-8:		«1 234 56 789 000»; какая-то В В В
			et-EE:				«123 456 789 000»;
			en_IN.UTF-8:		«123,456,789,000»;
			en_US.UTF-8:		«123,456,789,000»;
			ja_JP.UTF-8:		«123,456,789,000»;
			nl_NL.UTF-8:		«1.234.56.789.000»;
			English_US.1251:	«123,456,789,000»;
		*/
		const std::string local_sprtr = "et-EE";
		auto period_of_system_clock = std::chrono::system_clock::period();
		cout << "period_of_system_clock == " << "std::ratio<"
			<< period_of_system_clock.num << ", "
			<< std::format(std::locale(local_sprtr), "{:L}",  period_of_system_clock.den)
			<< ">" << endl;
		// то есть, системные часы тикают с частотой 10 миллионов раз в секунду, 
		// то есть, период системных часов составляет 100 наносекунд
		// Однако Нет никакой гарантии, что наблюдаемая в ходе текущего выполнения программы 
		// продолжительность такта часов совпадет с продолжительностью,
		// указанной для них в компоненте period.
		if (std::chrono::system_clock::is_steady)
			cout << "Системные часы таки стабильны" << endl;
		else
			cout << "Системные часы таки HE стабильны" << endl;		// Системные часы таки НЕ стабильны

		auto period_of_steady_clock = std::chrono::system_clock::period();
		cout << "period_of_steady_clock == " << "std::ratio<"
			 << period_of_steady_clock.num << ", "
			 << std::format(std::locale(local_sprtr), "{:L}", period_of_steady_clock.den)
			 << ">" << endl;
		if (std::chrono::steady_clock::is_steady)
			cout << "Стабильные часы таки стабильны" << endl;		// Стабильные часы таки стабильны
		else
			cout << "Стабильные часы таки HE стабильны" << endl;

		auto period_of_high_resolution_clock = std::chrono::high_resolution_clock::period();
		cout << "period_of_high_resolution_clock == " << "std::ratio<"
		  << period_of_high_resolution_clock.num << ", "
		  << std::format(std::locale(local_sprtr), "{:L}", period_of_high_resolution_clock.den)
		  << ">" << endl;

		// Период часов с минимально возможным периодом отсчетов составляет 1 наносекунду
		if (std::chrono::high_resolution_clock::is_steady)
			cout << "Часы с минимально возможным периодом тактов, таки стабильны" << endl;		// Часы с минимально возможным периодом отсчетов таки стабильны
		else
			cout << "Часы с минимально возможным периодом тактов, таки HE стабильны" << endl;
	};
	/*
		***************************************************************************************
	*/
	using namespace		std::chrono_literals;
	auto	one_day_4_3_2						= 24h;
	auto	half_an_hour_4_3_2					= 30min;
	auto	max_time_between_messages_4_3_2		= 30ms;
	/*
		Стандартная библиотека предоставляет в пространстве имен std::chrono 
		целый набор предопределенных typedef-компонентов для различных продолжительностей: 

		nanoseconds, 
		microseconds, 
		milliseconds, 
		seconds, 
		minutes 
		и
		hours. 
	
		Явно заданное преобразование может выполняться с помощью std::chrono::duration_cast<>:
	*/
	std::chrono::milliseconds	ms_4_3_2(54'802);
	std::chrono::seconds		s_4_3_2 = std::chrono::duration_cast<std::chrono::seconds>(ms_4_3_2);

	double some_task_4_3_2()
	{
		cout << "I'm void some_task_4_3_2() в потоке с ID == " << std::this_thread::get_id() << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		return 3.14;
	};
	void do_something_4_3_2_with(double par_reslt) 
	{
		cout << "I'm void do_something_4_3_2_with(double par_reslt) в потоке с ID == " 
			 << std::this_thread::get_id()
			 << "\n и я печатаю результат работы double some_task_4_3_2() == " << par_reslt << endl;
	};
	void test_duration_type_4_3_2()
	{
		cout << "I'm void test_duration_type() в потоке с ID == " << std::this_thread::get_id() 
			 << endl;

		//  Количество единиц продолжительности можно получить с помощью компонентной функции count().
		const std::string	local_sprtr = "et-EE";
		cout << "std::chrono::duration_cast<std::chrono::seconds>(ms_4_3_2) == 54 секунды? "
			 << s_4_3_2.count() << endl;
		cout << "std::chrono::milliseconds(1234).count(), равно 1234 == "
			 << std::format(std::locale(local_sprtr), "{:L}", std::chrono::milliseconds(1'234).count())
			 << endl;
		// Ожидания, основанные на продолжительности, задаются с помощью экземпляра
		// класса std::chrono::duration<>.
		std::future<double>		ftrs = std::async(some_task_4_3_2);
		/*
			Все 'функции ожидания' возвращают 'состояние', 
			показывающее либо 'истечение срока ожидания', 
			либо 'наступление ожидаемого события'. 

			В данном случае ожидается 'готовность фьючерса', следовательно, функция возвращает:
		
			std::future_status::timeout,		если срок ожидания истек, 
												либо 
			std::future_status::ready,			если фьючерс готов до истечения срока ожидания, 
												либо 
			std::future_status::deferred,		если связанная с фьючерсом задача отложена. 
		*/
		// wait_for() вернет future_status::timeout или future_status::ready
		if (ftrs.wait_for(std::chrono::milliseconds(350)) == std::future_status::ready) {
			// Асинхронная функция завершилась раньше заданной временной уставки
			do_something_4_3_2_with(ftrs.get());
			cout << "\ntest_duration_type()> дождалась-таки завершения some_task_4_3_2()" << endl;
		}
		else
			// Асинхронная функция НЕ УЛОЖИЛАСЬ в заданное время уставки
			cout << "void test_duration_type()> не дождалась завершения some_task_4_3_2()" << endl;
	};
	/*
	*************************************************************************************************
	Параграф 4.3.3. Моменты времени (time_point)
	*************************************************************************************************
		'Момент времени' для часов представляется 
		с помощью экземпляра шаблона класса std::chrono::time_point<CLOCK, DURATION>, 
		в первом параметре которого определяется, на какие часы делается ссылка, 
		а во втором - единица измерения (специализация шаблона std::chrono::duration<>). 
	
		Значение "момента времени" представляет собой 
		"промежуток времени" () (кратный заданной "продолжительности"), 
		прошедший с указанного момента времени, 
		называемого 'эпохой отсчета часов' (epoch of the clock). 

		Эта "эпоха" является базовым свойством, 
		которое невозможно запросить или указать напрямую стандартом C++.
	*/
	struct date_and_time
	{ 
		int seconds;
		int minutes;
		int hours;
		int days_of_month;
		int months;
		int years;
	};
	void time_point_test_433()
	{
		cout << "I'm time_point_test_433() в потоке ID == " << std::this_thread::get_id() << endl;
		// Хотя определить начало эпохи невозможно, для заданного компонента
		// time_point можно получить время с начала эпохи, вызвав функцию time_since_epoch().
		cout << endl;
		using system_clock_time_point_t = 
			std::chrono::time_point	<		std::chrono::system_clock
										,	std::chrono::duration<int, std::ratio<1, 1> > // seconds
									>;
		using steady_clock_time_point_t =
			std::chrono::time_point	<		std::chrono::steady_clock
										,	std::chrono::duration<int, std::ratio<1, 1> > // seconds
									>;
		using high_resolution_clock_time_point_t =
			std::chrono::time_point	<		std::chrono::high_resolution_clock
										,	std::chrono::duration<int, std::ratio<1, 1> > // seconds
									>;

		cout << "system_clock_time_point_t type == "
			 << endl
			 << type_name_as_str<system_clock_time_point_t>() << endl;
		cout << "steady_clock_time_point_t type == "
			 << endl
			 << type_name_as_str<steady_clock_time_point_t>() << endl;
		cout << "high_resolution_clock_time_point_t type == "
			 << endl
			 << type_name_as_str<high_resolution_clock_time_point_t>() << endl;
		// Это один и тот же тип: class std::chrono::duration<int, struct std::ratio<1, 1> >
		// Но с поправкой на часы:
		/*
		class std::chrono::time_point<struct std::chrono::system_clock,
							class std::chrono::duration<__int64,struct std::ratio<1,10000000> > >

		class std::chrono::time_point<struct std::chrono::steady_clock,
							class std::chrono::duration<__int64,struct std::ratio<1,1000000000> > >

		class std::chrono::time_point<struct std::chrono::steady_clock,
							class std::chrono::duration<__int64,struct std::ratio<1,1000000000> > >

		Почему-то для часов high_resolution_clock тип оказался
		как для часов steady_clock...

		!!! Посмотрим, что будет дальше
		*/
		cout << endl;

		system_clock_time_point_t				sys_clck_time_point;
		steady_clock_time_point_t				std_clck_time_point;
		high_resolution_clock_time_point_t		hi_res_clck_time_point;
		// Пока что я думаю, что здесь получены моменты времени начала эпох
		// А теперь получим продолжительности от начала эпох в секундах = нули должны получить
		auto tse_sys_clck		= sys_clck_time_point		.time_since_epoch();
		auto tse_std_clck		= std_clck_time_point		.time_since_epoch();
		auto tse_hi_res_clck	= hi_res_clck_time_point	.time_since_epoch();

		cout << "type of std::chrono::time_point(system_clock)::time_since_epoch() == " 
			 << endl
			 << type_name_as_str<decltype(tse_sys_clck)>() << endl;
		cout << "type of std::chrono::time_point(steady_clock)::time_since_epoch() == "
			 << endl
			 << type_name_as_str<decltype(tse_std_clck)>() << endl;
		cout << "type of std::chrono::time_point(high_resolution_clock)::time_since_epoch() == "
			 << endl
			 << type_name_as_str<decltype(tse_hi_res_clck)>() << endl;
		/*
			class std::chrono::duration<__int64,struct std::ratio<1,10000000> >
			class std::chrono::duration<__int64,struct std::ratio<1,1000000000> >
			class std::chrono::duration<__int64,struct std::ratio<1,1000000000> >

			Здесь длительности часов steady_clock и high_resolution_clock совпали 1 наносекунда
			в то время как для часов system_clock 100 наносекунд
		*/
		cout << endl;

		cout << "time_since_epoch() для трех часов" << endl;
		cout << tse_sys_clck	<< endl;
		cout << tse_std_clck	<< endl;
		cout << tse_hi_res_clck << endl;
		// Нули, от понимания пока далеко :(
		// Ну правильно, и должны получить нули - 
		// а сколько должно пройти времени от начала эпох
		// до начала эпох? Ноль!!!
		cout << endl;

		/*
			Текущее время соответствующих часов можно получить, 
			вызвав для данного класса часов статическую компонентную функцию now(). 
		*/
		// А теперь сосчитаем, сколько пройдет от начала эпох до настоящего момента запуска проги
		// Продолжительность в секундах от начала эпохи до now()
		auto sys_clck_time_point_now		= std::chrono::system_clock::now();
		auto std_clck_time_point_now		= std::chrono::steady_clock::now();
		auto hi_res_clck_time_point_now		= std::chrono::high_resolution_clock::now();
		/*
			Тип момента времени для конкретных часов
			указывается с помощью typedef компонента time_point,
			поэтому возвращаемым типом some_clock::now() является some_clock::time_point.
		*/
		cout << "type of std::chrono::system_clock::now() == "
			 << endl
			 << type_name_as_str<decltype(sys_clck_time_point_now)>() << endl;
		cout << "type of std::chrono::steady_clock::now() == "
			 << endl
			 << type_name_as_str<decltype(std_clck_time_point_now)>() << endl;
		cout << "type of std::chrono::high_resolution_clock::now() == "
			 << endl
			 << type_name_as_str<decltype(hi_res_clck_time_point_now)>() << endl;
		cout << endl;
		// class 
		// std::chrono::time_point	<	struct std::chrono::system_clock,	
		//								class  std::chrono::duration<	__int64,
		//																struct std::ratio<1, 10000000>
		//															>
		//							>
		/*
		class std::chrono::time_point<struct std::chrono::system_clock,
							class std::chrono::duration<__int64,struct std::ratio<1,10000000> > >
		class std::chrono::time_point<struct std::chrono::steady_clock,
							class std::chrono::duration<__int64,struct std::ratio<1,1000000000> > >
		class std::chrono::time_point<struct std::chrono::steady_clock,
							class std::chrono::duration<__int64,struct std::ratio<1,1000000000> > >

		Здесь длительности часов steady_clock и high_resolution_clock также совпали 1 наносекунда
		в то время как для часов system_clock 100 наносекунд
		*/
		auto tse_sys_clck_now		= sys_clck_time_point_now	 .time_since_epoch();
		auto tse_std_clck_now		= std_clck_time_point_now	 .time_since_epoch();
		auto tse_hi_res_clck_now	= hi_res_clck_time_point_now .time_since_epoch();

		cout << "type of std::chrono::time_point(system_clock::now())::time_since_epoch() == "
			 << endl
			 << type_name_as_str<decltype(tse_sys_clck_now)>() << endl;
		cout << "type of std::chrono::time_point(steady_clock::now())::time_since_epoch() == "
			 << endl
			 << type_name_as_str<decltype(tse_std_clck_now)>() << endl;
		cout << "type of std::chrono::time_point(high_resolution_clock::now())::time_since_epoch() == "
			 << endl
			 << type_name_as_str<decltype(tse_hi_res_clck_now)>() << endl;
		cout << endl;
		/*
		class std::chrono::duration<__int64,struct std::ratio<1,10000000> >
		class std::chrono::duration<__int64,struct std::ratio<1,1000000000> >
		class std::chrono::duration<__int64,struct std::ratio<1,1000000000> >
		*/
		/*
		Количество единиц продолжительности можно получить с помощью компонентной функции count().
		*/
		const std::string local_sprtr = "et-EE";
		cout << "system_clock::now().time_since_epoch().count() - Продолжительность в 100ns ==  " 
			 << endl
			 << std::format(std::locale(local_sprtr), "{:L}", tse_sys_clck_now.count()) << endl;
		cout << "steady_clock::now().time_since_epoch().count() - Продолжительность в 1ns ==  "
			 << endl 
			 << std::format(std::locale(local_sprtr), "{:L}", tse_std_clck_now.count()) << endl;
		cout << "high_resolution_clock::now().time_since_epoch().count() - Продолжительность в 1ns ==  "
			 << endl 
			 << std::format(std::locale(local_sprtr), "{:L}", tse_hi_res_clck_now.count()) << endl;
		/*
			system_clock::now().time_since_epoch().count() - Продолжительность в 100ns ==
			17 026 867 637 468 152
			steady_clock::now().time_since_epoch().count() - Продолжительность в 1ns ==
			141 402 274 339 600
			high_resolution_clock::now().time_since_epoch().count() - Продолжительность в 1ns ==
			141 402 274 342 900
		*/
		cout << "duration_cast<std::chrono::seconds>" << endl;
		auto sys_std_clck_now_in_sec = duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch());
		cout << std::format(std::locale(local_sprtr), "{:L}", sys_std_clck_now_in_sec) << endl;

		auto tse_std_clck_now_in_sec = duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now().time_since_epoch());
		cout << std::format(std::locale(local_sprtr), "{:L}", tse_std_clck_now_in_sec) << endl;

		auto tse_hi_res_now_in_sec = duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
		cout << std::format(std::locale(local_sprtr), "{:L}", tse_hi_res_now_in_sec) << endl;
		cout << endl;

		// В году вот столько секунд (в секунде 10'000'000 тактов длительностью по 100 наносекунд)
		decltype(auto) sec_in_year = 60 * 60 * 24 * 365;
		cout << "Сосчитаем, сколько примерно лет от начала эпохи для системных часов: "
			<< tse_sys_clck_now.count() / sec_in_year / 10'000'000 << endl;

		cout << "time(nullptr) - Продолжительность в секундах от полуночи 00:00, 01.01.1970 года, "
			 << "то есть, от начала эпохи для системных часов == \n"
			 << std::format(std::locale(local_sprtr), "{:L}", time(nullptr)) << endl;
			 
		cout << "Сосчитаем, сколько это примерно лет: " 
			 << time(nullptr) / sec_in_year << endl;
	};
	void time_point_test_433_2()
	{
		cout << "I'm time_point_test_433_2() в потоке ID == " << std::this_thread::get_id() << endl;
		// Хотя определить начало эпохи невозможно, для заданного компонента
		// time_point можно получить время с начала эпохи, вызвав функцию time_since_epoch().
		cout << endl;
		using system_clock_time_point_t =
			std::chrono::time_point	<		std::chrono::system_clock
			, std::chrono::duration<int, std::ratio<1, 1> > // seconds
			>;
		using steady_clock_time_point_t =
			std::chrono::time_point	<		std::chrono::steady_clock
			, std::chrono::duration<int, std::ratio<1, 1> > // seconds
			>;
		using high_resolution_clock_time_point_t =
			std::chrono::time_point	<		std::chrono::high_resolution_clock
			, std::chrono::duration<int, std::ratio<1, 1> > // seconds
			>;

		cout << "system_clock_time_point_t type == "
			<< endl
			<< type_name_as_str<system_clock_time_point_t>() << endl;
		cout << "steady_clock_time_point_t type == "
			<< endl
			<< type_name_as_str<steady_clock_time_point_t>() << endl;
		cout << "high_resolution_clock_time_point_t type == "
			<< endl
			<< type_name_as_str<high_resolution_clock_time_point_t>() << endl;
		
		cout << endl;
		// Моменты времени, соответствующие началам эпох
		system_clock_time_point_t				sys_clck_time_point;
		steady_clock_time_point_t				std_clck_time_point;
		high_resolution_clock_time_point_t		hi_res_clck_time_point;
		// Пока что я думаю, что здесь получены моменты времени начала эпох
		// А теперь получим продолжительности от начала эпох в секундах = нули должны получить
		auto tse_sys_clck = sys_clck_time_point.time_since_epoch();
		auto tse_std_clck = std_clck_time_point.time_since_epoch();
		auto tse_hi_res_clck = hi_res_clck_time_point.time_since_epoch();

		cout << "type of std::chrono::time_point(system_clock)::time_since_epoch() == "
			<< endl
			<< type_name_as_str<decltype(tse_sys_clck)>() << endl;
		cout << "type of std::chrono::time_point(steady_clock)::time_since_epoch() == "
			<< endl
			<< type_name_as_str<decltype(tse_std_clck)>() << endl;
		cout << "type of std::chrono::time_point(high_resolution_clock)::time_since_epoch() == "
			<< endl
			<< type_name_as_str<decltype(tse_hi_res_clck)>() << endl;
		cout << endl;
		/*
			Текущее время соответствующих часов можно получить,
			вызвав для данного класса часов статическую компонентную функцию now().
		*/
		// А теперь сосчитаем, сколько пройдет от начала эпох до настоящего момента запуска проги
		// Продолжительность в секундах от начала эпохи до now()
		auto sys_clck_time_point_now = std::chrono::system_clock::now();
		auto std_clck_time_point_now = std::chrono::steady_clock::now();
		auto hi_res_clck_time_point_now = std::chrono::high_resolution_clock::now();
		
		auto tse_sys_clck_now = sys_clck_time_point_now.time_since_epoch();
		auto tse_std_clck_now = std_clck_time_point_now.time_since_epoch();
		auto tse_hi_res_clck_now = hi_res_clck_time_point_now.time_since_epoch();
		/*
			Количество единиц продолжительности можно получить с помощью компонентной функции count().
		*/
		const std::string local_sprtr = "et-EE";
		cout << "system_clock::now().time_since_epoch().count() - Продолжительность в 100ns ==  "
			<< endl
			<< std::format(std::locale(local_sprtr), "{:L}", tse_sys_clck_now.count()) << endl;
		cout << "steady_clock::now().time_since_epoch().count() - Продолжительность в 1ns ==  "
			<< endl
			<< std::format(std::locale(local_sprtr), "{:L}", tse_std_clck_now.count()) << endl;
		cout << "high_resolution_clock::now().time_since_epoch().count() - Продолжительность в 1ns ==  "
			<< endl
			<< std::format(std::locale(local_sprtr), "{:L}", tse_hi_res_clck_now.count()) << endl;
		cout << endl;

		/*
		Продолжительности можно БЕЗ компонентной функции count().
		*/
		//const std::string local_sprtr = "et-EE";
		cout << "system_clock::now().time_since_epoch() - Продолжительность в 100ns ==  "
			<< endl
			<< std::format(std::locale(local_sprtr), "{:L}", tse_sys_clck_now) << endl;
		cout << "steady_clock::now().time_since_epoch() - Продолжительность в 1ns ==  "
			<< endl
			<< std::format(std::locale(local_sprtr), "{:L}", tse_std_clck_now) << endl;
		cout << "high_resolution_clock::now().time_since_epoch() - Продолжительность в 1ns ==  "
			<< endl
			<< std::format(std::locale(local_sprtr), "{:L}", tse_hi_res_clck_now) << endl;
	};
/*
	Временной тип данных time_t способен представлять время и поддерживает арифметические операции.

	Этот тип данных используется в качестве возвращаемого значения функции time() 
	и используется в качестве параметра в некоторых других функциях заголовочного файла <ctime>.
	
	Этот тип данных используется для представления целого числа - количества секунд, 
	прошедших после полуночи 00:00, 1 января 1970 года в формате GMT. 
	
	Это обусловлено историческими причинами, связанными со становлением платформы  UNIX. 

	Однако этот тип данных широко используется в библиотеках С++ на всех платформах.
	*************************************************************************************************
*/

/*
	Листинг 4.11. Ожидание условной переменной с указанием определенного срока
*/
	std::condition_variable		cv_4_11;
	bool						done_4_11;
	std::mutex					mtx_4_11;
	// Это рекомендуемый способ ожидания для условных переменных с ограничением по времени, 
	// если функции ожидания wait() не передается никакого предиката.
	// При этом ограничивается 'общая продолжительность цикла' !!!.
	bool wait_loop_4_11()
	{
		cout << "I'm bool wait_loop_4_11() в потоке с ID == " << std::this_thread::get_id() << endl;
		// Обращаем внимание на то, что используются СТАБИЛЬНЫЕ часы
		auto const	timeout		=	std::chrono::steady_clock::now() + std::chrono::milliseconds(500);

		std::unique_lock<std::mutex>	lk(mtx_4_11);
		while (!done_4_11)
		{
			// Лучше wait_until(), чем wait_for()  !!!  в данном случае - смотри ниже...
			// Ожидание происходит в координатах времени стабильных часов -
			// это вряд ли совпадет с физическим временем по системным часам,
			// зато избавит прогу от возможной подводки системных часов
			if (cv_4_11.wait_until(lk, timeout) == std::cv_status::timeout)
			{
				cout << "I'm bool wait_loop_4_11() - TIMEOUT " << endl;
				break;
			};
		}
		// Если в цикле воспользоваться функцией wait_for(), ожидание может продлиться почти все отведенное время, 
		// прежде чем произойдет 'ложное пробуждение', 
		// после чего при следующем прохождении цикла отсчет времени начнется заново.
		// Это может повторяться произвольное количество раз, аннулируя все границы времени ожидания.
		return done_4_11;
	}
	/*
	*************************************************************************************************
		Параграф 4.3.4. Функции, принимающие сроки ожидания
	*********************************************************************************
Класс / пространство имен				Функции								Возвращаемые значения
std::this_thread
пространство имен
										sleep_for(duration)
										sleep until(time point)
																			Неприменимо
std::condition_variable
или
std::condition_variable_
										anywait_for(lock, duration)
										wait_until(lock, time_point)
std::cv_status::timeout
или
std::cv_status::no_timeout
										wait_for(lock, duration, predicate)
										wait_until(lock, time_point, predicate)
																			bool - значение, возвращенное предикатом при пробуждении

std::timed_mutex,
std::recursive_timed_mutex
или
std::shared_timed_mutex
										try_lock_for(duration)
										try lock until(time point)
																			bool - true, если блокировка была получена, в противном случае false

std::shared_timed_mutex
										try_lock_shared_for(duration)
										try lock shared until(time point)
																			bool - true, если блокировка получена, false, если нет

std::unique_lock<TimedLockable>unique_lock(lockable, duration)
unique_lock(lockable,time_point)
Неприменимо - owns_lock() на вновь созданном объекте возвращает true, если блокировка была получена, и false, если нет
										try_lock_for(duration)
										try lock until(time point)
																			bool - true, если блокировка получена, false, если нет
std::shared_lock<SharedTimedLockable>shared_lock (lockable, duration)
shared_lock(lockable,time_point)
Неприменимо - owns_lock() на вновь созданном объекте возвращает true, если блокировка была получена, и false, если нет
										try_lock_for(duration)
										try lock until(time point)
																			bool - true, если блокировка получена, false, если нет 
std::future<ValueType>
или
std::shared_future<ValueType>
										wait_for(duration)
										wait_until(time_point)
																			std::future_status::timeout
																			если срок ожидания истек,
																			std::future_status::ready, если фьючерс готов, или
																			std::future_status::deferred, если фьючерс содержит отложенную функцию, которая еще не запущена
*/
/*
	*************************************************************************************************
	Раздел 4.4. Применение синхронизации операций для упрощения кода
	*********************************************************************************
	Параграф 4.4.1. Функциональное программирование с применением фьючерсов
	*************************************************************************************************
	Понятие 'функционального программирования' (FP) относится к стилю программирования, 
при котором результат вызова функции зависит только от ее параметров и не зависит от любого внешнего состояния. 
	Это перекликается с математическим понятием функции и означает, 
что при неоднократном вызове функции с одними и теми же параметрами будут получены абсолютно одинаковые результаты.
	'Чистая функция' не изменяет никакое внешнее состояние,
производимый ею эффект строго ограничивается возвращаемым значением.
	*************************************************************************************************
	Пункт 4.4.1.1. Быстрая сортировка в стиле функционального программирования
	*************************************************************************************************
	Основной его замысел довольно прост: в списке значений выбирается некий опорный элемент,
а затем список разбивается на два набора значений - тех, что меньше значения опорного элемента, 
и тех, что больше или равны ему. 
	Отсортированная копия списка получается сортировкой двух наборов значений и 
возвращением отсортированного списка значений меньше значения опорного элемента,
за которым следуют опорный элемент и отсортированный список значений, больших или равных ему.
	*************************************************************************************************
	
	//std::list<double> src_list {3.14, 2.73, 5.76, 8.1, 7.2, 7.2};
	//std::list<double> res_list = sequential_quick_sort_4_12(src_list);
	//for (auto lst_elem : res_list) cout << lst_elem << "  ";


	Листинг 4.12. Последовательная реализация Quicksort
*/
	template<typename T>
	std::list<T> sequential_quick_sort_4_12(std::list<T> input)
	{
		if (input.empty())
		{
			return input;
		}
		std::list<T> result;
		// Перемещаем первый элемент из исходного списка в результирующий - это будет 'опорный элемент'
		// из исходного списка этот элемент удаляется 
		result.splice(result.begin(), input, input.begin());			// splice - сращивать
		// Это  'опорный элемент'
		T const& pivot = *result.begin();							// pivot - точка опоры
		// partition() переупорядочивает элементы в диапазоне [first,last). 
		// Все элементы, для которых предикат pred равен true, 
		// помещаются перед элементами, для которых он равен false. 
		// Функция std::partition() переставляет элементы списка на месте и возвращает итератор, 
		// ссылающийся на первый элемент, значение которого не меньше значения опорного элемента.
		auto divide_point = std::partition(input.begin(), input.end(),
																[&](T const& t) {return t < pivot; });
		// Подсписок с элементами, меньше опорного
		std::list<T> lower_part;
		// Вырежем из исходного списка все элементы, меньше опорного и вставим в первый подсписок
		lower_part.splice(lower_part.end(), input, input.begin(), divide_point);
		// Рекурсивно запустим сортировку списка с элементами, меньше опорного
		// и помещаем результат в подсписок new_lower
		auto new_lower( sequential_quick_sort_4_12(std::move(lower_part)) );
		// Рекурсивно запустим сортировку списка с элементами, больше опорного,
		// и помещаем результат в подсписок new_higher
		auto new_higher(	sequential_quick_sort_4_12(std::move(input)) );
		// Добавим к опорному элементу в результирующем списке посписок с большими элементами
		result.splice(result.end(), new_higher);
		// Вставим в результирующий список подсписок с меньшими элементами
		result.splice(result.begin(), new_lower);
		return result;
	}
/*
	*************************************************************************************************
	Пункт 4.4.1.2. Параллельная быстрая сортировка в FP-стиле
	*************************************************************************************************
	Листинг 4.13. Параллельная быстрая сортировка Quicksort с использованием фьючерсов  и FP-стиля.
*/
	//res_list = parallel_quick_sort_4_1_3(src_list);
	template<typename T>
	std::list<T> parallel_quick_sort_4_1_3(std::list<T> input)
	{
		cout << "I'm <parallel_quick_sort_4_1_3()> в потоке с ID == " << std::this_thread::get_id() << endl;
		if (input.empty())
		{
			return input;
		}
		std::list<T> result;
		result.splice(result.begin(), input, input.begin());
		T const& pivot = *result.begin();
		auto divide_point = std::partition(input.begin(), input.end(),
																[&](T const& t) {return t < pivot; });
		std::list<T> lower_part;
		lower_part.splice(lower_part.end(), input, input.begin(), divide_point);
		// До этого момента все было как и прошлом примере
		// Сортировку первого подсписка запустим асинхронно во вспомогательном потоке
		std::future< std::list<T> > 
		new_lower( std::async(&parallel_quick_sort_4_1_3<T>, std::move(lower_part)) );
		// Далее также все, как в предыдущем примере
		auto 
		new_higher( 	parallel_quick_sort_4_1_3(std::move(input)) );

		result.splice(result.end(), new_higher);
		result.splice(result.begin(), new_lower.get());
		
		return result;
	}
	/*
	*************************************************************************************************
	Листинг 4.14. Примерная реализация функции spawn_task (spawn - порождать)
	*************************************************************************************************
*/
	//std::list<double> res_list = parallel_quick_sort_4_14(src_list);
	//for (auto lst_elem : res_list) cout << lst_elem << "  ";
	//testQuickSort();

	#pragma warning(disable : 4996)
	/*
	template<typename F, typename A>
	static std::future<typename std::result_of<F(A&&)>::type> 
	spawn_task(F&& f, A&& a)
	{
		typedef typename std::result_of<F(A&&)>::type result_type;
		std::packaged_task<result_type(A&&)> task(std::move(f));
		std::future<result_type> res(task.get_future());
		std::thread myThread(std::move(task), std::move(a));
		myThread.detach();
		return res;
	}
	template<class T, class Alloc, template<class T, class Alloc> class Container>
	static Container<T, Alloc> 
	parallel_quick_sort(Container<T, Alloc> input)
	{
		if (input.empty())
		{
			return input;
		}

		Container<T, Alloc> result;
		result.splice(result.begin(), input, input.begin());
		T const& partition_val = *result.begin();
		typename Container<T, Alloc>::iterator divide_point = std::partition	(
													input.begin(), input.end(), [&](T const& t) {   return t < partition_val; }
																		);
		Container<T, Alloc> lower_part;
		lower_part.splice(lower_part.end(), input, input.begin(), divide_point);

		std::future<Container<T, Alloc> > new_lower	(
										spawn_task(&parallel_quick_sort<T, Alloc, Container>, std::move(lower_part))
													);

		Container<T, Alloc> new_higher(parallel_quick_sort(std::move(input)));
		result.splice(result.end(), new_higher);
		result.splice(result.begin(), new_lower.get());
		return result;
	}
	static void testQuickSort()
	{
		std::list<int> toSort = { 1, 4, 3, 6, 4, 89, 3 };
		std::for_each(std::begin(toSort), std::end(toSort), [](int n) { cout << n << endl; });
		std::list<int> sorted;
		sorted = parallel_quick_sort(toSort);
		std::for_each(std::begin(sorted), std::end(sorted), [](int n) { cout << n << endl; });
	};
	
	template<typename F, typename A>
	static std::future<typename std::result_of<F(A&&)>::type>
	spawn_task_4_14(F&& f, A&& a)
	{
		using result_type = typename std::result_of<F(A&&)>::type;
		// Cоздаем новую упакованную задачу из переданной как параметр функции
		std::packaged_task<result_type(A&&)>	task(std::move(f));
		// Bызовом компонентной функции get_future() из этой задачи получается фьючерс
		std::future<result_type> result(task.get_future());
		// Запускаем задачу в отдельном вспомогательном потоке
		std::thread thrd(std::move(task), std::move(a));
		// Поток отсоединяем. 
		thrd.detach();
		// и возвращаем фьючерс вызывающей задаче
		return result;
		// Когда вызывающая задача вызовет функцию get() на фьючерсе, она получит результат - список
	};
	*/
	template<typename T>
	std::list<T> parallel_quick_sort_4_14(std::list<T> input)
	{
		cout << "I'm <parallel_quick_sort_4_14()> в потоке с ID == " << std::this_thread::get_id() << endl;
		if (input.empty())
		{
			return input;
		}
		std::list<T> result;
		result.splice(result.begin(), input, input.begin());
		T const& pivot		= *result.begin();
		auto divide_point		= std::partition(input.begin(), input.end(), [&](T const& t) {return t < pivot; });
		std::list<T> lower_part;
		lower_part.splice(lower_part.end(), input, input.begin(), divide_point);
		// До этого момента все было как и прошлом примере

		// Сортировку первого подсписка запустим в оболочке std::packaged_task<> во вспомогательном потоке
		std::future<std::list<T> > new_lower_ftrs			// Это еще не список - это фьючерс
		(
			// spawn_task породит поток std::thread с задачей, упакованной в std::packaged_task<> так,
			// чтобы потом можно было получить результат работы из фьючерса
			spawn_task_4_14(&parallel_quick_sort_4_14<T>, std::move(lower_part))
		);

		auto new_higher									// Это список - это НЕ фьючерс
		( 
			parallel_quick_sort_4_14(std::move(input)) 
		);

		result.splice(result.end(), new_higher);				// Здесь список уже готов
		result.splice(result.begin(), new_lower_ftrs.get());	// Из фьючерса здесь получаем список

		return result;
	};
	
};	// End of the namespace 'ns_AW_chapter_4'
/*
*	*********************************************************************************
*	РЕЗЮМЕ
*	*********************************************************************************


*/
//		#ifndef GUARD_AW_chapter_4_2_5_hpp
#endif
