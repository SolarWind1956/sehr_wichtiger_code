//clock_timepoint_duration.cpp
/*
	***********************************************************************************************
	Продолжительность (duration)
	***********************************************************************************************
	
	Еще встречаются термины 'интервал времени', 
								'промежуток времени', 
									'временной промежуток', 
										"длительность".
		
	Продолжительность - это самая простая часть поддержки времени, 
	она регламентируется шаблоном класса std::chrono::duration<REPRESENTATION, PERIOD>.
		
	Первым параметром шаблона является "тип представления" (type of the representation) - 
	количество единиц продолжительности, 
	например, int, long или double, 

	а вторым - дробное значение "количества секунд", 
	представляющее каждую единицу продолжительности std::ratio<sec_number/sec_number>. 
		
	"Количество единиц продолжительности" можно получить с помощью компонентной функции count().

	Ожидания, основанные на продолжительности, 
	задаются с помощью экземпляра класса std::chrono::duration<>. 
		
	Для удобства представления продолжительности 
	в пространстве имен std::chrno_literals 
	есть несколько предопределенных операторов литеральных суффиксов (появились в C++11). 

	https://en.cppreference.com/w/cpp/chrono/duration_values
	
	std::chrono::duration_values
 
	C++ Utilities library Date and time utilities std::chrono::duration std::chrono::duration_values 
	Defined in header <chrono>

	template<class Rep>
	struct duration_values;

	The std::chrono::duration_values type defines three common durations:

	std::chrono::duration_values::zero
	std::chrono::duration_values::min
	std::chrono::duration_values::max

	The zero, min, and max static member functions in std::chrono::duration forward their work to these functions.

	This type can be specialized if the representation Rep requires a specific implementation to return these duration objects.

	Member functions
	zero						returns a zero-length representation
	min							returns the smallest possible representation
	max							returns the largest possible representation
*/
template <class _Rep, class _Period = ratio<1>>
class duration
{
public:
    using 	rep    	= _Rep;
    using 	period 	= typename _Period::type;
	
	// Количество тактов в интервале времени для процессора 3 GHz ~0.3 ns - округлить в большую сторону ~1 ns
	constexpr _Rep 	count() const { return _MyRep; }
	
	static constexpr duration zero() noexcept {
		// get zero value
		return duration(duration_values<_Rep>::zero());
	}
	// Это чтобы не запустились злополучные макросы max() и min()
	static constexpr duration(min)() noexcept {
		// get minimum value
		return duration((duration_values<_Rep>::min)());
	}
	static constexpr duration(max)() noexcept {
		// get maximum value
		return duration((duration_values<_Rep>::max)());
	}
private:
	_Rep 	_MyRep; 				// the stored rep
};
//	В стандарте языка C++ предопределены следующие наиболее важные типы промежутков времени:
typedef 	duration<signed int, nano> 			nanoseconds;
typedef 	duration<signed int, micro> 		microseconds;
typedef 	duration<signed int, millT> 		milliseconds;
typedef 	duration<signed int> 				seconds;
typedef 	duration<signed int, ratio<60>> 	minutes;
typedef 	duration<signed int, ratio<3600>> 	hours;
/*
/*
	***********************************************************************************************
	Момент времени (time point)
	***********************************************************************************************
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
template <class _Clock, class _Duration = typename _Clock::duration>
class time_point { // represents a point in time
public:
	using clock    	= 	_Clock;
	using duration 	= 	_Duration;
	using rep      	= 	typename _Duration::rep;
	using period   	= 	typename _Duration::period;
	
	constexpr 	_Duration 	time_since_epoch() const  {
		return _MyDur;
	}
	static constexpr time_point(min)() noexcept {
		return time_point((_Duration::min)());
	}
	static constexpr time_point(max)() noexcept {
		return time_point((_Duration::max)());
	}

private:
    _Duration 	_MyDur{duration::zero()}; 		// duration since the epoch
};

/*	
	***********************************************************************************************
	Часы 		- std::chrono::some_clock
	***********************************************************************************************
	
	Часы являются источником информации о времени (time information).

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
	
	
*/

struct system_clock { 
	using rep                       = long long;
	using period                    = ratio<1, 10'000'000>; // 100 nanoseconds
	using duration                  = duration<rep, period>;
	using time_point                = time_point<system_clock>;
	
	static 	constexpr 	bool 	is_steady = false;

	static time_point 			now(); // Возвращает момент времени с длительностью в сотнях наносекунд от начала эпохи
	static __time64_t 			to_time_t(const time_point& _Time);	// В секундах от начала эпохи
	static time_point 			from_time_t(__time64_t _Tm);
};

struct steady_clock { // wraps QueryPerformanceCounter
	using rep                       = long long;
	using period                    = nano;
	using duration                  = nanoseconds;
	using time_point                = time_point<steady_clock>;
	
	static 	constexpr 	bool 	is_steady = true;
	static 	time_point 			now();	// Возвращает в наносекундах от начала эпохи
};

using 	high_resolution_clock = steady_clock; 