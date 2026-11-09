#pragma once
/*
	5_The_Standard_Library_5_4_4.hpp
*/
#include	<chrono>
#include	<iostream>
using std::cout; using std::endl; using std::boolalpha;
#include <iomanip>

#include <numbers>
#include <numeric>
#include <limits>
using std::numeric_limits;
#include <bit>
#include <bitset>

#include <string>
using std::string;
#include <cassert>
#include <cstring>
#include <concepts>
#include <iterator>
#include <optional>

#include <vector>
#include <deque>
#include <list>
#include <forward_list>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <map>

#include <typeinfo> 
#include <type_traits> 
#include <variant>
#include <algorithm>
#include <cmath>
#include <utility>
#include <future>
#include <ranges>
#include <algorithm>
#include <functional>
#include <cctype>
#include <span>
#include <array>
#include <memory>

#include "..\separate_number_with_apostrophe.hpp"

// Для clang и gcc математические библиотеки в разных заголовках описаны
#if defined(__clang__)
	#include <math.h>
#else	
	#include <cmath>
#endif

#include "..\type_name_as_str.h"
/*	
	***************************************************************************************************
	Rainer Grimm 'C++20 Get the Details'
	--------------------------------------------------------------------------------------------
	5. 		The Standard Library
	--------------------------------------------------------------------------------------------
	5.1. 		Библиотека диапазонов
	5.2.		std::span
	5.3.		Улучшения контейнеров
	5.4.		Арифметические функции

	-----------------------------------------------------------------------------------------
	5.4.4 Работа с битами
	
	Заголовочный файл <bit> 
	содержит функции 
	для доступа и манипулирования отдельными битами 
	или последовательностями битов.
	
	5.4.4.1 std::endian
	
	Благодаря новому типу std::endian 
	вы можете проверить порядок байтов (endianness) 
	для заданного скалярного типа. 
	
	Такой порядок может быть 
	
	-	порядком от старшего к младшему (big-endian) 
	
	и 
	
	-	порядком от младшего к старшему (little-endian). 
	
	Порядок 'от старшего к младшему' означает, 
	что наиболее значащий байт – это 'крайний левый'; 
	
	порядок 'от младшего к старшему' означает, 
	что наиболее значащий байт – это 'крайний справа'. 
	
	Под 'скалярным типом' 
	подразумевается:

		-	арифметический тип, 
		-	enum, 
		-	указатель, 
		-	указатель на поле объекта 
		или 
		-	std::nullptr_t. 

	Класс 'endian' проверяет порядок байтов для всех скалярных типов.
	
	 enum class endian
*/	 
enum class endian
{
	little 	= static_cast<int>(std::endian::little)		/*implementation-defined*/,
	big 	= static_cast<int>(std::endian::big)		/*implementation-defined*/,
	native 	= static_cast<int>(std::endian::little)		/*implementation-defined*/
};
/*
	Если все скалярные типы с порядком от младшего к старшему, 
	то std::endian::native равно std::endian::little.
 
	Если все скалярные типы с порядком от старшего к младшему, 
	то std::endian::native равно std::endian::big.
 
	Поддерживаются даже пограничные случаи:
 
	-	если у всех скалярных типов размер равен 1 и порядок байтов не играет никакой роли, 
		то значения std::endian::native, std::endian::little и std::endian::big совпадают.
		
	-	если платформа, на которой выполняется программа, 
		поддерживает смешанный порядок байтов, 
		то std::endian::native не равен ни std::endian::little, ни std::endian::big.
		
	Когда я запускаю программу getEndiannes.cpp на архитектуре х86, 
	то получаю ответ std::endian::little.
	
	enum class endian
*/
#include <bit>
void	test_33(){
	cout << "-------------- I'm void " << __func__ << "()" << endl;
	
    if constexpr (std::endian::native == std::endian::big) {
        cout << "big-endian" << '\n';
    }
    else if constexpr (std::endian::native == std::endian::little) {
        cout << "little-endian"  << '\n';
    }
	/*
		Здесь используется constexpr if для условной компиляции кода. 
		
		Это значит, 
		что компиляция зависит 
		от порядка байтов на архитектуре компьютера, 
		где запущена программа.
	*/
}
/*
	-------------------------------------------------------------------------------------------------------
	5.4.4.2 Работа с битами и битовыми последовательностями
	
	Следующая таблица дает обзор всех функций для работы с битами. 
	
	Вы можете найти их в заголовочном файле <bit>.
	-------------------------------------------------------------------------------------------------------
	 Функция 					Описание
	-------------------------------------------------------------------------------------------------------
	
	std::bit_cast					Выполнить интерпретацию представления объекта
	
	std::has_single_bit			Проверить, является ли число степенью двух
	
	std::bit_ceil					Найти минимальную целую степень двух, не меньшую заданного числа
 	
	std::bit_floor					Найти максимальную целую степень двух, не большую заданного числа
 
	std::bit_width					Найти наименьшее количество битов для представления данного числа

	std::rotl						Выполнить побитовый циклический сдвиг влево
 	
	std::rotr						Выполнить побитовый циклический сдвиг вправо
 	
	std::countl_zero				Найти количество подряд идущих 0, начиная с наиболее значащего бита 
	
	std::countl_one				Найти количество подряд идущих 1, начиная с наиболее значащего бита
 	
	std::countr_zero				Найти количество подряд идущих 0, начиная с наименее значащего бита
 	
	std::countr_one				Найти количество подряд идущих 1, начиная с наименее значащего бита
 	
	std::popcount					Найти количество 1 в беззнаковом целом числе

 	-------------------------------------------------------------------------------------------------------
	Все из этих функций, кроме std::bit_cast, 
	требуют значения беззнакового целого типа 
	(unsigned char, unsigned short, unsigned int, unsigned long или unsigned long long).
*/
#include <bit>
#include <bitset>
void	test_34(){
	cout << "-------------- I'm void " << __func__ << "()" << endl;
    
    std::uint8_t 	num	= 0b00'110'010;
    
    cout << std::boolalpha;
	// Не работает в данном случае - двоичные же числа, не десятичные
	separate_number_with_apostrophe		s_n_w_a;
	s_n_w_a.set_separator();
	
    cout << "std::has_single_bit(0b00'110'010) : " << std::has_single_bit(num)               	<< '\n';
    cout << "std::bit_ceil(0b00'110'010)       : " << std::bitset<8>(std::bit_ceil(num))     	<< '\n';
    cout << "std::bit_floor(0b00'110'010)      : " << std::bitset<8>(std::bit_floor(num)) 		<< '\n';
    cout << "std::bit_width(5u)                : " << std::bit_width(5u) 						<< '\n';
    cout << "std::rotl(0b00110010, 2)          : " << std::bitset<8>(std::rotl(num, 2))       	<< '\n';
    cout << "std::rotr(0b00110010, 2)          : " << std::bitset<8>(std::rotr(num, 2))       	<< '\n';
    cout << "std::countl_zero(0b00'110'010)    : " << std::countl_zero(num) 					<< '\n';
    cout << "std::countl_one(0b00'110'010)     : " << std::countl_one(num) 						<< '\n';
    cout << "std::countr_zero(0b00'110'010)    : " << std::countr_zero(num) 					<< '\n';
    cout << "std::countr_one(0b00'110'010)     : " << std::countr_one(num) 						<< '\n';
    cout << "std::popcount(0b00'110'010)       : " << std::popcount(num) 						<< '\n';
	s_n_w_a.locale_restore();
	
	for (auto i = 2u; i < 8u; ++i) {
        cout << "bit_floor (" << std::bitset<8>(i)  << ")  = " << std::bit_floor(i) 	<< '\n';
        cout << "bit_ceil  ("  << std::bitset<8>(i) << ")  = " << std::bit_ceil(i) 	<< '\n';
        cout << "bit_width (" << std::bitset<8>(i)  << ")  = " << std::bit_width(i) 	<< '\n';
        cout << "popcount  ("  << std::bitset<8>(i) << ")  = " << std::popcount(i) 	<< '\n';   
        cout << '\n';
    }
    cout << '\n';
}
/*
	---------------------------------------------------------------------------------------------
	Краткая информация
	
	Функции cmp_* в С++20 поддерживают безопасное сравнение целых чисел, 
	поскольку они поддерживают сравнение знакового и беззнакового целых чисел. 
	
	В случае небезопасного сравнения возникает ошибка.
	
	Многие математические константы теперь определены.
	
	С++20 предоставляет специальные функции 
	для вычисления среднего значения и линейной интерполяции двух значений.
	
	Доступны новые функции для работы с отдельными битами 
	или последовательностями битов.
*/
/*
	***************************************************************************************************
	5.5 Календарные зоны и часовые пояса
	
	--------------------------------------------------------------------------------------------
	Отсутствие поддержки компилятора
	
	В конце 2020 года ни один компилятор С++20 не поддерживал расширения 'chrono'. 
	
	Благодаря прототипу – библиотеке 'date' от Говарда Хиннанта, 
	которая является надмножеством расширенной функциональности в С++20, – 
	я мог поэкспериментировать. 
	
	Библиотека находится на GitHub. 
	
	Есть несколько способов попробовать этот прототип:
	вы можете попробовать его в Wandbox. 
	
	Говард выложил заголовочный файл date.h, 
	которого вполне достаточно для работы с новым типом std::time_of_day и календарем. 
	
	Вот ссылка от Говарда: Try it on Wandbox!;
	скопируйте заголовочный файл date.h в путь поиска для вашего компилятора С++;
	скачайте проект и соберите его. 
	
	Уже упоминавшаяся страница 'date' на GitHub содержит дополнительную информацию. 
	
	Этот шаг требуется, если вы хотите попробовать новые функции по работе с временными зонами.
	
	Примеры в этой главе используют библиотеку Говарда Хиннанта. 
	
	Но мои объяснения основаны на терминологии С++20. 
	
	Когда компиляторы С++20 будут поддерживать расширенную функцио нальность chrono, 
	я изменю примеры, 
	чтобы они соответствовали синтаксису С++20.
	
	https://github.com/HowardHinnant/date.
	https://wandbox.org/permlink/L8MwjzSSC3fXXrMd.
	https://github.com/HowardHinnant/date.
*/
/*
	С++20 добавляет новые компоненты в библиотеку chrono:
	
	-	время дня 		– 	это продолжительность времени с полуночи, 
							разбитая на часы, минуты, секунды и дробные части секунды;
						
	-	календарь		-	обозначает различные даты в календаре, 
							такие как год, месяц, день, день недели 
							или n-й день недели;
						
	-	часовой пояс		-	представляет собой время, заданное для какой-то географической области.
	
	По сути, функционал часовых поясов (С++20) 
	основан на функционале календаря (С++20), 
	а функционал календаря (С++20) 
	основан на библиотеке 'chrono' (С++11).
	
	-------------------------------------------------------------------------------------------------------
	Библиотека работы со временем в С++11
	
	Для того чтобы получить наибольшее количество информации из этой главы, 
	важно 'базовое понимание' библиотеки chrono. 
	
	В С++11 имеется три главных компонента для работы со временем:
	
	-	'точка во времени' (time point, 'момент времени') 
		определяется 'стартовым временем' ('момент начала отсчета'), 
		так называемым 'epoch' 
		и 
		дополнительной 'продолжительностью' (duration);
		
	-	'продолжительность' (интервал) 	– 	это разница во времени между 
											двумя точками во времени. 
											
											Определяется 'числом отсчетов' (ticks);
											
	-	часы 							– 	состоят из 'стартовой точки отсчета во времени' 
											'числa отсчетов' (ticks)
											так что может быть вычислена 'текущая точка во времени'.
											
	Честно говоря, все это довольно сложно. 
	
	С одной стороны, у каждого есть свое интуитивное понятие времени, 
	но с другой – крайне сложно дать ему формальное определение. 
	
	Например, три компонента – 
	
	-	точка во времени (момент времени), 
	-	продолжительность 
	и 
	-	часы – 
	
	зависят друг от друга. 
	
	Если вы хотите узнать больше о функциональности, 
	связанной со временем в С++11, 
	то почитайте мой пост об этом.
	https://www.modernescpp.com/index.php/tag/time
	
	Но это еще не все. 
	
	С++20 вводит несколько новых часов. 
	
	Благодаря библиотеке форматирования в С++20 
	интервалы времени 
	могут быть удобно записаны или прочитаны.
	
	---------------------------------------------------------------------------------------
	5.5.1 Время дня (Time of Day)
 
	std::chrono::hh_mm_ss 	– 	это время начиная с полуночи, 
							разбитое на 
							-	часы, 
							-	минуты, 
							-	секунды 
							и 
							-	дробные части секунды. 
	
	Этот тип обычно используется при форматировании. 
	
	Следующая таблица дает вам краткий обзор tOfDay 	– 	экземпляра класса std::chrono::hh_mm_ss.
	---------------------------------------------------------------------------------------------------
	Функция						 	Описание
	---------------------------------------------------------------------------------------------------
	std::chrono::hh_mm_ss			tOfDay;
	
	tOfDay.hours()				Возвращает количество часов с полуночи (компоненту часов)
 
	tOfDay.minutes()				Возвращает количество минут, прошедших с полуночи
	
	tOfDay.seconds() 				Возвращает количество секунд для времени, прошедшего с полуночи 
	 
	tOfDay.subseconds() 			Возвращает количество дробного количества секунд для времени, прошедшего с полуночи 
	
	tOfDay.to_duration() 			Возвращает продолжительность времени с полуночи
	
	std::chrono::make12(hour) 		Возвращает 12-часовой эквивалент для 24-часового времени
	
	std::chrono::make24(hour) 		Возвращает 24-часовой эквивалент для 12-часового времени
	
	std::chrono::is_am(hour) 		Проверяет, является ли заданное 24-часовое время AM
 
	std::chrono::is_pm(hour) 		Проверяет, является ли заданное 24-часовое время PM
	---------------------------------------------------------------------------------------------------
	Использование этих функций довольно просто.
*/
//	#include "date.hpp"
void	test_35(){
	cout << "-------------- I'm void " << __func__ << "()" << endl;
	
    using namespace 	std::chrono_literals; 

    cout << std::boolalpha << '\n';
    
    std::chrono::hh_mm_ss 	timeOfDay{10.0h + 18min + 20s + 0.5s};
    
    cout << "timeOfDay: " << timeOfDay << '\n';                             
    cout << '\n';
	
    cout << "timeOfDay.hours()       : " << timeOfDay.hours() 			<< '\n';            
    cout << "timeOfDay.minutes()     : " << timeOfDay.minutes() 		<< '\n';         
    cout << "timeOfDay.seconds()     : " << timeOfDay.seconds() 		<< '\n';    
	#if defined(__clang__)
		cout << "clang++ отдыхает :( Надо скомпилировать с gcc++." << '\n';    
	#else	
		cout << "timeOfDay.subseconds()  : " << timeOfDay.subseconds() 		<< '\n';   
		cout << "timeOfDay.to_duration() : " << timeOfDay.to_duration() 	<< '\n'; 
	#endif
	
    cout << '\n';

    cout << "std::chrono::hh_mm_ss(45700.5s) : " << std::chrono::hh_mm_ss(45700.5s) << '\n'; 
    cout << '\n';
	/*
    cout << "std::chrono::hh_mm_ss::is_am(5h)  : " << std::chrono::hh_mm_ss::is_am(5h) 	<< '\n';                   
    cout << "date::is_am(15h) : " << date::is_am(15h) 	<< '\n';              
    cout << '\n';
    
    cout << "date::make12(5h)  : " << date::make12(5h) 	<< '\n';                 
    cout << "date::make12(15h) : " << date::make12(15h) << '\n';   	
	*/          
}
/*
	-----------------------------------------------------------------------------------------------
	5.5.2 Календарные даты	-	5.5.2 Calendar Dates
	
	Новый тип, появившийся в С++20 в библиотеке chrono, – это 'календарная дата' (Calendar Date). 
	
	С++20 поддерживает различные способы задания календарных дат и взаимодействия с ними. 
	
	Но прежде всего давайте разберемся с тем, что такое календарная дата.
	
	'Календарная	дата' 	– 	это дата (день), 
							состоящая из 
							года, месяца и дня. 
	
	Соответственно, в С++20 есть тип 
	
	std::chrono::year_month_day. 
	
	Но в С++20 есть не только это. 
	
	Следующая таблица дает первоначальный обзор типов, связанных с датами:
	-------------------------------------------------------------------------------------------------------
								Различные типы календарных дат
	-------------------------------------------------------------------------------------------------------				
	Тип							Описание
	-------------------------------------------------------------------------------------------------------
	std::chrono::last_spec						Обозначает последний день или день недели месяца
 	
	std::chrono::day							Представляет день месяца
 
	std::chrono::month							Представляет месяц года
  
	std::chrono::year							Представляет год в григорианском календаре
	 
	std::chrono::weekday						Представляет день недели в григорианском календаре
 
  	std::chrono::weekday_indexed				Представляет n-й день недели месяца
 
	std::chrono::weekday_last					Представляет последний день недели месяца
 
	std::chrono::month_day						Представляет конкретный день конкретного месяца
  
	std::chrono::month_day_last				Представляет последний день заданного месяца
  
	std::chrono::month_weekday				Представляет n-й день недели месяца
 	
	std::chrono::month_weekday_last			Представляет последний день недели заданного месяца
  
	std::chrono::year_month					Представляет заданный месяц заданного года
 
	std::chrono::year_month_day				Представляет заданный год, месяц и день
 
	std::chrono::year_month_day_last			Представляет последний день заданного месяца и года
  
	std::chrono::year_month_weekday			Представляет n-й день недели заданного года и месяца
  
	std::chrono::year_month_day_weekday_last	Представляет последний день недели заданного года и месяца
  
	std::chrono::operator 						Создает дату в григорианском календаре	
	-------------------------------------------------------------------------------------------------------------
 
	5.5.2.1 Создание календарных дат
	
	Pазличные способы создания календарных дат.
 
	Создание календарных дат
*/
void	test_36(){
	cout << "-------------- I'm void " << __func__ << "()" << endl;
    
	using namespace std::chrono;
    namespace date = std::chrono;
	
    constexpr auto yearMonthDay{year{2024y}/month{5}/day{22d}}; //	Символ '/' необходим для универсальной инициализации
    cout << yearMonthDay << " ";
    cout << std::chrono::year_month_day(2024y, May, 22d) << '\n';  
    cout << '\n';
	
	//	Последний день мая 2024 года 
    constexpr auto yearMonthDayLast{year(2024y)/May/last};             
    cout << yearMonthDayLast << " ";
    cout << year_month_day_last(2024y, month_day_last(month(5))) << '\n';
	
	//	Второй четверг марта 2020 года
    constexpr auto yearMonthWeekday{year(2020)/March/Thursday[2]};       
    cout << yearMonthWeekday << " ";
    cout << year_month_weekday(2020y, month(March), Thursday[2]) << '\n';

	//	Последний понедельник марта 2010 года
	constexpr auto yearMonthWeekdayLast{year(2010)/March/Monday[last]};  
    cout << yearMonthWeekdayLast << " ";
    cout << year_month_weekday_last(year(2010), month(March), weekday_last(Monday[last])) <<  '\n';

    cout << '\n';

    constexpr auto day_{day(19)};         
    cout << day_  << " ";
    cout << day(19) << '\n';

    constexpr auto month_{month(1)};       
    cout << month_  << " ";
    cout << month(1) << '\n';

    constexpr auto year_{year(1988)};     
    cout << year_  << " ";
    cout << year(1988) << '\n';

    constexpr auto weekday_{weekday(5)};
    cout << weekday_  << " ";
    cout << weekday(5) << '\n';
 
    constexpr auto yearMonth{year(1988)/1};
    cout << yearMonth  << " ";
    cout << year_month(year(1988), January) << '\n';
 
    constexpr auto monthDay{10/day(22)};
    cout << monthDay <<  " ";
    cout << month_day(October, day(22)) << '\n';

    constexpr auto monthDayLast{June/last};
    cout << monthDayLast << " ";
    cout << month_day_last(month(6)) << '\n';
 
    constexpr auto monthWeekday{2/Monday[3]};
    cout << monthWeekday << " ";
    cout << month_weekday(February, Monday[3]) << '\n';
 
    constexpr auto monthWeekDayLast{June/Sunday[last]};
    cout << monthWeekDayLast << " ";
    cout << month_weekday_last(June, weekday_last(Sunday)) << '\n';
	/*
		Есть два разных способа создания календарных дат. 
		
		Вы можете использовать 'специальный синтаксис' (cute syntax) 
		
		yearMonthDay{year(1940)/month(6)/day(26)} 
		или 
		
		'явно заданный тип' 
		
		year_month_day(1940y, June, 26d). 
		
		'Явный тип' достаточно интересен, 
		поскольку он использует литералы даты и времени 
		вроде 1940y, 26d 
		и предопределенные константы вороде June. 
		
		Это была наиболее очевидная часть программы.
 
		Друие строки предлагают другие способы задания дат.
		
		Последний день марта 2010 года 	{year(2010)/March/last} 
		или 

		year_month_day_last(2010y, month_day_last(month(3))).
		
		Второй четверг марта 2020 года: {year(2020)/March/Thursday[2]} 
		или 
		
		year_month_weekday(2020y, month(March), Thursday[2]).
	
		Последний понедельник марта 2010 года: 		{year(2010)/March/Monday[last]} 
		или 
		
		year_month_-weekday_last(2010y, month(March), weekday_last(Monday)).
		
		Оставшиеся календарные типы обозначают день, месяц или год. 
		
		Вы можете совмещать их 
		и использовать как строительные блоки 
		для полного задания дат.
	*/
}
/*
	-------------------------------------------------------------------------------------------------------
	5.5.2.2 Специальный синтаксис		-	 5.5.2.2 Cute Syntax
	
	Этот синтаксис состоит из 'переопределенного оператора деления', 
	используемого для задания даты. 
	
	'Перегруженный оператор' поддерживает 'литералы времени' (например, 2020y, 31d) 
	и константы 
	(January, February, March, April, May, June, July, August, September, October, November, December).
	
	Возможны следующие три комбинации года, месяца и дня при использовании этого синтаксиса.
	
	Специальный синтаксис
	
	year/month/day
	
	day/month/year
 
	month/day/year
	
	Эти комбинации выбраны не произвольно. 
	
	Это используемые по всему миру комбинации. 
	
	Все остальные комбинации не допустимы.
	
	Соответственно, 
	когда вы выбираете тип 
	
	year, month или day 
	
	для своего 'первого аргумента', 
	типы для двух оставшихся уже не важны, 
	и надо указывать просто числа.
	
	Специальный синтаксис
*/
void	test_37(){
	cout << "-------------- I'm void " << __func__ << "()" << endl;

	using namespace std::chrono;
    namespace date = std::chrono;
	
	constexpr auto yearMonthDay{year{1966y}/5/22};
    cout << yearMonthDay << '\n';

    constexpr auto dayMonthYear{day(26)/6/1966};
    cout << dayMonthYear << '\n';

    constexpr auto monthDayYear{month(6)/26/1966};
    cout << monthDayYear << '\n';

	//	is not a valid date	- а кто выдает это сообщение?
	//	Комбинация год/день/месяц недопустима и вызывает ошибку на прогоне!!!! Компилятор пропускает.
    constexpr auto yearDayMonth{year(1966)/month(26)/6};  
    cout << yearDayMonth << '\n';
	/*
		Я полагаю, что вы хотите показывать календарные даты {year(2010)/March/last} 
		в удобочитаемом виде, например 20-03-2010. 

		Для этого используется оператор local_days или sys_days.
	*/
	cout << local_days(monthDayYear) << '\n';
	cout << sys_days(monthDayYear) << '\n';
}
/*
	----------------------------------------------------------------------------------------------
	5.5.2.3 Показ календарных дат
	
	Благодаря std::chrono::local_days() или std::chrono::sys_days() 
	можно 
	'преобразовывать' 
	календарные даты 
	в 
	std::chrono::time_point. 

	Я использую std::chrono::sys_days в своем примере. 

	std::chrono::sys_days основан на std::chrono::system_clock. 

	Показ календарных дат
*/
 
void	test_38(){
	cout << "-------------- I'm void " << __func__ << "()" << endl;

	using namespace std::chrono;
    namespace date = std::chrono;

    constexpr auto yearMonthDayLast{year(2010)/March/last};
    cout << "sys_days(yearMonthDayLast): " << sys_days(yearMonthDayLast)  <<  '\n';

    constexpr auto yearMonthWeekday{year(2020)/March/Thursday[2]};
    cout << "sys_days(yearMonthWeekday): " <<  sys_days(yearMonthWeekday) << '\n';

	constexpr auto yearMonthWeekdayLast{year(2010)/March/Monday[last]};
    cout << "sys_days(yearMonthWeekdayLast): " << sys_days(yearMonthWeekdayLast) << '\n';

    cout << '\n';
	/*
		Константа std::chrono::last позволяет легко определить, 
		сколько дней в месяце. 

		Вывод программы показывает, что 2012 – это високосный год, 
		а 2013 – невисокосный.
	*/
    constexpr auto leapDate{year(2012)/February/std::chrono::last};               
    cout << "sys_days(leapDate): " << sys_days(leapDate) << '\n';

    constexpr auto noLeapDate{year(2013)/February/last};             
    cout << "sys_day(noLeapDate): " << sys_days(noLeapDate) << '\n';
}   
/*
	Пусть у вас есть календарная дата, например year(2100)/2/29. 

	Вашим первым вопросом может быть: корректна ли эта дата?
 
	Различные календарные типы в С++20 имеют встроенный метод ok(). 
	
	Этот метод возвращает true, если дата корректна.
	
	Проверка даты на корректность
*/
void	test_39(){
	cout << "-------------- I'm void " << __func__ << "()" << endl;

	using namespace std::chrono;
    namespace date = std::chrono;

    cout << std::boolalpha << '\n';
    
    cout << "Valid days" << '\n';               
    day 	day31(31);
    day 	day32 = day31 + days(1);

    cout << "  day31: " << day31 << "; ";
    cout << "day31.ok(): "  << day31.ok() << '\n';
    cout << "  day32: " << day32 << "; ";
    cout << "day32.ok(): "  << day32.ok() << '\n';
    cout << '\n';

    cout << "Valid months" << '\n';       
    month month1(1);
    month month0(0);

    cout << "  month1: " << month1 << "; ";
    cout << "month1.ok(): "  << month1.ok() << '\n';
    cout << "  month0: " << month0 << "; ";
    cout << "month0.ok(): "  << month0.ok() << '\n';
    cout << '\n';

    cout << "Valid years" << '\n';      
    year year2020(2020);
    year year32768(-32768);

    cout << "  year2020: " << year2020 << "; ";
    cout << "year2020.ok(): "  << year2020.ok() << '\n';
    cout << "  year32768: " << year32768 << "; ";
    cout << "year32768.ok(): "  << year32768.ok() << '\n';
    cout << '\n';

    cout << "Leap Years"  << '\n';       
    constexpr auto leapYear2016{year(2016)/2/29};
    constexpr auto leapYear2020{year(2020)/2/29};
    constexpr auto leapYear2024{year(2024)/2/29};

    cout << "  leapYear2016.ok(): " << leapYear2016.ok() << '\n';
    cout << "  leapYear2020.ok(): " << leapYear2020.ok() << '\n';
    cout << "  leapYear2024.ok(): " << leapYear2024.ok() << '\n';
    cout << '\n';

    cout << "No Leap Years"  << '\n';   

    constexpr auto leapYear2100{year(2100)/2/29};
    constexpr auto leapYear2200{year(2200)/2/29};
    constexpr auto leapYear2300{year(2300)/2/29};

    cout << "  leapYear2100.ok(): " << leapYear2100.ok() << '\n';
    cout << "  leapYear2200.ok(): " << leapYear2200.ok() << '\n';
    cout << "  leapYear2300.ok(): " << leapYear2300.ok() << '\n';
    cout << '\n';

    cout << "Leap Years"  << '\n';      

    constexpr auto leapYear2000{year(2000)/2/29};
    constexpr auto leapYear2400{year(2400)/2/29};
    constexpr auto leapYear2800{year(2800)/2/29};

    cout << "  leapYear2000.ok(): " << leapYear2000.ok() << '\n';
    cout << "  leapYear2400.ok(): " << leapYear2400.ok() << '\n';
    cout << "  leapYear2800.ok(): " << leapYear2800.ok() << '\n';
    cout << '\n';
}
/*
	Есть два интересных момента, 
	связанных с выводом различных значений. 

	1.
	Во-первых, если значение не действительно, 
	то при выводе мы получаем 
	«is not a valid day», 
	«is not a valid month» 
	или 
	«is not a valid year». 

	2.
	Во-вторых, месяцы выводятся в текстовом представлении.

	---------------------------------------------------------------------------------------------
	Вы можете легко применить вызов ok() к календарной дате. 

	Теперь легко проверить, является ли заданный день високосным,
	и является ли соответствующий год високосным. 

	В распространенном григорианском календаре применяются следующие правила.

	Каждый год, который делится на 4, является високосным годом.

	За исключением годов, которые кратны 100. 

	Они не високосные:
	за исключением годов, которые кратны 400, они високосные.

	Слишком сложно? Программа leapYears.cpp демонстрирует это правило.
	
	Расширенная библиотека chrono делает очень легким расчет разницы между 
	двумя календарными датами.
*/

