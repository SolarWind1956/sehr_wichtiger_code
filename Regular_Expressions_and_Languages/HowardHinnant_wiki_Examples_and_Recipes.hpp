#pragma once
/*
	HowardHinnant_wiki_Examples_and_Recipes.hpp
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
#include <cstdlib>
#include "date\date.h"
#include "date\tz.h"
#include "..\separate_number_with_apostrophe.hpp"

// Для clang и gcc математические библиотеки в разных заголовках описаны
#if defined(__clang__)
	#include <math.h>
#else	
	#include <cmath>
#endif

#include "..\type_name_as_str.h"

using namespace date;
using namespace std::chrono;
//namespace 		date = std::chrono;

/*	
	***************************************************************************************************
	https://github.com/HowardHinnant/date/wiki/Examples-and-Recipes
	--------------------------------------------------------------------------------------------
	Примеры и рецепты
 
	Говард Хиннант, 1 августа 2023 г. 
	--------------------------------------------------------------------------------------------
	
	Текущее местное время
	( Говард Хиннант )

	Это не может быть проще:
*/
void	test_1(){
	cout << "-------------- I'm void " << __func__ << "()" << endl;
	
	#if defined(__clang__)
		cout << "clang++  имеет проблемы с zoned_time() и std::chrono::current_zone()" << endl;
	#else	
		cout << zoned_time	{	std::chrono::current_zone()
							, 	std::chrono::system_clock::now()
							};
	#endif
}
/*
	system_clock::now() конечно, получает текущее время. 

	Де-факто стандартом является то, 
	что текущее время измеряется в единицах времени Unix , 
	что очень близко к UTC. 

	zoned_time()	связывает текущее время UTC 
	с текущим местным часовым поясом компьютера. 

	У zoned_time() есть потоковый оператор вывода на консоль, 
	поэтому вы можете легко его распечатать:

	2024-05-23 23:01:05.818378 EDT

	Обратите внимание, 
	что точность этого вывода соответствует той точности, 
	которую system_clock::now() поддерживает.

	Если вы не хотите, чтобы аббревиатура часового пояса печаталась, 
	другой вариант — вызвать функцию-член 
	to_local() самой time_zone():
*/
void	test_2(){
	cout << "-------------- I'm void " << __func__ << "()" << endl;

    //using date::operator<<;
	#if defined(__clang__)
		cout << "clang++  имеет проблемы с zoned_time() и std::chrono::current_zone()" << endl;
		//cout << date::current_zone()->to_local(std::chrono::system_clock::now()) << '\n';
	#else	
		cout << current_zone()->
								to_local(std::chrono::system_clock::now()) << '\n';
	#endif
}
/*
	2016-07-05 23:01:05.818378
	
	using date::operator<<;	
	
	необходимо, потому что результатом to_local() является std::chrono::time_point, 
	и ADL не сможет найти своего оператора потоковой передачи.
	
	-------------------------------------------------------------------------------------------
	2.
	Текущее время где-то в другом месте
	(Говард Хиннант )

	Если вам нужно узнать текущее время, где вас нет , то это тоже простой вопрос:

#include "date/tz.h"
#include <iostream>

int
main()
{
    cout << date::zoned_time{"Asia/Shanghai",
                                  std::chrono::system_clock::now()} << '\n';
}
Это выводит текущее время в Шанхае, например:

2016-07-06 11:01:05.818378 CST
Поддерживаются все имена часовых поясов IANA (или ссылки — псевдонимы часовых поясов).

Как и в случае с текущим местным временем, если вам просто нужно местное время time_point(без сокращения часового пояса), вы можете работать напрямую с ним, time_zoneесли хотите:

#include "date/tz.h"
#include <iostream>

int
main()
{
    using date::operator<<;
    cout << date::locate_zone("Asia/Shanghai")->
                                   to_local(std::chrono::system_clock::now()) << '\n';
}
2016-07-06 11:01:05.818378
Если locate_zone()не удается найти time_zone, он выдаст std::runtime_errorполезное what()сообщение.


Получите текущую разницу между любыми двумя произвольными часовыми поясами
( Говард Хиннант )

Хотели бы вы просто узнать, на сколько часов впереди или позади ваш друг находится в другом часовом поясе? Вот как это можно сделать с помощью этой библиотеки.

#include "date/tz.h"
#include <iostream>

int
main()
{
    auto current_time = std::chrono::system_clock::now();
    auto la = date::zoned_time{"America/Los_Angeles", current_time};
    auto sy = date::zoned_time{"Australia/Sydney", current_time};
    cout << date::format("%T\n", sy.get_local_time() - la.get_local_time());
}
Допустим, вы хотите узнать, на сколько часов вперед Сидней до Лос-Анджелеса (сейчас). Сначала получите текущее время UTC. Затем создайте zoned_timeдля «America/Los_Angeles» и «Australia/Sydney», используя текущее время UTC. Затем вычтите их local_times. format— удобный инструмент форматирования для распечатки этой продолжительности:

17:00:00.000000
Поскольку мы не указали иное, по умолчанию нам предоставляется полная точность system_clock::time_point. Если вы предпочитаете другое поведение (например, с точностью до минут), этого легко добиться, приложив немного больше усилий:

cout << date::format("%H:%M\n", sy.get_local_time() - la.get_local_time());
И теперь вывод:

17:00

Установите одновременную встречу в двух разных часовых поясах
( Говард Хиннант )

Допустим, вы хотите организовать видеоконференцию между Нью-Йорком и Москвой. Это можно сделать несколькими простыми способами. Сначала вам нужно решить, когда будет встреча по чьим-то часам. Например, предположим, что мы хотим провести встречу 8 июля 2016 года в 9 утра в Нью-Йорке. Как нам определить это время, а затем найти тот же самый момент в Москве?

#include "date/tz.h"
#include <iostream>

int
main()
{
    using namespace std::chrono;
    using namespace date;
    zoned_time ny{"America/New_York", local_days{July/8/2016} + 9h};
    zoned_time moscow{"Europe/Moscow", ny};
    cout << ny << '\n';
    cout << moscow << '\n';
}
Использование local_daysсоздает календарную дату, локальную для любого часового пояса, с которым вы ее связываете (в данном случае «Америка/Нью_Йорк»). Чтобы сделать это время, отличное от полуночи, просто добавьте продолжительность времени с полуночи (часы, минуты, секунды и т. д.) к local_days, в данном случае 9hк 09:00:00. Это формирует zoned_timeзначение, соответствующее 08.07.2016, 09:00:00 по восточному времени. Чтобы найти то же время в Москве, просто создайте новое zoned_timeс «Европой/Москвой» и Нью-Йорком zoned_time. Это создает zoned_timeэквивалентное время UTC, но связанное с часовым поясом «Европа/Москва».

Затем просто распечатайте его:

2016-07-08 09:00:00 EDT
2016-07-08 16:00:00 MSK
Очевидно, вы могли бы с тем же успехом указать время встречи в часовом поясе Москвы, а затем найти эквивалентное время в Нью-Йорке:

    zoned_time moscow{"Europe/Moscow", local_days{8_d/July/2016} + 16h};
    zoned_time ny{"America/New_York", moscow};
Это приведет к точно такому же результату. Для тех, кто обратил внимание, я изменил порядок даты с м/д/г на д/м/г, просто чтобы показать, что я могу. Смысл тот же.

Иногда удобно указать время независимо от часового пояса. Например, это может быть какое-то небесное событие, такое как затмение. Часто такие события записываются в формате UTC, чтобы людям во всех часовых поясах было легче узнать правильное время. В этом примере так же легко использовать UTC:

    auto utc = sys_days{2016_y/July/8} + 13h;
    zoned_time ny{"America/New_York", utc};
    zoned_time moscow{"Europe/Moscow", utc};
Я изменил порядок даты на г/м/д, просто чтобы показать, что могу. Пока первый модуль однозначен ( year, monthили day), следующие два однозначны (принимаются только y/m/d, d/m/yи m/d/y; все остальные отклоняются во время компиляции).

Вместо local_days, sys_daysиспользуется вместо. sys_daysозначает UTC (технически это означает время Unix , которое очень близко к UTC). Затем вы можете создать каждый из них zoned_timeс указанием времени UTC ( sys_time<hours>в этом примере оно имеет тип). Вы также можете построить второе zoned_timeиз первого, как и раньше:

    zoned_time ny{"America/New_York", sys_days{July/8/2016} + 13h};
    zoned_time moscow{"Europe/Moscow", ny};
В любом случае вывод по-прежнему:

2016-07-08 09:00:00 EDT
2016-07-08 16:00:00 MSK

Взаимодействие с C API
( Говард Хиннант )

Цель этой библиотеки — гарантировать, что вам никогда больше не придется беспокоиться об API синхронизации C. При этом реальный мир иногда мешает моим целям, и нужно просто конвертировать в/из tm, или timespecвремя от времени для взаимодействия с чужим кодом, который все еще использует API синхронизации C. Итак, эта заметка о том, как написать эти функции преобразования:

Преобразование изtm
C позволяет tmсодержать больше членов, чем указано в спецификации C. В этом примере предполагается только переносимые члены tm. Надеемся, что как только вы поймете, как работать с переносимыми участниками, вы сможете при необходимости распространить свои знания на непереносимых участников вашей платформы. Портативный tm— это:

struct tm
{
    int tm_sec;   // seconds after the minute — [0, 60]
    int tm_min;   // minutes after the hour — [0, 59]
    int tm_hour;  // hours since midnight — [0, 23]
    int tm_mday;  // day of the month — [1, 31]
    int tm_mon;   // months since January — [0, 11]
    int tm_year;  // years since 1900
    int tm_wday;  // days since Sunday — [0, 6]
    int tm_yday;  // days since January 1 — [0, 365]
    int tm_isdst; // Daylight Saving Time flag
};
Эту tmструктуру можно использовать для хранения времени UTC ( sys_timeили utc_time) или местного времени ( local_time). Однако он tmне содержит достаточно информации для определения часового пояса местного времени или даже смещения UTC (некоторые платформы включают элемент смещения utc в качестве соответствующего расширения). Кроме того, tmточность ограничена секундами. Это ограничивает то, во что мы можем конвертировать. Например, невозможно преобразовать в a, zoned_timeпоскольку для этого недостаточно информации. И имеет смысл конвертировать в пункт назначения только с точностью до секунды. Если вы хотите преобразовать во что-то с более высокой точностью, это неявное преобразование из результата этих функций с точностью до секунды.

Таким образом, мы можем преобразовать a в tma sys_secondsили в a local_seconds. Вот первое:

date::sys_seconds
to_sys_time(std::tm const& t)
{
    using namespace date;
    using namespace std::chrono;
    return sys_days{year{t.tm_year + 1900}/(t.tm_mon+1)/t.tm_mday} +
           hours{t.tm_hour} + minutes{t.tm_min} + seconds{t.tm_sec};
}
Как видно, не все поля tmнеобходимы для этого преобразования. И это преобразование предполагает, что tmпредставляет собой момент времени в формате UTC. Само преобразование довольно простое: преобразуйте информацию о году/месяце/дне в формат sys_days, а затем добавьте информацию H:M:S, попутно преобразуя ее в длительность хроно. Нужно быть осторожным с данными за год и месяц, чтобы tmправильно сместить их.

Преобразование в a local_secondsидентично, за исключением использования local_daysвместо sys_days:

date::local_seconds
to_local_time(std::tm const& t)
{
    using namespace date;
    using namespace std::chrono;
    return local_days{year{t.tm_year + 1900}/(t.tm_mon+1)/t.tm_mday} +
           hours{t.tm_hour} + minutes{t.tm_min} + seconds{t.tm_sec};
}
Это может дать вам местное время в еще не указанном часовом поясе. Впоследствии можно соединить это local_timeс часовым поясом, например, current_zone()для завершения преобразования (при условии, что current_zone()это правильный часовой пояс).

Преобразование вtm
Преобразование в a tmнемного более гибкое, поскольку обычно у нас более чем достаточно информации для заполнения файла tm. Начнем с преобразования из zoned_secondsв tm:

std::tm
to_tm(date::zoned_seconds tp)
{
    using namespace date;
    using namespace std;
    using namespace std::chrono;
    auto lt = tp.get_local_time();
    auto ld = floor<days>(lt);
    time_of_day<seconds> tod{lt - ld};  // <seconds> can be omitted in C++17
    year_month_day ymd{ld};
    tm t{};
    t.tm_sec  = tod.seconds().count();
    t.tm_min  = tod.minutes().count();
    t.tm_hour = tod.hours().count();
    t.tm_mday = (ymd.day() - 0_d).count();
    t.tm_mon  = (ymd.month() - January).count();
    t.tm_year = (ymd.year() - 1900_y).count();
    t.tm_wday = (weekday{ld} - Sunday).count();
    t.tm_yday = (ld - local_days{ymd.year()/January/1}).count();
    t.tm_isdst = tp.get_info().save != minutes{0};
    return t;
}
Предполагается, что кто-то хочет преобразовать местное время в zoned_timeформат tm. Если это предположение не так, мы рассмотрим размещение времени UTC в таблице tmниже. Итак, первое, что нужно сделать, это получить местное время из файла zoned_secondswith tp.get_local_time().

Затем мы усекаем local_secondsиспользование local_daysфункции floor<days>(lt). Теперь у нас есть две local_time time_points, одна с точностью до секунд, а другая с точностью до дней. Точность дней time_pointможно явно преобразовать в year_month_dayобъект, чтобы мы могли получить поля года, месяца и дня.

Время суток — это просто разница между точностью секунд time_pointи точностью дней time_point, что дает нам секунды с полуночи. Эту длительность можно разбить на {hours, minutes, seconds}структуру, преобразовав ее в файл time_of_day<seconds>. В C++17 <seconds>параметр шаблона будет определяться по продолжительности в секундах, используемой в конструкторе.

Теперь мы просто начинаем заполнять tm, стараясь правильно указать месяц и год. Также рекомендуется сначала обнулить все, tmчтобы обнулить все поля, специфичные для платформы, в файле tm.

Вычисление дней с 1 января производится простым вычитанием выражения для дня Нового года текущего года из уже сохраненного local_daysзначения.

Наконец, мы можем установить tm_isdstзначение 1, если saveчлен sys_infoне является 0min, и 0 в противном случае. Его sys_infoможно получить из zoned_seconds, и он содержит все виды полезной информации (включая смещение UTC, если вы захотите установить его в свой специфичный для платформы tm).

Если мы хотим преобразовать a в sys_secondsa tm, это довольно легко сделать, используя приведенную выше функцию преобразования:

std::tm
to_tm(date::sys_seconds tp)
{
    return to_tm(date::zoned_seconds{tp});
}
Это создает zoned_timeи по умолчанию устанавливает time_zoneзначение «UTC», а затем передает его zoned_timeв to_tm. При желании можно было бы повторить код, zoned_secondsа не использовать его повторно. Это сэкономит небольшое количество времени, затрачиваемого на поиск UTC в базе данных. И в этом случае вы всегда будете устанавливать t.tm_isdstзначение 0. В этой альтернативе можно также использовать sys_daysвместо local_days.

Также можно создать tmfrom local_seconds:

std::tm
to_tm(date::local_seconds tp)
{
    auto tm = to_tm(date::sys_seconds{tp.time_since_epoch()});
    tm.tm_isdst = -1;
    return tm;
}
В этом варианте time_zoneнеизвестно и, следовательно, -1является правильным значением для tm.tm_isdst.

Преобразование изtimespec
timespecявляется новым в последней спецификации C. Он содержит как минимум следующие элементы в любом порядке:

struct timespec
{
    time_t tv_sec;  // whole seconds -- >= 0
    long   tv_nsec; // nanoseconds -- [0, 999999999]
};
C использует timespecкак момент времени, так и продолжительность времени. Таким образом, мы должны иметь возможность конвертировать как в nanoseconds(a duration), так и sys_time<nanoseconds>в (a time_point). И то, и другое легко. Сначала преобразуем в продолжительность:

std::chrono::nanoseconds
to_nanoseconds(timespec const& ts)
{
    using namespace std::chrono;
    return seconds{ts.tv_sec} + nanoseconds{ts.tv_nsec};
}
Нужно просто преобразовать интегралы в соответствующие durationтипы и сложить их. Результат имеет тип nanoseconds.

Мы можем повторно использовать приведенную выше функцию для преобразования в time_point:

date::sys_time<std::chrono::nanoseconds>
to_time_point(timespec const& ts)
{
    return date::sys_time<std::chrono::nanoseconds>{to_nanoseconds(ts)};
}
Просто получите продолжительность и явно преобразуйте ее в правильный формат time_point.

Преобразование вtimespec
Обратные преобразования лишь немного сложнее:

timespec
to_timespec(std::chrono::nanoseconds const& d)
{
    using namespace std::chrono;
    timespec ts;
    seconds s = duration_cast<seconds>(d);
    ts.tv_sec = s.count();
    ts.tv_nsec = (d - s).count();
    return ts;
}
Сначала усеките продолжительность с точностью до наносекунд до секунды. Это усеченное значение можно поместить в ts.tv_sec. Теперь разница между исходной длительностью с точностью до наносекунд и длительностью с точностью до секунды представляет собой количество оставшихся наносекунд и присваивается ts.tv_nsec.

Преобразование в a time_pointследует той же логике, но синтаксис немного изменен, чтобы учесть тот факт, что мы работаем с time_points вместо durations:

timespec
to_timespec(date::sys_time<std::chrono::nanoseconds> const& tp)
{
    using namespace std::chrono;
    timespec ts;
    auto tps = time_point_cast<seconds>(tp);
    ts.tv_sec = tps.time_since_epoch().count();
    ts.tv_nsec = (tp - tps).count();
    return ts;
}

Получите миллисекунды с местной полуночи
( Говард Хиннант )

Прочитав вопросы и ответы о переполнении стека, я решил, что было бы неплохо показать, как решить эту проблему с помощью этой библиотеки. Поскольку вопрос не совсем ясен, о чем он спрашивает, я попытаюсь создать здесь недвусмысленную формулировку проблемы:

Найдите продолжительность времени в миллисекундах с последней полуночи по местному часовому поясу.

Чтобы мне было легче протестировать код, я собираюсь написать это в более общем виде:

    std::chrono::milliseconds
    since_local_midnight(std::chrono::system_clock::time_point t, const date::time_zone* zone);
А затем создайте перегрузку для передачи текущего времени и текущего часового пояса:

    inline
    std::chrono::milliseconds
    since_local_midnight()
    {
        return since_local_midnight(std::chrono::system_clock::now(), date::current_zone());
    }
Таким образом, я могу тестировать такие вещи, как время сразу после известных переходов на летнее время, чтобы убедиться, что мой код делает то, что я хочу.

Это займет всего несколько строк кода:

std::chrono::milliseconds
since_local_midnight(std::chrono::system_clock::time_point t, const date::time_zone* zone)
{
    using namespace date;
    using namespace std::chrono;
    zoned_time zt{zone, t};
    zt = floor<days>(zt.get_local_time());
    return floor<milliseconds>(t - zt.get_sys_time());
}
Первое, что нужно сделать, это создать файл zoned_time, который на самом деле вообще ничего не делает, кроме пары zoneи t. Это сочетание сделано главным образом для того, чтобы сделать синтаксис более приятным.

Следующий шаг — получить местное время, связанное с t. Вот что zt.get_local_time()делает. Оно будет иметь любую точность t, кроме случаев, tкогда она грубее секунд, и в этом случае местное время будет иметь точность в секунды.

Вызов floor<days> усекает местное время до точности days. Это фактически создает local_timeэквивалент местной полуночи. Присвоив это значение local_timeобратно zt, мы вообще не меняем часовой пояс zt, но меняем local_timeзначение ztна полночь (и, таким образом, sys_timeтакже меняем его).

Мы можем получить соответствующее значение с sys_timeпомощью . Это время UTC, соответствующее местной полуночи. Затем можно легко вычесть это значение из входных данных и усечь результаты до желаемой точности.ztzt.get_sys_time()t

В качестве дополнительного средства тестирования удобно написать a, since_local_midnightкоторый принимает a zoned_seconds(с zoned_timeточностью до секунд), вызывающий основную перегрузку:

inline
std::chrono::milliseconds
since_local_midnight(const date::zoned_seconds& zt)
{
    return since_local_midnight(zt.get_sys_time(), zt.get_time_zone());
}
Итак, чтобы вывести текущее время в миллисекундах с момента местной полуночи, вам просто нужно:

cout << since_local_midnight() << '\n';
Чтобы убедиться, что наша функция работает, стоит вывести несколько примеров дат. Это проще всего сделать, указав часовой пояс (я буду использовать «Америка/Нью-Йорк») и некоторую местную дату/время, когда я знаю правильный ответ:

zoned_time zt{locate_zone("America/New_York"), local_days{January/15/2016} + 3h};
cout << zt << " is " << since_local_midnight(zt) << " after midnight\n";
Это 3 часа ночи в середине зимы. Это выводит:

2016-01-15 03:00:00 EST is 10800000ms after midnight
что правильно (10800000 мс == 3 часа).

Я могу запустить тест еще раз, просто назначив новое местное время для файла zt. Ниже показано 3 часа ночи сразу после перехода на летнее время (2-е воскресенье марта):

zt = local_days{Sunday[2]/March/2016} + 3h;
cout << zt << " is " << since_local_midnight(zt) << " after midnight\n";
Это выводит:

2016-03-13 03:00:00 EDT is 7200000ms after midnight
Поскольку местное время с 2:00 до 3:00 было пропущено, с полуночи правильно выводится 2 часа.

Пример середины лета возвращает нас к 3 часам после полуночи:

zt = local_days{July/15/2016} + 3h;
cout << zt << " is " << since_local_midnight(zt) << " after midnight\n";
2016-07-15 03:00:00 EDT is 10800000ms after midnight
И, наконец, пример сразу после осеннего перехода с летнего времени обратно на стандартное дает нам 4 часа:

zt = local_days{Sunday[1]/November/2016} + 3h;
cout << zt << " is " << since_local_midnight(zt) << " after midnight\n";
2016-11-06 03:00:00 EST is 14400000ms after midnight
Эта библиотека не только упрощает написание кода для выполнения желаемых вычислений, но также упрощает написание тестового кода.


Какая у меня версия базы данных часовых поясов?
( Говард Хиннант )

Многие люди могут этого не осознавать, но мировые правила часовых поясов обновляются довольно часто . За десятилетний период [2006 – 2015] база данных часовых поясов IANA обновлялась в среднем 12,2 раза в год (чуть чаще одного раза в месяц). 2015 год был медленным: обновлялось всего 7 раз. Но 6-я версия за 2016 год вышла 5 июля, и события уже изменились, что потребует появления еще одной версии задолго до конца года.

Некоторые из этих обновлений являются исправлениями ошибок. Но большинство из них являются результатом политических изменений на местах: правительства по всему миру (в совокупности) регулярно меняют свои правила часового пояса, и часто с удивительно небольшим уведомлением (всего за 3 дня). Поскольку компьютеры все больше и больше общаются друг с другом и обмениваются временными метками, для компьютеров становится важным убедиться, что они используют одни и те же правила часового пояса, чтобы локальные временные метки туда и обратно были согласованными.

Эта библиотека может запрашивать номер версии у установленной базы данных часовых поясов IANA. Номер версии состоит std::stringиз года, за которым следует строчная буква, aобозначающая первую версию за год (еще не было более 26 версий в год, что было бы довольно абсурдно).

Вот как вы можете получить версию установленной базы данных часовых поясов IANA:

#include "date/tz.h"
#include <iostream>

int
main()
{
    cout << date::get_tzdb().version << '\n';
}
Когда я пишу это, результат:

2016f
Если удаленный API включен ( HAS_REMOTE_API == 1), вы также можете запросить номер последней версии на веб-сайте IANA с помощью:

cout << date::remote_version() << '\n';
который в настоящее время выводит 2016f.


Получение time_pointиз y/m/d h:m:sкомпонентов
Дата/время интерпретируется как UTC,time_point
Преобразование даты/времени, интерпретируемой как UTC

struct DateTime {              // hold date/time (interpreted as UTC), to be converted to time_point
    int year;
    int month;
    int day;
    int hour = 0;
    int min  = 0;
    int sec  = 0;
};
*/
//DateTime datetime{2016 /*year*/,  12 /*month*/,  24 /*day*/,     23 /* hour */,  0 /* min */,  0 /*sec*/}; 
// variable definition
// в a std::chrono::system_clock::time_pointможно сделать с помощью следующей функции:
/*
// convert date/time from UTC, to time_point
std::chrono::system_clock::time_point datetime_utc_to_timepoint(const DateTime &dt) // note: this function does NOT need the tz library and header
{
    using namespace std::chrono;
    using namespace date;

    auto ymd = year(dt.year)/dt.month/dt.day; // year_month_day type
    if (!ymd.ok()) { throw std::runtime_error("Invalid date"); }

    return                     sys_days(ymd) + hours(dt.hour) + minutes(dt.min) + seconds(dt.sec);
}
Чтобы напечатать std::chrono::system_clock::time_point, получив дату/время UTC, можно использовать следующее:

cout << "Date/Time is ";
date::operator<<(std::cout, tp) << std::endl;

// or 
using date::operator<<;
cout << "Date/Time is " << tp << std::endl;

// It will print the following
// Date/Time is 2016-12-24 23:00:00.000000000
// Вышеупомянутому нужен только "date/date.h"заголовок. ( "date/tz.h"заголовок и библиотека tz не нужны).

Дата/время (интерпретируется как происходящее из определенного часового пояса), чтобыtime_point
Если дата/время взяты из определенного часового пояса, то необходимы заголовок «date/tz.h» и библиотека tz:

//// convert date/time from tzone, to time_point
std::chrono::system_clock::time_point datetime_to_timepoint(const DateTime &dt, const date::time_zone* tzone)
{
    using namespace std::chrono;
    using namespace date;

    auto ymd = year(dt.year)/dt.month/dt.day; // year_month_day type
    if (!ymd.ok()) { throw std::runtime_error("Invalid date"); }

    return zoned_time{tzone, local_days{ymd} + hours(dt.hour) + minutes(dt.min) + seconds(dt.sec)}.get_sys_time();
}
Вышеупомянутую функцию можно вызвать следующим образом
*/
//DateTime datetime{2016 /*year*/,  12 /*month*/,  24 /*day*/,     23 /* hour */,  0 /* min */,  0 /*sec*/}; 
// variable definition
/*
auto tp = datetime_to_timepoint(datetime, date::current_zone()); // datetime from local timezone

using date::operator<<;
cout <<                                                      tp  << std::endl;
cout << date::zoned_time{"UTC",                              tp} << std::endl;
cout << date::zoned_time{date::current_zone(),               tp} << std::endl;
cout << date::zoned_time{date::locate_zone("Europe/Moscow"), tp} << std::endl;

// Will print e.g. (if you're in CET timezone...)
// 2016-12-24 22:00:00.000000000
// 2016-12-24 22:00:00.000000000 UTC
// 2016-12-24 23:00:00.000000000 CET
// 2016-12-25 01:00:00.000000000 MSK



auto tp2 = datetime_to_timepoint(datetime, date::locate_zone("America/New_York")); // datetime from New York

cout <<                                                      tp2  << std::endl;
cout << date::zoned_time{"UTC",                              tp2} << std::endl;
cout << date::zoned_time{date::current_zone(),               tp2} << std::endl;
cout << date::zoned_time{date::locate_zone("Europe/Moscow"), tp2} << std::endl;

// Will print
// 2016-12-25 04:00:00.000000000
// 2016-12-25 04:00:00.000000000 UTC
// 2016-12-25 05:00:00.000000000 CET //  e.g. (if you're in CET timezone...)
// 2016-12-25 07:00:00.000000000 MSK
*/
//datetime = DateTime{2018 /*year*/,  3 /*month*/,  25 /*day*/,     2 /* hour */,  10 /* min */,  0 /*sec*/};
/*
auto tp3 = datetime_to_timepoint(datetime, date::locate_zone("Europe/Berlin"));

// Will throw, with the following message
// terminate called after throwing an instance of 'date::nonexistent_local_time'
//   what():  2018-03-25 02:10:00 is in a gap between
// 2018-03-25 02:00:00 CET and
// 2018-03-25 03:00:00 CEST which are both equivalent to
// 2018-03-25 01:00:00 UTC
struct tmудерживая компоненты
Если у вас есть struct tmкомпоненты (в формате UTC), следующая функция устанавливает time_point:

// Ignores tm_isdst!
template <typename Clock, typename Duration>
void to_time_point(const std::tm& t,
                   std::chrono::time_point<Clock, Duration>& tp)
{
    using namespace std::chrono;
    using namespace date;
    int y = t.tm_year + 1900;
    auto ymd = year(y)/(t.tm_mon+1)/t.tm_mday; // Yields a year_month_day type
    if (!ymd.ok())
        throw std::runtime_error("Invalid date");
    tp =  sys_days(ymd) +
          hours(t.tm_hour) + minutes(t.tm_min) + seconds(t.tm_sec);
}

std::chrono::system_clock::time_point tp;
std::tm components = {...};
to_time_point(components, tp);
Получение y/m/d h:m:sкомпонентов изtime_point
(по Экорму )

Первый рецепт, приведенный ниже, основан на примере библиотеки дат из слайдов CppCon 2015 Говарда Хиннанта .

using namespace date;
auto time = std::chrono::system_clock::now();
auto daypoint = floor<days>(time);
year_month_day ymd = daypoint;   // calendar date
hh_mm_ss tod{time - daypoint}; // Yields time_of_day type

// Obtain individual components as integers
auto y   = int(ymd.year());
auto m   = unsigned(ymd.month());
auto d   = unsigned(ymd.day());
auto h   = tod.hours().count();
auto min = tod.minutes().count();
auto s   = tod.seconds().count();
В качестве альтернативы можно использовать struct tmдля хранения компонентов:

template <typename Clock, typename Duration>
std::tm to_calendar_time(std::chrono::time_point<Clock, Duration> tp)
{
    using namespace date;
    auto date = floor<days>(tp);
    auto ymd = year_month_day(date);
    auto weekday = year_month_weekday(date).weekday_indexed().weekday();
    hh_mm_ss tod{tp - date};
    days daysSinceJan1 = date - sys_days(ymd.year()/1/1);

    std::tm result{};
    result.tm_sec   = tod.seconds().count();
    result.tm_min   = tod.minutes().count();
    result.tm_hour  = tod.hours().count();
    result.tm_mday  = (ymd.day() - 0_d).count();
    result.tm_mon   = (ymd.month() - January).count();
    result.tm_year  = (ymd.year() - 1900_y).count();
    result.tm_wday  = (weekday - Sunday).count();
    result.tm_yday  = daysSinceJan1.count();
    result.tm_isdst = -1; // Information not available
    return result;
}

auto t = to_calendar_time(std::chrono::system_clock::now());
cout << t.tm_year + 1900 << "-" << t.tm_mon + 1 << "-" << t.tm_mday << " "
          << t.tm_hour << ":" << t.tm_min << ":" << t.tm_sec << "\n";

Нормализация, y/m/dкогда это!ok()
( Говард Хиннант )

Следующая функция «нормализует» a year_month_day, так же, как mktimeнормализует a tm. Эта функция не является частью библиотеки, но предлагается в качестве примера, если вам понадобятся такие вещи:

date::year_month_day
normalize(date::year_month_day ymd)
{
    using namespace date;
    ymd += months{0};
    ymd = sys_days{ymd};
    return ymd;
}
Первая строка просто добавляет 0 месяцев к ymd. Если ymd.month()равно 0, это вычтет единицу yearи установит месяц на Dec. Если ymd.month()больше 12, год будет увеличен столько раз, сколько необходимо, а месяц будет переведен в правильный диапазон. Эта операция ничего не сделает, если ymd.month().ok()она уже истинна.

Вторая строка «нормализует» поле дня. Вторая строка требует, чтобы поле месяца уже было нормализовано. Например 2015_y/December/32станет 2016_y/January/1. Если поле дня уже находится в пределах диапазона, изменений не произойдет.

Преобразование из year_month_dayв sys_daysвызывает этот алгоритм:

http://howardhinnant.github.io/date_algorithms.html#days_from_civil

И преобразование обратно sys_daysв year_month_dayвызывает этот алгоритм:

http://howardhinnant.github.io/date_algorithms.html#civil_from_days

Пример:

int
main()
{
    using namespace date;
    auto ymd = 2015_y/55/250;
    cout << ymd << (ymd.ok() ? "\n" : " invalid date\n");
    ymd += months{0};
    cout << ymd << (ymd.ok() ? "\n" : " invalid date\n");
    ymd = sys_days{ymd};
    cout << ymd << (ymd.ok() ? "\n" : " invalid date\n");
}
Выходы:

2015-55-250 invalid date
2019-07-250 invalid date
2020-03-06

Когда это нормально !ok()?
( Говард Хиннант )

Эта библиотека позволяет датам незаметно переходить в состояние !ok(). Почему не !ok()утверждает и не бросает? Когда это будет нормально !ok()?

Рассмотрим эту проблему:

Я хочу найти все даты за какой-то год y, которые являются 5-й пятницей месяца (потому что это день вечеринки или что-то в этом роде). Вот очень эффективная функция, которая собирает все пятые пятницы года:

std::pair<std::array<date::year_month_day, 5>, unsigned>
fifth_friday(date::year y)
{
    using namespace date;
    constexpr auto nan = 0_y/0/0;
    std::array<year_month_day, 5> dates{nan, nan, nan, nan, nan};
    unsigned n = 0;
    for (auto m = January; true; ++m)
    {
        auto d = Friday[5]/m/y;
        if (d.ok())
        {
            dates[n] = year_month_day{d};
            ++n;
        }
        if (m == December)
            break;
    }
    return {dates, n};
}
Оказывается, инвариантом является то, что в году будет либо 4, либо 5 месяцев с 5 пятницами. Таким образом, мы можем эффективно возвращать результаты в виде pair<array<year_month_day, 5>, unsigned>, где второй член pairвсегда будет равен 4 или 5.

Первая задача — просто инициализировать arrayгруппу year_month_days. Я произвольно выбрал 0_y/0/0хорошее значение инициализации. Что мне нравится в этом значении? Одна из вещей, которые мне нравятся, это то, что это так !ok()! Если я случайно получу доступ .first[4]к When .second == 4, дополнительной безопасностью будет то, что результатом year_month_dayбудет !ok(). Поэтому возможность создавать эти !ok()значения без утверждений или исключений важна именно по этой причине (например, nan). Цена? Ничего. Это константы времени компиляции.

Далее я перебираю каждый месяц года y. Первое, что нужно сделать, это построить 5-ю пятницу для этой пары месяц/год:

auto d = Friday[5]/m/y;
Поскольку не в каждом месяце есть пятая пятница, это может привести к тому, что дата окажется недействительной. Но в этой функции правильным ответом на создание недопустимой даты является не утверждение и не исключение. Правильный ответ — игнорировать дату и перейти к следующему месяцу. Если это действительная дата, то она переходит к результату.

Эту функцию можно реализовать следующим образом:

int
main()
{
    using namespace std::chrono;
    using namespace date;
    auto current_year = year_month_day{floor<days>(system_clock::now())}.year();
    auto dates = fifth_friday(current_year);
    cout << "Fifth Friday dates for " << current_year << " are:\n";
    for (auto i = 0u; i < dates.second; ++i)
        cout << dates.first[i] << '\n';
}
Переменная current_yearинициализируется текущим годом в часовом поясе UTC (достаточно близком для работы правительства — используйте «tz.h», если вам нужно сделать его более точным в соответствии с вашим регионом). Тогда легко ввести current_yearи fifth_fridayперебрать результаты. Это просто вывод для меня:

Fifth Friday dates for 2016 are:
2016-01-29
2016-04-29
2016-07-29
2016-09-30
2016-12-30
В следующем году выйдет:

Fifth Friday dates for 2017 are:
2017-03-31
2017-06-30
2017-09-29
2017-12-29
Во время выполнения этой программы было вычислено много недопустимых дат. И ни один из них не содержал ошибок.


Как найти следующий понедельник (или четверг)
( Говард Хиннант )

Учитывая year_month_dayи день недели, как мне эффективно увеличивать его, year_month_dayпока он не станет желаемым днем ​​недели?

Это очень легко. Но сначала надо определиться: Если year_month_dayнужный уже есть weekday, хотите ли вы вернуть оригинал year_month_dayили добавить неделю? Не существует единственного правильного ответа. Здесь мы сделаем это в обе стороны. Сначала я покажу, как сохранить оригинал. Затем можно легко модифицировать, чтобы показать, как перейти к следующей неделе.

date::year_month_day
next_weekday(date::year_month_day ymd, date::weekday target)
{
    using namespace date;
    sys_days sd = ymd;
    return sd + (target - weekday{sd});
}
Первое, что нужно сделать, это преобразовать year_month_dayв файл sys_days. Это сделано потому, что очень эффективно найти день недели sys_days(количество дней) и выполнить арифметику, ориентированную на день, в этой структуре данных.

Дальше узнаем, сколько дней нам нужно прибавить, sdчтобы добраться до желаемого weekday. Это просто целевой день недели минус текущий день недели. weekdayвычитание является беззнаковым по модулю 7. То есть оно всегда возвращает значение от 0 до 6. Monday - Sundayпоскольку всегда на 1 день days{1}позже . всегда через 6 дней после .MondaySundaySunday - Mondaydays{6}SundayMonday

Выше if target == weekday{sd}, то мы вообще не добавляем дни, потому что ymdнужный уже есть weekday. В противном случае мы добавляем до 6 дней. Затем возвращаемый результат неявно преобразуется обратно в year_month_day.

Если мы хотим добавить неделю, когда входные данные уже соответствуют целевому дню недели, тогда вы просто добавляете день до sdалгоритма:

date::year_month_day
next_weekday(date::year_month_day ymd, date::weekday target)
{
    using namespace date;
    sys_days sd = ymd;
    sd += days{1};
    return sd + (target - weekday{sd});
}
Обратное аналогично:

date::year_month_day
prev_weekday(date::year_month_day ymd, date::weekday target)
{
    using namespace date;
    sys_days sd = ymd;
    sd -= days{1};
    return sd - (weekday{sd} - target);
}
Но теперь мы вычитаем дни недели, чтобы узнать, сколько дней нужно вычесть из sd. Удалите предварительное уменьшение на день, если вы хотите, чтобы эта функция возвращала входные данные, когда входные данные уже соответствуют целевому дню недели.


Конвертация из {год, микросекунды} в CCSDS
( Говард Хиннант )

CCSDS ( http://public.ccsds.org/default.aspx ) имеет структуру данных, которая выглядит следующим образом:

struct CCSDS
{
    std::uint16_t days;
    std::uint32_t ms;
    std::uint16_t us;
};
где days— количество дней, прошедших с 1 января 1958 года, ms— количество миллисекунд текущего дня, us— количество микросекунд текущей миллисекунды.

Возникла необходимость преобразовать структуру данных {год, микросекунда} в приведенную выше структуру данных CCSDS, где второй компонент — это количество микросекунд с начала года. Кроме того, отсчет микросекунд включает високосные секунды.

Вот функция для выполнения этого преобразования:

CCSDS
to_CCSDS(date::year y, std::chrono::microseconds us)
{
    using namespace date;
    using namespace std::chrono;
    auto utc = clock_cast<utc_clock>(sys_days{y/January/1}) + us;
    auto sys = clock_cast<system_clock>(utc);
    auto dp = floor<days>(sys);
    auto d = dp - sys_days{1958_y/January/1};
    us = utc - clock_cast<utc_clock>(dp);
    auto ms = duration_cast<milliseconds>(us);
    us -= ms;
    return {static_cast<std::uint16_t>(d.count()),
            static_cast<std::uint32_t>(ms.count()),
            static_cast<std::uint16_t>(us.count())};
}
Переменная utcсодержит «год + нас» как момент времени с точностью до микросекунд. В этот момент времени отсчитываются микросекунды, включая дополнительные секунды, начиная с 1970-01-01 00:00:00 UTC.

Следующий шаг — определить, когда начался день, связанный с utc. Для этого необходимо преобразовать utcобратно во «время Unix», а затем усечь этот момент времени до точности days, в результате чего получится переменная dp. dpэто количество дней с 1 января 1970 года. Поскольку требуемая эпоха — 1958-01-01, это учитывается при создании dпервого значения, необходимого в возвращаемом типе.

Теперь необходимо вычислить количество микросекунд с начала дня. Начало дня dpпреобразуется обратно в систему учета дополнительных секунд и вычитается из микросекундной точки времени: utc. Переменная usиспользуется повторно для хранения «микросекунд с полуночи». Теперь нужно просто вычислить, чтобы разделить это время на миллисекунды с полуночи и микросекунды с последней миллисекунды.


Разница в месяцах между двумя датами
( Говард Хиннант )

Недавно я наткнулся на этот вопрос о переполнении стека:

http://stackoverflow.com/q/19290421/576911

И я решил посмотреть, смогу ли я ответить на него (здесь), используя эту библиотеку. Возникает вопрос: как я могу получить количество месяцев между двумя датами? И он дает два примера дат:

auto d1 = 1_d/October/2013;
auto d2 = 30_d/October/2016;
(Я преобразовал синтаксис в синтаксис этой библиотеки).

Вопрос не совсем ясен, поскольку «месяцы» — не очень точная единица. Хотим ли мы количество «полных месяцев»? Или, может быть, нам следует округлить до ближайшего целого числа месяцев? Или нам нужно представление месяцев с плавающей запятой, которое может отображать дробные месяцы?

Это все разумные возможности, и вы можете вычислить все эти вещи с помощью этой библиотеки. Иногда самая сложная часть вопроса — это его уточнение до тех пор, пока вы не поймете, о чем на самом деле спрашивают.

Если мы хотим просто игнорировать поле дня в этих датах, а затем вычислить количество месяцев, это легко сделать следующим образом:

cout << (d2.year()/d2.month() - d1.year()/d1.month()).count() << '\n';
Это создает два year_monthобъекта и вычитает их. Это дает a std::chrono::duration, представляющее целое число месяцев со знаком. Результат:

36
Чтобы учесть влияние дневных полей, лучше всего преобразовать d1и d2в sys_dayss:

auto dp1 = sys_days(d1);
auto dp2 = sys_days(d2);
Теперь мы могли бы (например) вычесть две sys_dayss и округлить результат до ближайшего целого месяца:

cout << round<months>(dp2-dp1).count() << '\n';
Это выводит:

37
Или мы могли бы создать новый std::chrono::durationтип на основе float, но с периодом в несколько месяцев, и преобразовать в него разницу:

cout << duration<float, months::period>(dp2-dp1).count() << '\n';
Это выводит:

36.9617
Все это разумные ответы на вопрос, и все они легко вычислимы с помощью этой библиотеки.


Работа с недельным годом ISO
( Говард Хиннант )

Дата недели ISO — это международно признанная система подсчета недель в году. По сути, это отдельная календарная система, хотя она тесно связана с григорианским календарем. Вместо указания года, месяца и дня каждый день указывается годом, номером недели и днем ​​недели. Например, 2015-W51-Sat — это полностью определенная дата. Такую дату можно сформировать, используя <iso_week.h>заголовок следующим образом:

using namespace iso_week::literals;
auto iso_date = 2015_y/51/sat;
Как и <date.h>, вы можете указать дату недели по стандарту ISO в любом из трех порядков: г/вн/вд, вд/вн/г, вн/вд/г (с прямым порядком байтов, с прямым порядком байтов, смешанным (американским) порядком байтов).

Также, как и <date.h>, вы можете неявно преобразовать дату недели ISO в sys_days, и наоборот. Для удобства псевдоним date:: sys_days существует как iso_week:: sys_days :

iso_week::sys_days dp = iso_date;
Напомним, что sys_daysэто всего лишь псевдоним типа для файла std::chrono::time_point<std::chrono::system_clock, days>. Таким образом, дата недели ISO ( iso_week:year_weeknum_weekday) сразу же совместима со всей <chrono>библиотекой, как и date::year_month_dayесть.

auto now = std::chrono::system_clock::now();
auto dp = date::floor<iso_week::days>(now);
iso_week::year_weeknum_weekday iso_date = dp;
date::hh_mm_ss time{now-dp};
cout << iso_date << ' ' << time << '\n';
Что просто выводится для меня:

2016-W11-Sat 03:07:02.460737
А поскольку iso_week:year_weeknum_weekdayон неявно конвертируется в и из sys_days, это делает его немедленно (и явно) конвертируемым в любую другую календарную систему, которая неявно конвертируется в и из sys_days:

auto civil_date = date::year_month_day{iso_date};
cout << civil_date << ' ' << time << '\n';
который выводит:

2016-03-19 03:07:02.460737
И вот оно: sys_daysРозеттский камень для перевода любого календаря в любой другой календарь. Просто преобразуйте свой календарь в и из sys_days, и вы получите совместимость с любым другим календарем, который это делает.

using namespace date::literals;
auto today = 2016_y/mar/19;
cout << "civil   : " << today << '\n';
cout << "Julian  : " << julian::year_month_day{today} << '\n';
cout << "Coptic  : " << coptic::year_month_day{today} << '\n';
cout << "iso_week: " << iso_week::year_weeknum_weekday{today} << '\n';
Выход:

civil   : 2016-03-19
Julian  : 2016-03-06
Coptic  : 1732-07-10
iso_week: 2016-W11-Sat
Это своего рода тизер, потому что на момент написания этой статьи юлианский и коптский календари еще не доступны публично. Программное обеспечение существует, но еще не полностью протестировано. Но что еще более важно, просто следуйте рецепту <iso_week.h>для вашего любимого календаря (конвертируйте в и из sys_days), и ваш календарь теперь станет частью клуба! Разместите свой календарь, чтобы мы все могли им воспользоваться!


День Рождения 2G
( Говард Хиннант )

Этот пример демонстрирует как простую арифметику дат, так и то, как обрабатывать разрывы в часовом поясе. Дэйв родился в часовом поясе «Америка/Лос-Анджелес» в 10:03 24 апреля 1954 года. Когда ему исполнится 2 000 000 000 секунд в том же часовом поясе?

#include <chrono>
#include <iostream>
#include "date/date.h"
#include "date/tz.h"

int
main()
{
    using namespace std::chrono_literals;
    using namespace date;
    //  Dave was born April 24, 1954. 10:03 AM pst
    //  Want to know when he is 2 Gigaseconds old
    zoned_time birthday{"America/Los_Angeles", local_days{April/24/1954} + 10h + 3min};
    cout << "born        : " << birthday << '\n';
    birthday = birthday.get_sys_time() + 2'000'000'000s;
    cout << "2Gs birthday: " << birthday << '\n';
}
Сначала создается местное время, а затем сопоставляется с часовым поясом «Америка/Лос_Анжелес», используя zoned_time. Затем добавьте 2G к sys_time(не local_time) из birthday. Это выводит:

born        : 1954-04-24 10:03:00 PST
2Gs birthday: 2017-09-08 14:36:20 PDT
Обратите внимание, что без правильной обработки часового пояса этот результат будет отличаться на час (2017-09-08 13:36:20), поскольку дата рождения выпадает на PST, а дата празднования — на PDT.

Также обратите внимание, что эта библиотека правильно обрабатывает изменения в правилах часовых поясов на протяжении десятилетий. За последние десятилетия несколько дней 24 апреля попадали в летнее время, причем все с 1987 года.

А как насчет високосных секунд?
Это становится немного сложнее.

Атомное ведение времени началось экспериментально в 1955 году, примерно через год после рождения Дэйва. В 1958 году был запущен первый атомный международный стандарт времени: TAI. А затем между 1958 и 1972 годами были добавлены 10 «недокументированных» дополнительных секунд, чтобы сформировать то, что мы теперь знаем как UTC. Я говорю «недокументированные», потому что они не были вставлены за доли секунды, и я не знаю, сколько и когда было вставлено. Я знаю только, что общая сумма за этот период времени составила ровно 10 секунд.

Предполагая, что дата рождения точно синхронизирована с TAI (смещение по часовому поясу), тогда мы можем сформировать день рождения как tai_time:

auto zone = locate_zone("America/Los_Angeles");
auto birthday = to_tai_time(zoned_time{zone,
                local_days{April/24/1954} + 10h + 3min - 10s}.get_sys_time());
Нам приходится вычитать 10 секунд вручную, потому что мы хотим, чтобы день рождения был 1954-04-24 18:03:00 TAI, и без этого вычитания 10 секунд мы смоделируем UTC до 1954 года вместо моделирования TAI в 1954 году. Затем мы добавляем 2G tai_timeи конвертируем этот результат обратно в sys_time, а затем в a zoned_time:

zoned_time Day2Gs{zone, to_sys_time(birthday + 2'000'000'000s)};
Теперь вывод:

born        : 1954-04-24 18:03:00 TAI 
2Gs birthday: 2017-09-08 14:35:43 PDT
что на 37 секунд раньше, чем мы сообщали, без учета дополнительных секунд. Обычно вычисление времени с такой точностью (с учетом дополнительных секунд) в будущем было бы очень рискованным предложением, поскольку никогда не знаешь, когда в будущем будет добавлена ​​дополнительная секунда. Однако, когда я пишу это (24 октября 2016 г.), база данных IANA знает о високосной секунде, которая должна быть вставлена ​​1 января 2017 г., и маловероятно (хотя и не невозможно), что еще одна дополнительная секунда будет вставлена ​​до 2017 г. -09-08. Поэтому у меня есть высокая уверенность в том, что было учтено правильное количество дополнительных секунд. К 1 февраля 2017 г. мы должны знать наверняка, верны ли эти вычисления.


Вычисление порядковых дат
( Роланд Бок )

Порядковая дата состоит из года и дня года (1 января — день 1, 31 декабря — день 365 или день 366). Год можно получить непосредственно из Year_month_day. А рассчитать день удивительно легко. В приведенном ниже коде мы учитываем тот факт, что год_месяц_день может работать с недопустимыми датами, такими как 0 января:

int main()
{
   using namespace date;
   
   const auto time = std::chrono::system_clock::now();
   const auto daypoint = floor<days>(time);
   const auto ymd = year_month_day{daypoint};
   
   // calculating the year and the day of the year
   const auto year = ymd.year();
   const auto year_day = daypoint - sys_days{year/January/0};
   
   cout << year << '-' << std::setfill('0') << std::setw(3) << year_day.count() << std::endl;
   
   // inverse calculation and check
   assert(ymd == year_month_day{sys_days{year/January/0} + year_day});
}

Арифметика местного времени
( Говард Хиннант )

Общепринято считать, что выполнение арифметических операций с временными точками в «местном времени» подвержено ошибкам из-за изменений смещения UTC, таких как переход на летнее время. Например, при переходе на летнее время день длится всего 23 часа вместо обычных 24.

Однако эту библиотеку можно очень легко использовать для правильного выполнения таких вычислений. Ниже приведен код, который выводит 9 утра в течение нескольких дней в «Америке/Нью-Йорке», как непосредственно перед переходом на летнее время, так и сразу после:

#include "date/tz.h"
#include <iostream>

int
main()
{
    using namespace std::chrono;
    using namespace date;
    zoned_time base{"America/New_York", local_days{March/11/2016} + 9h};
    for (int i = 0; i < 4; ++i)
    {
        cout << format("%F %T %z", base) << '\n';
        base = base.get_local_time() + days{1};
    }
}
Этот код выводит:

2016-03-11 09:00:00 -0500
2016-03-12 09:00:00 -0500
2016-03-13 09:00:00 -0400
2016-03-14 09:00:00 -0400
Обратите внимание, что код просто получает текущее местное время, добавляет к нему 1 день и присваивает это местное время обратно в файл zoned_time. Пока вновь вычисленное местное время не является двусмысленным или несуществующим, этот код просто работает. И если вновь вычисленное местное время неоднозначно или отсутствует, будет выдано исключение.


Преобразование аббревиатуры часового пояса в часовой пояс
( Говард Хиннант )

Общеизвестно, что преобразование аббревиатуры часового пояса в часовой пояс в целом является сложной задачей. Часовые пояса не только постоянно меняют используемые сокращения, но и используемые сокращения не уникальны. В любой момент времени несколько часовых поясов могут использовать одну и ту же аббревиатуру. И не все эти часовые пояса всегда будут иметь одинаковое смещение UTC.

Во всяком случае, эта операция может быть чрезвычайно запутанной и подверженной ошибкам. Следует стремиться не оказаться в ситуации необходимости выполнения этой операции. Однако, если вы вынуждены это сделать, эта библиотека предоставляет решения, которые вы можете запрограммировать самостоятельно, и которые превосходят все другие решения.

Для иллюстрации в этом примере разрабатываются find_by_abbrevфункции, которые возвращают объект std::vector<date::zoned_time<some-duration>>. Затем клиент может перебирать этот список, проверять каждый zoned_timeи выводить всю интересующую его информацию. Предположительно часть этой информации можно использовать для выбора уникального часового пояса.

Для начала предположим, что у нас есть дата/время: 2016-04-03 03:15и аббревиатура «CST». Мы хотим найти все возможные часовые пояса, в которых может использоваться «CST» в 2016-04-03 03:15.

Первая проблема: 2016-04-03 03:15время соответствует часовому поясу или UTC? Ответ на этот вопрос может повлиять на результаты, поэтому нам нужно четко понимать, о чем мы просим. Оба вопроса действительны: в каких часовых поясах в данный момент используется «CST»? В каких часовых поясах есть местный житель time_point t, использующий «CST»?

Итак, нам нужны два find_by_abbrev: один, который принимает a sys_time, и другой, который принимает a local_time. Давайте sys_timeсначала сделаем:

#include "date/tz.h"
#include <string>
#include <iostream>
#include <vector>

template <class Duration>
std::vector<date::zoned_time<std::common_type_t<Duration, std::chrono::seconds>>>
find_by_abbrev(date::sys_time<Duration> tp, const std::string& abbrev)
{
    using namespace std::chrono;
    using namespace date;
    std::vector<zoned_time<std::common_type_t<Duration, std::chrono::seconds>>> results;
    auto& db = get_tzdb();
    for (auto& z : db.zones)
    {
        if (z.get_info(tp).abbrev == abbrev)
            results.emplace_back(&z, tp);
    }
    return results;
}

int
main()
{
    using namespace std::chrono;
    using namespace date;
    auto now = sys_days{2016_y/4/3} + 3h + 15min;
    auto v = find_by_abbrev(now, "CST");
    for (auto const& zt : v)
        cout << zt << " " << zt.get_time_zone()->name() << '\n';
}
Функция find_by_abbrevна удивление проста: перебирает все time_zones в базе данных, получает sys_infoдля каждого из них tp, и если у него sys_infoесть аббревиатура, равная abbrev, создайте zoned_timeдля этого time_zoneи tpдобавьте его к vector. Самая сложная часть — определить точность продолжительности, zoned_timeкоторая будет мельче secondsи Duration. Заведение std::common_typeэто понимает за нас.

Эта программа выводит:

2016-04-02 21:15:00 CST America/Bahia_Banderas
2016-04-02 21:15:00 CST America/Belize
2016-04-02 21:15:00 CST America/Costa_Rica
2016-04-02 21:15:00 CST America/El_Salvador
2016-04-02 21:15:00 CST America/Guatemala
2016-04-02 21:15:00 CST America/Managua
2016-04-02 21:15:00 CST America/Merida
2016-04-02 21:15:00 CST America/Mexico_City
2016-04-02 21:15:00 CST America/Monterrey
2016-04-02 21:15:00 CST America/Regina
2016-04-02 21:15:00 CST America/Swift_Current
2016-04-02 21:15:00 CST America/Tegucigalpa
2016-04-03 11:15:00 CST Asia/Macau
2016-04-03 11:15:00 CST Asia/Shanghai
2016-04-03 11:15:00 CST Asia/Taipei
Это все время и места с использованием «CST» в 2016-04-03 03:15 UTC. Обратите внимание, что некоторые места находятся на противоположных сторонах планеты.

Теперь самая интересная часть этой проблемы: что произойдет, если мы говорим о 2016-04-03 03:15 местном времени? Примерно в это время года многие регионы переходят на летнее время. И как нам это вычислить?

Программа-драйвер идентична показанной выше, за исключением того, что вместо:

auto now = sys_days{2016_y/4/3} + 3h + 15min;
у нас есть:

auto now = local_days{2016_y/4/3} + 3h + 15min;
Вместо a принимает find_by_abbreva , и логика немного сложнее:local_time<Duration>sys_time<Duration>

template <class Duration>
std::vector<date::zoned_time<std::common_type_t<Duration, std::chrono::seconds>>>
find_by_abbrev(date::local_time<Duration> tp, const std::string& abbrev)
{
    using namespace std::chrono;
    using namespace date;
    std::vector<zoned_time<std::common_type_t<Duration, std::chrono::seconds>>> results;
    auto& db = get_tzdb();
    for (auto& z : db.zones)
    {
        auto i = z.get_info(tp);
        switch (i.result)
        {
        case local_info::unique:
            if (i.first.abbrev == abbrev)
                results.emplace_back(&z, tp);
            break;
        case local_info::ambiguous:
            if (i.first.abbrev == abbrev)
                results.emplace_back(&z, tp, choose::earliest);
            else if (i.second.abbrev == abbrev)
                results.emplace_back(&z, tp, choose::latest);
            break;
        default:
            break;
        }
    }
    return results;
}
A local_timeможет иметь или не иметь уникальное сопоставление с UTC. Оно может быть уникальным, может быть неоднозначным или вообще не существовать. Эта функция определяет, является ли сопоставление уникальным, неоднозначным или несуществующим i.result. Если он уникален, логика такая же, как и find_by_abbrevдля sys_timeпоиска. Обратите внимание, что он zoned_timeзнает разницу между a sys_timeи a local_timeи всегда поступает правильно.

Если отображение неоднозначно, то существует два потенциальных отображения из {local time, time zone abbrev}в UTC. Если первое соответствует аббревиатуре, то выбирается первое, в противном случае, если второе соответствует аббревиатуре, выбирается второе. В противном случае ни один из них не выбран.

Если сопоставление не существует, оно игнорируется.

Эта программа выводит:

2016-04-03 03:15:00 CST America/Belize
2016-04-03 03:15:00 CST America/Costa_Rica
2016-04-03 03:15:00 CST America/El_Salvador
2016-04-03 03:15:00 CST America/Guatemala
2016-04-03 03:15:00 CST America/Managua
2016-04-03 03:15:00 CST America/Regina
2016-04-03 03:15:00 CST America/Swift_Current
2016-04-03 03:15:00 CST America/Tegucigalpa
2016-04-03 03:15:00 CST Asia/Macau
2016-04-03 03:15:00 CST Asia/Shanghai
2016-04-03 03:15:00 CST Asia/Taipei
Обратите внимание, что все местное время сейчас 03:15:00. Также обратите внимание, что этот список является подмножеством предыдущего списка, поскольку к этому времени некоторые из предыдущих часовых поясов уже перешли на другое сокращение.

Выбор между этими часовыми поясами выходит за рамки этой библиотеки. Возможно, вы априори знаете, что имеете дело с часовыми поясами Азии. Возможно, вы априори знаете, что имеете дело с часовым поясом США. Независимо от логики, специфичной для вашего приложения, эти функции дают вам инструменты, позволяющие сузить поиск до возможных часовых поясов.


Найдите все случаи, когда летняя смена длится не 1 час.
( Говард Хиннант )

Допустим, вы хотите найти по всему земному шару и во всем мире часовые пояса, когда переход на летнее время не составляет 1 часа. Звучит странно? Возможно, но этот код научит вас, как эффективно перебирать все переходы часовых поясов и проверять их характеристики. Таким образом, вы можете использовать этот код для всех видов поиска по часовым поясам.

#include "date/tz.h"
#include <iostream>

int
main()
{
    using namespace date;
    using namespace std::chrono_literals;
    auto& db = get_tzdb();
    for (auto const& z : db.zones)
    {
        auto begin = sys_days{January/1/year::min()} + 0s;
        auto end   = sys_days{January/1/2035} + 0s;
        do
        {
            auto info = z.get_info(begin);
            if (info.save != 0h && info.save != 1h)
            {
                cout << z.name() << " has a daylight savings offset of "
                          << info.save.count() << "min from " << info.begin
                          << " UTC to " << info.end << " UTC with the abbreviation "
                          << info.abbrev << '\n';
            }
            begin = info.end;
        } while (begin < end);
    }
}
Сначала вы получаете ссылку на базу данных tz, а затем перебираете каждую зону в базе данных. Для каждой зоны установите диапазон временных точек для поиска. В этом примере я начинаю поиск как можно раньше и смотрю вперед до 2035 года.

Начиная с начала времени, получите sys_infoдля этого UTC time_point. Ан sys_infoвыглядит так:

struct sys_info
{
    second_point         begin;
    second_point         end;
    std::chrono::seconds offset;
    std::chrono::minutes save;
    std::string          abbrev;
};
Каждый переход часового пояса происходит в begin(UTC). Общее смещение от UTC для этого часового пояса и периода составляет offset. Это offsetбудет действовать до end(UTC). Разница между «нормальным» этого периода offsetи offsetэтим save. Сокращение часового пояса этого периода — abbrev.

В этом примере мы ищем те периоды, когда значение saveне равно ни 0 минутам, ни 60 минутам. Когда мы найдем такой, просто распечатайте его.

Чтобы увеличить цикл, установите локальную переменную beginна info.endи найдите новую Info.

На самом деле удивительно просто искать по земному шару и во всем мире интересные хронологические события, связанные со сменой часовых поясов.

Пример вывода этой программы:

Africa/Accra has a daylight savings offset of 20min from 1920-09-01 00:00:00 UTC to 1920-12-30 23:40:00 UTC with the abbreviation GHST
...

Сколько часовых поясов используют летнее время?
( Говард Хиннант )

Вы когда-нибудь задумывались, как меняется с течением времени глобальное использование летнего времени? Вот один из способов узнать:

#include "date/tz.h"
#include <iostream>

int
main()
{
    using namespace date;
    using namespace std::chrono_literals;
    auto& db = get_tzdb();
    cout << db.version << '\n';
    for (auto y = 1850_y; y < 2020_y; ++y)
    {
        auto use_daylight = 0;
        auto total = 0;
        for (auto& z : db.zones)
        {
            ++total;
            auto info1 = z.get_info(sys_days{y/January/15});
            auto info2 = z.get_info(sys_days{y/July/15});
            if (info1.save != 0min || info2.save != 0min)
                ++use_daylight;
        }
        cout << y << " : " << use_daylight << '/' << total
                  << " = " << static_cast<float>(use_daylight)/total << '\n';
    }
}
Этот код циклически обрабатывает широкий диапазон лет, а затем для каждого года проходит по всем часовым поясам в базе данных и для каждого часового пояса определяет, происходит ли переключение между стандартным и летним временем для этого года. Обнаружение переключения довольно грубое, но вы можете сделать это обнаружение настолько сложным, насколько захотите. В настоящее время он выбирает две даты с интервалом в 6 месяцев, которые вряд ли будут использовать стандартное время, если в этом году в зоне используется летнее время. Для каждой из этих двух дат sys_info.saveэлемент проверяется. В любом случае save != 0minв этом году используется летнее время.

Результаты этой программы показаны ниже (код построения графика не является частью приведенной выше программы).

График использования летнего времени по годам


Какова разница в эпохах между временем Unix и временем GPS?
( Говард Хиннант )

Этот вопрос интересен тем, что имеет более одного ответа. И причина, по которой у него более одного ответа, заключается в том, что существует более одной меры. Вы можете винить во всем високосные секунды. Но, надеюсь, эта библиотека поможет вам прояснить ситуацию.

Unix-время — это то, что sys_timeизмеряет. Это количество не високосных секунд с 1 января 1970 г. Эта мера полезна, поскольку она обеспечивает эффективное средство преобразования между типами полей, такими как {year, month, day, hours, minutes, seconds}и {count_of_seconds}. Это то, что измеряется std::chrono::system_clock, gettimeofday, std::timeи т. д. К сожалению, это решение не лишено проблем. Один из них заключается в том, что не существует времени Unix , которое представляет собой вставленные дополнительные секунды utc_time(и, следовательно, даже несколько секунд в tai_timeи gps_time).

Все это означает, что вычитание двух sys_timeточек времени может дать другой результат, чем вычитание двух точек времени типа utc_time, tai_timeили gps_time, и этот пример подчеркивает эту разницу.

Прежде всего нам нужно узнать gps_timeэпоху и sys_timeэпоху. Нам даже не нужно знать даты этих эпох. Мы можем просто использовать 0:

auto gps_epoch  = gps_seconds{0s};  // 1980-01-06 00:00:00 UTC
auto unix_epoch = sys_seconds{0s};  // 1970-01-01 00:00:00 UTC
Это оба std::chrono::time_points, но если мы попытаемся их вычесть, то получим ошибку времени компиляции, поскольку они относятся к разным часам. Поэтому, чтобы вычесть их, мы должны преобразовать одно в другое перед вычитанием. Вот один из способов сделать это:

cout << (gps_epoch - to_gps_time(unix_epoch)).count() << "s\n";
Это преобразует the sys_timeв a gps_time, выполняет вычитание и выводит:

315964809s
В России gps_timeмежду этими двумя эпохами проходит 315 964 809 секунд.

Вот еще один способ:

cout << (to_sys_time(gps_epoch) - unix_epoch).count() << "s\n";
который выводит:

315964800s
В России sys_timeмежду этими двумя эпохами проходит 315 964 800 секунд.

Почему разница в 9 баллов? Потому что между этими двумя эпохами было вставлено 9 дополнительных секунд, и sys_timeэти 9 секунд не учитываются gps_time.

Если бы вы провели тот же эксперимент, но с использованием utc_timeи gps_time, обе меры привели бы к результату, 315964809sпоскольку обе учитывают дополнительные секунды.

Подождите секундочку, не gps_timeигнорирует високосные секунды?! Да, но это происходит путем перевода перехода на гражданский календарь вперед на одну секунду. Таким образом, он не игнорирует физическую секунду. Он игнорирует время суток. sys_timeс другой стороны, в первую очередь игнорируется существование физической секунды, таким образом сохраняя соответствие гражданскому календарю таким же, как UTC.

Вы можете открыть для себя множество интересных тонкостей, играя с sys_time, utc_time, tai_timeи gps_time.


Как конвертировать в/из TDate и TDateTime C++ Builder
( Говард Хиннант )

Если вы используете TDateTimeC ++ Builder, вам иногда могут понадобиться функции, которых нет в этой библиотеке, но есть в этой. В этой статье объясняется, как выполнять преобразование между этими двумя библиотеками, чтобы вы могли легко использовать функциональные возможности обеих.

Два важных факта:

Эпоха TDateTime— 30 декабря 1899 г., 00:00:00 UTC.
Эпоха std::chrono::system_clock(не указана, но де-факто): 1970-01-01 00:00:00 UTC.
TDateTimeхранит double. Неотъемлемой частью этого двойного счета является отсчет дней с эпохи. Дробная часть хранит доли суток, но в странной кодировке времени, предшествовавшего этой эпохе:

При работе с отрицательными значениями System::TDateTime вычисления должны обрабатывать часть времени отдельно. Дробная часть отражает долю 24-часовых суток без учета знака значения System::TDateTime. Например, 6:00 утра 29 декабря 1899 года равно –1,25, а не –1 + 0,25, что было бы –0,75. Значения System::TDateTime между –1 и 0 отсутствуют.

Мы обеспечим два двунаправленных преобразования:

TDate     <---> sys_days
TDateTime <---> sys_time<D>
Преобразования TDate/ sys_daysне должны беспокоиться о проблеме дробных дней для отрицательных TDateTimeзначений, поэтому они проще и эффективнее:

date::sys_days
to_sys_days(System::TDate td)
{
    using namespace date;
    return sys_days(days{static_cast<int>(td)} -
        (sys_days{1970_y/January/1} - sys_days{1899_y/December/30}));
}
Единственное, что здесь нужно сделать, — это извлечь целое значение из TDate, преобразовать его в sys_daysи вычесть разницу между двумя эпохами. Обратное преобразование выполняется так же просто:

System::TDate
to_TDate(date::sys_days sd)
{
    using namespace date;
    return System::TDate(static_cast<int>((sd.time_since_epoch() +
        (sys_days{1970_y/January/1} - sys_days{1899_y/December/30})).count()));
}
Их можно тренировать следующим образом:

int
main()
{
    using namespace date;
    using namespace System;
    cout << to_sys_days(TDate{0}) << '\n';
    cout << to_sys_days(TDate{2}) << '\n';
    cout << to_sys_days(TDate{-1}) << '\n';
    cout << to_sys_days(TDate{35065}) << '\n';

    cout << (int)to_TDate(1899_y/December/30) << '\n';
    cout << (int)to_TDate(1900_y/January/1) << '\n';
    cout << (int)to_TDate(1899_y/December/29) << '\n';
    cout << (int)to_TDate(1996_y/January/1) << '\n';
}
который выводит:

1899-12-30
1900-01-01
1899-12-29
1996-01-01
0
2
-1
35065
Этот вывод соответствует тому, что указано в TDateTimeдокументации.

Для преобразования в/из TDateTimeудобно позволить клиенту выбирать точность преобразования sys_timeв или из. Например, это преобразуется в точность minutes:

to_sys_time<minutes>(TDateTime{2.75})
в то время как это преобразуется в точность milliseconds:

to_sys_time<milliseconds>(TDateTime{2.75})
Вот реализация:

template <class D>
date::sys_time<D>
to_sys_time(System::TDateTime dt)
{
    using namespace date;
    using namespace std::chrono;
    using fdays = duration<double, days::period>;
    using ftime = time_point<system_clock, fdays>;
    auto ft = ftime{fdays{static_cast<double>(dt)}};
    if (ft < ftime{})
    {
        auto d = time_point_cast<days>(ft);
        auto t = d - ft;
        ft = d + t;
    }
    ft -= sys_days{1970_y/January/1} - sys_days{1899_y/December/30};
    return round<D>(ft);
}
Для моментов времени, не предшествующих эпохе TDateTime, это довольно просто. Это помогает создать durationхронограф time_pointна основе doubleэтого значения days. Затем просто извлекается doubleиз TDateTimeи преобразуется в нашу двойную базу time_point, вычитается разница в эпохах, а затем используется round<D>возможность усечь результат до требуемой точности.

Если это предэпоха TDateTime, то есть дополнительный танец, чтобы рассматривать целую и дробную части doubleотдельно.

Обратное преобразование аналогично:

template <class D>
System::TDateTime
to_TDateTime(date::sys_time<D> tp)
{
    using namespace date;
    using namespace std::chrono;
    using fdays = duration<double, days::period>;
    using ftime = time_point<system_clock, fdays>;
    auto ft = ftime{tp} + (sys_days{1970_y/January/1} - sys_days{1899_y/December/30});
    if (ft >= ftime{})
        return System::TDateTime(ft.time_since_epoch().count());
    auto d = floor<days>(ft);
    auto t = d - ft;
    return System::TDateTime((d + t).time_since_epoch().count());
}
Все это можно реализовать следующим образом:

int
main()
{
    using namespace date;
    using namespace std::chrono;
    using namespace System;
    cout << to_sys_time<minutes>(TDateTime{0.}) << '\n';
    cout << to_sys_time<minutes>(TDateTime{2.75}) << '\n';
    cout << to_sys_time<minutes>(TDateTime{-1.25}) << '\n';
    cout << to_sys_time<minutes>(TDateTime{35065.}) << '\n';

    cout << (double)to_TDateTime(sys_days{1899_y/December/30} + 0min) << '\n';
    cout << (double)to_TDateTime(sys_days{1900_y/January/1} + 18h + 0min) << '\n';
    cout << (double)to_TDateTime(sys_days{1899_y/December/29} + 6h + 0min) << '\n';
    cout << (double)to_TDateTime(sys_days{1996_y/January/1} + 0min) << '\n';
}
который выводит:

1899-12-30 00:00
1900-01-01 18:00
1899-12-29 06:00
1996-01-01 00:00
0
2.75
-1.25
35065

Как конвертировать в/из QDate
( Говард Хиннант )

Вот функции, которые вы можете использовать для преобразования между QDateи sys_days:

date::sys_days
to_sys_days(QDate qd)
{
    using namespace date;
    return sys_days{days{qd.toJulianDay()} -
        (sys_days{1970_y/January/1} - sys_days{year{-4713}/November/24})};
}

QDate
to_QDate(date::sys_days sd)
{
    using namespace date;
    return QDate::fromJulianDay((sd.time_since_epoch() +
        (sys_days{1970_y/January/1} - sys_days{year{-4713}/November/24})).count());
}
Они работают, просто регулируя эпоху этих двух типов.


Как конвертировать между Windows FILETIMEиsystem_clock
( Билли О'Нил )

Вот функции и определения типов, которые вы можете использовать для работы со структурой FILETIME в Windows.

Предположение system_clockосновано на эпохе Unix:

using std::ratio;
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::system_clock;

// filetime_duration has the same layout as FILETIME; 100ns intervals
using filetime_duration = duration<int64_t, ratio<1, 10'000'000>>;
// January 1, 1601 (NT epoch) - January 1, 1970 (Unix epoch):
constexpr duration<int64_t> nt_to_unix_epoch{INT64_C(-11644473600)};

system_clock::time_point FILETIME_to_system_clock(FILETIME fileTime) {
    const filetime_duration asDuration{static_cast<int64_t>(
        (static_cast<uint64_t>(fileTime.dwHighDateTime) << 32)
            | fileTime.dwLowDateTime)};
    const auto withUnixEpoch = asDuration + nt_to_unix_epoch;
    return system_clock::time_point{
        duration_cast<system_clock::duration>(withUnixEpoch)};
}

FILETIME system_clock_to_FILETIME(system_clock::time_point systemPoint) {
    const auto asDuration = duration_cast<filetime_duration>(
        systemPoint.time_since_epoch());
    const auto withNtEpoch = asDuration - nt_to_unix_epoch;
    const uint64_t rawCount = withNtEpoch.count();
    FILETIME result;
    result.dwLowDateTime = static_cast<DWORD>(rawCount); // discards upper bits
    result.dwHighDateTime = static_cast<DWORD>(rawCount >> 32);
    return result;
}

Как конвертировать в/изboost::posix_time::ptime
( Говард Хиннант )

boost::posix_time::ptimeи std::chrono::system_clock::time_pointпредставляют одно и то же в своих библиотеках (boost и Chronic). Так что приятно иметь возможность конвертировать между ними. Вот как это сделать:

#include "boost/date_time/posix_time/posix_time.hpp"
#include <chrono>

std::chrono::system_clock::time_point
to_system_clock(boost::posix_time::ptime const& pt)
{
    using namespace std;
    using namespace boost;
    auto const dt = pt - posix_time::from_time_t(0);
#ifdef BOOST_DATE_TIME_HAS_NANOSECONDS
    chrono::nanoseconds ns{dt.total_nanoseconds()};
    auto sysd = chrono::duration_cast<chrono::system_clock::duration>(ns);
    return chrono::system_clock::time_point{sysd};
#else
    chrono::microseconds us{dt.total_microseconds()};
    return chrono::system_clock::time_point{us};
#endif
}

boost::posix_time::ptime
to_ptime(std::chrono::system_clock::time_point const& st)
{
    using namespace std;
    using namespace boost;
#ifdef BOOST_DATE_TIME_HAS_NANOSECONDS
    posix_time::nanoseconds fs{
        chrono::nanoseconds{st.time_since_epoch()}.count()};
#else
    posix_time::microseconds fs{
        chrono::duration_cast<chrono::microseconds>(
            st.time_since_epoch()).count()};
#endif
    return posix_time::from_time_t(0) + fs;
}

Распечатайте компактный календарь на год.
( Говард Хиннант )

Распечатать календарь на целый год — интересное занятие. Вы можете либо просто взять код и использовать его (изящная и полезная утилита), либо изучить его реализацию и узнать гораздо больше о "date.h".

Сначала код, а потом подробное объяснение:

#include "date/date.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <locale>
#include <ostream>
#include <stdexcept>
#include <string>

date::year
current_year()
{
    using namespace std::chrono;
    using namespace date;
    year_month_day ymd = floor<days>(system_clock::now());
    return ymd.year();
}

// The number of weeks in a calendar month layout plus 2 more for the calendar titles
unsigned
number_of_lines_calendar(date::year_month const ym, date::weekday const firstdow)
{
    using namespace date;
    return static_cast<unsigned>(
        ceil<weeks>((weekday{ym/1} - firstdow) + ((ym/last).day() - day{0})).count()) + 2;
}

// Print one line of a calendar month
void
print_line_of_calendar_month(std::ostream& os, date::year_month const ym,
                             unsigned const line, date::weekday const firstdow)
{
    using namespace std;
    using namespace date;
    switch (line)
    {
    case 0:
        // Output month and year title
        os << left << setw(21) << format(os.getloc(), " %B %Y", ym) << right;
        break;
    case 1:
        {
        // Output weekday names title
        auto wd = firstdow;
        do
        {
            auto d = format(os.getloc(), "%a", wd);
            d.resize(2);
            os << ' ' << d;
        } while (++wd != firstdow);
        break;
        }
    case 2:
        {
        // Output first week prefixed with spaces if necessary
        auto wd = weekday{ym/1};
        os << string(static_cast<unsigned>((wd-firstdow).count())*3, ' ');
        auto d = 1_d;
        do
        {
            os << format(" %e", d);
            ++d;
        } while (++wd != firstdow);
        break;
        }
    default:
        {
        // Output a non-first week:
        // First find first day of week
        unsigned index = line - 2;
        auto sd = sys_days{ym/1};
        if (weekday{sd} == firstdow)
            ++index;
        auto ymdw = ym/firstdow[index];
        if (ymdw.ok()) // If this is a valid week, print it out
        {
            auto d = year_month_day{ymdw}.day();
            auto const e = (ym/last).day();
            auto wd = firstdow;
            do
            {
                os << format(" %e", d);
            } while (++wd != firstdow && ++d <= e);
            // Append row with spaces if the week did not complete
            os << string(static_cast<unsigned>((firstdow-wd).count())*3, ' ');
        }
        else  // Otherwise not a valid week, output a blank row
            os << string(21, ' ');
        break;
        }
    }
}

void
print_calendar_year(std::ostream& os, unsigned const cols = 3,
                    date::year const y = current_year(),
                    date::weekday const firstdow = date::Sunday)
{
    using namespace date;
    if (cols == 0 || 12 % cols != 0)
        throw std::runtime_error("The number of columns " + std::to_string(cols)
                                 + " must be one of [1, 2, 3, 4, 6, 12]");
    // Compute number of lines needed for each calendar month
    unsigned ml[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    for (auto& m : ml)
        m = number_of_lines_calendar(y/month{m}, firstdow);
    for (auto r = 0u; r < 12/cols; ++r) // for each row
    {
        const auto lines = *std::max_element(std::begin(ml) + (r*cols),
                                             std::begin(ml) + ((r+1)*cols));
        for (auto l = 0u; l < lines; ++l) // for each line
        {
            for (auto c = 0u; c < cols; ++c) // for each column
            {
                if (c != 0)
                    os << "   ";
                print_line_of_calendar_month(os, y/month{r*cols + c+1}, l, firstdow);
            }
            os << '\n';
        }
        os << '\n';
    }
}

int
main()
{
    print_calendar_year(std::cout);
}
Как написано, эта программа выводит:

 January 2016            February 2016           March 2016          
 Su Mo Tu We Th Fr Sa    Su Mo Tu We Th Fr Sa    Su Mo Tu We Th Fr Sa
                 1  2        1  2  3  4  5  6           1  2  3  4  5
  3  4  5  6  7  8  9     7  8  9 10 11 12 13     6  7  8  9 10 11 12
 10 11 12 13 14 15 16    14 15 16 17 18 19 20    13 14 15 16 17 18 19
 17 18 19 20 21 22 23    21 22 23 24 25 26 27    20 21 22 23 24 25 26
 24 25 26 27 28 29 30    28 29                   27 28 29 30 31      
 31                                                                  

 April 2016              May 2016                June 2016           
 Su Mo Tu We Th Fr Sa    Su Mo Tu We Th Fr Sa    Su Mo Tu We Th Fr Sa
                 1  2     1  2  3  4  5  6  7              1  2  3  4
  3  4  5  6  7  8  9     8  9 10 11 12 13 14     5  6  7  8  9 10 11
 10 11 12 13 14 15 16    15 16 17 18 19 20 21    12 13 14 15 16 17 18
 17 18 19 20 21 22 23    22 23 24 25 26 27 28    19 20 21 22 23 24 25
 24 25 26 27 28 29 30    29 30 31                26 27 28 29 30      

 July 2016               August 2016             September 2016      
 Su Mo Tu We Th Fr Sa    Su Mo Tu We Th Fr Sa    Su Mo Tu We Th Fr Sa
                 1  2        1  2  3  4  5  6                 1  2  3
  3  4  5  6  7  8  9     7  8  9 10 11 12 13     4  5  6  7  8  9 10
 10 11 12 13 14 15 16    14 15 16 17 18 19 20    11 12 13 14 15 16 17
 17 18 19 20 21 22 23    21 22 23 24 25 26 27    18 19 20 21 22 23 24
 24 25 26 27 28 29 30    28 29 30 31             25 26 27 28 29 30   
 31                                                                  

 October 2016            November 2016           December 2016       
 Su Mo Tu We Th Fr Sa    Su Mo Tu We Th Fr Sa    Su Mo Tu We Th Fr Sa
                    1           1  2  3  4  5                 1  2  3
  2  3  4  5  6  7  8     6  7  8  9 10 11 12     4  5  6  7  8  9 10
  9 10 11 12 13 14 15    13 14 15 16 17 18 19    11 12 13 14 15 16 17
 16 17 18 19 20 21 22    20 21 22 23 24 25 26    18 19 20 21 22 23 24
 23 24 25 26 27 28 29    27 28 29 30             25 26 27 28 29 30 31
 30 31                                                               
Но приведенная выше программа очень гибкая и может локализовать этот вывод в самые разные форматы в соответствии с вашими предпочтениями.

date::year
current_year();
Все, current_year()что нужно сделать, это найти текущий year. Часовой пояс UTC используется для простоты. Если этого недостаточно для ваших нужд, достаточно легко указать, на какой год вам нужен календарь. Это работает путем вызова system_clock::now(), усечения результата до sys_days(количества дней), преобразования sys_daysв year_month_dayи возврата year()его поля.

unsigned
number_of_lines_calendar(date::year_month const ym, date::weekday const firstdow);
Эта функция вычисляет количество строк, которое потребуется для распечатки календаря для комбинации года и месяца ymи использования firstdowв качестве первого дня недели для этого календаря. Первое, что нужно вычислить, — это количество дней, на которое первое число месяца прошло после первого дня недели: (weekday{ym/1} - firstdow). Вычитание дней недели производится без знака по модулю 7, поэтому это всегда положительное число в диапазоне [0, 6], независимо от базовой кодировки weekday.

Добавьте к этому количество дней в месяце. Это вычисляется по ((ym/last).day() - day{0}). Выражение ym/lastсоздает число year_month_day_last, представляющее дату последнего дня месяца. .day()извлекает поле дня. Мы могли бы вычесть day{1}, а затем добавить days{1}к этой разнице, но проще просто вычесть недействительную day{0}. Тип этой разницы days: a chrono::duration.

Далее мы хотим преобразовать это число daysв weeksиспользование ceil, которое будет округляться в большую сторону, если преобразование будет неточным. Это позволяет использовать месяцы, которые не полностью заполняют свою последнюю строку. Затем мы извлекаем количество недель .count()и добавляем 2дополнительные строки: одну для названия дня недели и одну для названия месяца и года.

void
print_line_of_calendar_month(std::ostream& os, date::year_month const ym,
                             unsigned const line, date::weekday const firstdow);
Это суть логики печати календаря. При этом печатается одна строка месячного календаря без разрыва строки. Аргумент lineговорит, какая строка [0, бесконечность]. Если запрашивается больше строк, чем занимает календарь, печатаются пустые строки. Календарь начинается с weekday firstdow. Вся функция — это просто включение, lineчтобы увидеть, какую строку распечатать:

0: Распечатайте название месяца и года, используя ymи localeизвлеченные из os. Выровняйте этот вывод по левому краю в поле из 21 пробела, за исключением отступа на один пробел.

1: распечатайте заголовок дня недели, используя первые две буквы локализованного сокращения дней недели. Начните с first dowи повторяйте в течение 1 недели. Название каждого дня недели состоит из двух букв и выравнивается по правому краю на 3 пробела.

2: Распечатайте первую неделю. Это единственная неделя, перед которой могут стоять пробелы. Количество пробелов в префиксе равно 3 для каждого дня, после которого первое число месяца превышает первый день недели: weekday{ym/1} - firstdow. Затем, начиная с 1_d, выведите каждый день (без префикса 0) по правому краю в поле из 3 пробелов. Повторяйте до тех пор, пока приращение дня недели не вернется к значению firstdow.

3 - infinity: может выводиться либо неделя, начинающаяся с ym/firstdow[index], либо пустая строка. Он напечатает последнее, если ym/firstdow[index]это недействительная дата. Выражение ym/firstdow[index]означает n- ное firstdow число месяца этого года. Это indexфункция количества lineи того, выводим ли мы a firstdowдля line == 2. Если мы выведем a firstdowдля строки 2, то index == line - 1,other index == line - 2. Например line == 3, большую часть времени мы ищем первое значение firstdowза этот год/месяц.

Как только у нас есть ymdw(первая дата, которую нужно распечатать для этой строки), нам нужно преобразовать ее в, year_month_dayчтобы мы могли извлечь dayиз нее поле. И нам также нужно вычислить последнее dayчисло месяца (хранится в e). Затем мы выполняем итерацию от firstdow, распечатывая поле дня (выровненное по правому краю в поле из 3 пробелов) до тех пор, пока мы не напечатаем полную неделю или пока не напечатаем последний день месяца, в зависимости от того, что наступит раньше.

Наконец, мы проверяем, вывели ли мы полную неделю, и если нет, то сколько дней недели мы не распечатали ( firstdow - wd). Если это не ноль, то мы дополняем строку тремя пробелами для каждого дня.

На этом самая сложная часть самой сложной функции всей утилиты закончена!

void
print_calendar_year(std::ostream& os, unsigned const cols = 3,
                    date::year const y = current_year(),
                    date::weekday const firstdow = date::Sunday);
Эта функция печатает годовой календарь, osвызывая уже определенные функции. Календарь имеет формат по colsмесяцам rows, который colsвводится клиентом и показывает, сколько месяцев вы хотите распечатать по горизонтали. Этот аргумент должен быть одним из [1, 2, 3, 4, 6, 12]. В приведенном выше примере по умолчанию это значение составляет 3 месяца с шагом 4 вниз. Год можно ввести или по умолчанию использовать UTC текущего года. Можно указать день недели, с которого начинается календарь. По умолчанию это воскресенье.

Первое, что нужно сделать, это проверить colsправильность значения и создать исключение, если оно не соответствует.

Далее нам нужно узнать, сколько строк yнужно для каждого месяца года. Это делается путем вызова number_of_lines_calendarкаждого месяца этого года и сохранения результата в формате ml.

Тогда у нас есть 3 вложенных цикла. Внешний цикл обрабатывает количество строк (количество календарных месяцев по вертикали), которое равно 12/cols. Затем для каждой строки вычислите максимальное количество строк, необходимое для вывода каждый месяц для этой строки. Затем для каждой строки переберите количество столбцов (ежемесячные календари выводятся горизонтально).

Теперь просто распечатайте по одной строке для каждого календаря для этой {row, col, line}комбинации. Месяц для этой комбинации является пронумерованным r*cols + c + 1(индексация по строкам начинается с единицы). Также распечатайте 3 пробела между каждым месяцем по горизонтали и одну пустую строку между каждой строкой календарей для красивого заполнения.

Эту программу теперь можно использовать для локализации и вывода самых разных календарей. Например, немецкий календарь в формате 4x3 на 2016 год (выход в macOS, который поддерживает эту локализацию):

using namespace date::literals;
std::cout.imbue(std::locale("de_DE"));
print_calendar_year(std::cout, 4, 2016_y, Monday);
который выводит:

 Januar 2016             Februar 2016            März 2016              April 2016          
 Mo Di Mi Do Fr Sa So    Mo Di Mi Do Fr Sa So    Mo Di Mi Do Fr Sa So    Mo Di Mi Do Fr Sa So
              1  2  3     1  2  3  4  5  6  7        1  2  3  4  5  6                 1  2  3
  4  5  6  7  8  9 10     8  9 10 11 12 13 14     7  8  9 10 11 12 13     4  5  6  7  8  9 10
 11 12 13 14 15 16 17    15 16 17 18 19 20 21    14 15 16 17 18 19 20    11 12 13 14 15 16 17
 18 19 20 21 22 23 24    22 23 24 25 26 27 28    21 22 23 24 25 26 27    18 19 20 21 22 23 24
 25 26 27 28 29 30 31    29                      28 29 30 31             25 26 27 28 29 30   

 Mai 2016                Juni 2016               Juli 2016               August 2016         
 Mo Di Mi Do Fr Sa So    Mo Di Mi Do Fr Sa So    Mo Di Mi Do Fr Sa So    Mo Di Mi Do Fr Sa So
                    1           1  2  3  4  5                 1  2  3     1  2  3  4  5  6  7
  2  3  4  5  6  7  8     6  7  8  9 10 11 12     4  5  6  7  8  9 10     8  9 10 11 12 13 14
  9 10 11 12 13 14 15    13 14 15 16 17 18 19    11 12 13 14 15 16 17    15 16 17 18 19 20 21
 16 17 18 19 20 21 22    20 21 22 23 24 25 26    18 19 20 21 22 23 24    22 23 24 25 26 27 28
 23 24 25 26 27 28 29    27 28 29 30             25 26 27 28 29 30 31    29 30 31            
 30 31                                                                                       

 September 2016          Oktober 2016            November 2016           Dezember 2016       
 Mo Di Mi Do Fr Sa So    Mo Di Mi Do Fr Sa So    Mo Di Mi Do Fr Sa So    Mo Di Mi Do Fr Sa So
           1  2  3  4                    1  2        1  2  3  4  5  6              1  2  3  4
  5  6  7  8  9 10 11     3  4  5  6  7  8  9     7  8  9 10 11 12 13     5  6  7  8  9 10 11
 12 13 14 15 16 17 18    10 11 12 13 14 15 16    14 15 16 17 18 19 20    12 13 14 15 16 17 18
 19 20 21 22 23 24 25    17 18 19 20 21 22 23    21 22 23 24 25 26 27    19 20 21 22 23 24 25
 26 27 28 29 30          24 25 26 27 28 29 30    28 29 30                26 27 28 29 30 31   
                         31                                                                  

Анализ однозначной даты и времени при переходе на дневной свет
( Тай Мэн )

Нам нужна была возможность (де)сериализовать значения даты и времени. Мы исследовали, как можно проанализировать входную строку, которая выглядит следующим образом:

1) 1999-10-31 01:30:00 US/Pacific PST
Посоветовавшись с Говардом , мы узнали, что вместо этого входная строка должна выглядеть так:

2) 1999-10-31 01:30:00 -08:00 US/Pacific
и тогда решение простое. Преобразование PST в -08:00, как правило, нетривиально и часто требует ввода данных пользователем, поскольку сокращения часто соответствуют нескольким часовым поясам. Этот рецепт предполагает, что у пользователей этого рецепта будет возможность переформатировать строки формы 1) в строки формы 2).

Как только у нас будет строка формы 2), попробуйте этот пример кода (спасибо Аарону , который предоставил черновик):

#include <iostream>
#include <sstream>
#include <string>
#include "date/tz.h"

int main()
{
    using namespace std;
    using namespace date;

    istringstream inputStream{ "1999-10-31 01:30:00 -08:00 US/Pacific" };

    // Using local_seconds would resolve in ambiguous date exception
    sys_seconds tp;
    string tz_name;
    inputStream >> parse("%F %T %Ez %Z", tp, tz_name);

    // bool operator tells us whether stream was successfully parsed
    assert(bool(inputStream));

    zoned_time zt{tz_name, tp};

    // This will output America/Los_Angeles, because US/Pacific is an alias of it.
    cout << format("%F %T %Ez", zt) << ' ' << zt.get_time_zone()->name() << '\n';
}
Обновление от Говарда :

Теперь есть функция анализа исходного формата:

1) 1999-10-31 01:30:00 US/Pacific PST
И проверьте, согласована ли аббревиатура часового пояса, и в неоднозначном случае используйте аббревиатуру часового пояса, чтобы устранить неоднозначность отметки времени:

#include <iostream>
#include <sstream>
#include <string>
#include "date/tz.h"

int main()
{
    using namespace std;
    using namespace date;

    istringstream inputStream{ "1999-10-31 00:30:00 US/Pacific PST" };

    // Using local_seconds would resolve in ambiguous date exception
    local_seconds tp;
    string tz_name;
    string tz_abbrev;
    inputStream >> parse("%F %T", tp) >> tz_name >> tz_abbrev;

    // bool operator tells us whether stream was successfully parsed
    assert(bool(inputStream));

    // Check for ambiguous and nonexistent timestamps
    auto zone = locate_zone(tz_name);
    auto info = zone->get_info(tp);
    zoned_seconds zt{zone};
    switch (info.result)
    {
    case local_info::unique:
        zt = tp;  // easy case
        // One can check that the tz_abbrev is consistent with
        //   info.first.abbrev if desired.
        break;
    case local_info::nonexistent:
        // time stamp never existed.  Throw an error?
        // Or here is how map to a unique UTC equivalent:
        zt = zoned_time{zone, tp, choose::earliest};  // choose::latest also
                                                      //   gives same answer.
        break;
    case local_info::ambiguous:
        // Use tz_abbrev to break the ambiguity
        if (info.first.abbrev == tz_abbrev)
            zt = zoned_time{zone, tp, choose::earliest};
        else if (info.second.abbrev == tz_abbrev)
            zt = zoned_time{zone, tp, choose::latest};
        else
            throw std::runtime_error(tz_abbrev +
                " is not a valid abbreviation for " + tz_name);
        break;
    }

    // This will output America/Los_Angeles, because US/Pacific is an alias of it.
    cout << format("%F %T %Ez", zt) << ' ' << zt.get_time_zone()->name() << '\n';
}
Это включает в себя получение local_infoструктуры из файла time_zonefor that local_time. Он local_infoбудет иметь всю информацию об этой time_zone/local_timeкомбинации, в том числе о том, существует ли уникальное сопоставление с UTC, несуществующее сопоставление (как в случае с разрывом, созданным «прыжком вперед») или неоднозначное сопоставление (созданное локальным временем, происходящим дважды во время «отступления»).

В неоднозначном случае вы можете просмотреть оба сопоставления, включая их сокращения, и сравнить их с проанализированным вами сокращением, а затем выбрать либо более раннее сопоставление, либо более позднее сопоставление.


microfortnights?! Ты серьезно?
( Говард Хиннант )

Ну, типа того. Цель этой статьи — продемонстрировать, что какими бы безумными ни были ваши единицы времени, эта библиотека справится с этим, причем стильно.

Если вы имеете дело с длительностью в четверть секунды или длительностью кадра в 1/60 секунды или с чем-то еще, <chrono>вы можете построить длительность, и эта библиотека может ее отформатировать. И все это с минимальными усилиями. Допустим, у вас есть момент времени, состоящий из месяца, дня, года, часа, минуты и микронедели. Насколько сложно его отформатировать в удобочитаемый формат?!

Оказывается, совсем не сложно.

#include "date/date.h"
#include <iostream>

using fortnights =
    std::chrono::duration<date::weeks::rep,
                          std::ratio_multiply<std::ratio<2>, date::weeks::period>>;

using microfortnights =
    std::chrono::duration<std::int64_t, std::ratio_multiply<fortnights::period,
                                                            std::micro>>;

constexpr
inline
microfortnights
operator"" _ufn(unsigned long long x)
{
    return microfortnights{static_cast<microfortnights::rep>(x)};
}

int
main()
{
    using namespace date;
    using namespace std::chrono;
    cout << format("%F %T\n", sys_days{November/29/2016} + 15h + 13min + 35_ufn);
}
Первое, что нужно сделать, это создать файл, chrono::durationпредставляющий собой файл microfortnight. Лучше всего это сделать, сначала построив fortnight, а затем умножив его на std::micro. Это создаст какую-то странность chrono::durationс периодом, который нам на самом деле не обязательно знать, но оказывается, что он не так уж далек от секунды.

Чтобы указать a sys_timeв терминах этой странной единицы, вы просто делаете обычное сложение. Но вместо добавления secondsвы добавляете microfortnights. Теперь оказывается, что микронеделя равна ровно 1,2096 секунды (кто знал?). Но вам не нужно беспокоиться об этих деталях, если вы правильно определили fortnights2 weeks, как указано выше, и microfortnightsзначение a, std::microуказанное fortnightsвыше.

Теперь вы можете просто слепо добавить microfortnights{35}к метке времени года/месяца/дня hh::min, как показано выше, и попросить formatотформатировать ее с помощью "%F %T". Это выведет правильную отметку времени с дробными десятичными секундами, чтобы точно представить microfortnights:

2016-11-29 15:13:42.3360
Если эта библиотека может так легко сделать это с чем-то столь сумасшедшим, как microfortnights, она сможет справиться с вашей сумасшедшей проблемой времени.


Найдите и установите папку загрузки UNIX с помощью xdg-user-dir
(от Coin² )

Библиотека использует путь ~/Downloadsпо умолчанию. Что делать, если вы используете другой путь для каталога загрузки, например ~/Téléchargementsили, ~/Завантаженняи хотите, чтобы ваша программа автоматически находила этот путь? Решение — использовать xdg_user_dir . Следующий код показывает, как это можно сделать:

#include "date/date.h"
#include "date/tz.h"

#include <regex>
#include <iostream>

// Execute command, use a pipe and get the output of it (we cannot grab the output with std::system)
std::string exec_and_get_output(const char* cmd) 
{
    std::array<char, 128> buffer;
    std::string output;
    FILE *pipe(popen(cmd, "r"));
    if (!pipe) 
        throw std::runtime_error("Cannot execute command");
    while (!feof(pipe)) 
    {
        if (fgets(buffer.data(), 128, pipe) != nullptr)
            output += buffer.data();
    }
    pclose(pipe);
    return output;
}

std::string get_xdg_download_folder(const std::string& suffix = "tzdata_alt") 
{
    std::string download_folder = "~/Downloads";  // common download folder by default
    std::string xdg_command = "xdg-user-dir DOWNLOAD";
    std::string xdg_download_folder = exec_and_get_output(xdg_command.c_str());
    xdg_download_folder.erase // remove unnecessary new lines
    (
        std::remove(xdg_download_folder.begin(), xdg_download_folder.end(), '\n'), 
        xdg_download_folder.end()
    ); 
    // Verify that output from command is a folder so we can use it
    if (std::regex_match(xdg_download_folder, std::regex("^(/[^/ ]*)+/?$"))) 
    {
        download_folder = xdg_download_folder; 
    }
    return download_folder + '/' + suffix;
}

int main() 
{
    auto download_folder = get_xdg_download_folder();
    date::set_install(download_folder);

    // Your code which uses tz
}
Не забудьте настроить, -DAUTO_DOWNLOAD=1чтобы при первом использовании загружались tzdata.


Повторное использование незонированного кода даты/времени для зональных значений даты/времени (и наоборот)
( Тай Мэн )

Библиотеки даты/времени запрещают неявные преобразования между local_time и sys_time, и на это есть веские причины. Если мы везде будем вводить переменные с использованием «auto», рано или поздно мы столкнемся со случаем, когда мы будем присваивать local_time sys_time или наоборот, и компилятор уловит это опасное преобразование. Спасибо за безопасность ввода!

Однако иногда, чтобы повторно использовать существующий код, нам действительно нужно преобразовать local_time в sys_time. Если это то, чего вы хотите достичь, смело пропустите приведенную ниже историю и прочитайте следующее за ней решение.

<история начинается>

Предположим, вы написали слой кода для обработки незонированного сложения даты и времени с длительностью ISO в форме P1DT2H (период 1 день и 2 часа. Ссылка: https://en.wikipedia.org/wiki/ISO_8601#Durations ). ). В вашем коде вы бы везде использовали local_time в качестве типа даты/времени.

Теперь появилось новое требование пользователя. Пользователи хотят добавить зональную дату/время с длительностью ISO. Вы смотрите на код и начинаете хмуриться. Некоторые из ваших функций возвращают local_time, поэтому компилятор запрещает вам перегружать эти функции типом возврата sys_time. Хорошо, вы можете скопировать существующие функции и переименовать их, чтобы они возвращали sys_time. Но теперь вам придется поддерживать две параллельные ветви кода, которые будут более подвержены дефектам. Разве не было бы неплохо, если бы вы могли сократить зонированную дату/время до незонированной даты/времени, повторно использовать существующий код, а затем повторно добавить информацию о часовом поясе к выходному значению даты/времени?

<история заканчивается>

Вот как мы можем конвертировать между local_time и sys_time:

const some_local_time unzonedTime{utcTime.time_since_epoch()};
const some_sys_time utcTime{unzonedTime.time_since_epoch()};
Где some_local_time и some_sys_time — это экземпляры шаблонов local_time и sys_time.


Мысли о перезагрузке IANA tzdb для долго работающих программ
( Говард Хиннант )

Начнем с этой мысли: большинству программ не нужно работать достаточно долго, чтобы беспокоиться об изменении базы данных часовых поясов IANA. База данных часовых поясов IANA обновляется каждый месяц и каждый сезон (3 месяца, может быть 4). Регулярного расписания обновлений базы данных часовых поясов IANA нет. Но это происходит не ежедневно и даже не еженедельно, а происходит несколько раз в год.

Далее, если ваша программа работает достаточно долго и ей нужно беспокоиться о последней версии tzdb IANA, и если несколько потоков обращаются к tzdb, то у вас есть проблемы с многопоточностью, и эта библиотека не предоставляет готовых готовых функций. доказательство решения для вас. Причина этого в том, что я не хочу снижать производительность подавляющего большинства клиентов tzdb, которым не требуется поддержка потоков для обновляемых баз данных tzdb. Большинству клиентов подходит IANA tzdb, который существует при запуске их программы, и им не нужно беспокоиться об его обновлении во время работы программы.

Наконец, если вы находитесь в большинстве и не хотите беспокоиться об обновлении tzdb IANA во время работы вашей программы, все , что вам нужно сделать, это никогда не вызывать date::reload_tzdb(). Если вы никогда не вызываете date::reload_tzdb(), то у вас никогда не возникнет проблем с безопасностью потоков, которые рассматриваются в этой короткой статье. С тобой все в порядке, хватит читать. Иди возьми мохито.

Хорошо, теперь вы в положении: у меня есть несколько потоков, обращающихся к tzdb, и IANA обновило базу данных. Мне нужно перейти в эту новую базу данных, не останавливая все мои потоки. ПОМОЩЬ!!!

Помощь вам – вот чему посвящена эта статья.

Эта библиотека поддерживает односвязный список tzdbкак синглтон. Глава этого списка — это то, к чему вы неявно получаете доступ через общие функции, такие как current_zone()или locate_zone("some time zone name"). Если вы никогда не позвоните date::reload_tzdb(), этот список всегда будет иметь длину 1, и вам просто не придется об этом беспокоиться. Но если вы вызовете date::reload_tzdb(), и если этой функции удастся инициализировать new tzdb, она атомарно отправит новую tzdbв этот одноэлементный список. date::reload_tzdb()не делает недействительными ссылки или указатели на любой из предыдущих файлов tzdb. Они продолжают существовать и прекрасно работать.

Здесь начинается стратегия вашего кода...

Один из способов справиться с этим — просто позволить синглтону tzdb_listрасти вечно. Меня это устраивает, если тебя устраивает. Он увеличивается только на одну базу данных несколько раз в год. Это гарантирует, что все ваши потоки, которые могут указывать на более старые, tzdbпродолжат работать, хотя они могут использовать устаревшие данные.

Другой способ справиться с этим — использовать априорное знание о том, что ваши потоки не будут указывать на tzdbболее длительный Xпериод времени, например Xминуту, час, день, год и т. д. По истечении времени вы можете просто перемещаться по синглтону tzdb_listи удалять старые базы данных.

Ниже приведен код, который запускает поток, который ничего не делает, а только просыпается один раз в день и загружает новый tzdb, если он доступен, и устанавливает таймер для удаления старых tzdb, если с момента их замены прошло 10 дней.

#include "date/tz.h"
#include <atomic>
#include <iostream>
#include <thread>

// Checks for new tzdb once a day (early am local time).
// Allows old tzdb to hang around for 10 days and then deletes it
// Lock-free
void
tzdb_manager(std::atomic<bool>& run)
{
    using namespace std;
    using namespace std::chrono;
    using namespace date;
    // Get the current UTC time for today's local 02:00, approximation is ok
    auto tz = current_zone();
    auto check_at = zoned_time{tz,
                               floor<days>(zoned_time{tz, system_clock::now()}
                               .get_local_time()) + 2h, choose::latest}
                                   .get_sys_time();
    // Initialize clean-trigger for several years in the future
    auto clean_at = check_at + days{1000};
    while (run)
    {
        // Sleep until the wee hours of the morning
        this_thread::sleep_until(check_at);
        // If time to stop, stop
        if (!run)
            break;
        // Time for morning maintenance
        // First check if we need to pop_back the tzdb_list
        if (check_at >= clean_at)
        {
            auto& list = get_tzdb_list();
            auto i0 = list.begin();
            auto i1 = next(i0);
            // if the list has more than one tzdb
            if (i1 != list.end())
            {
                // pop_back the tzdb_list
                for (auto i2 = next(i1); i2 != list.end(); ++i0, ++i1, ++i2)
                    ;
                list.erase_after(i0);
                // if the new size is 1, clean again in a few years
                if (i0 == list.begin())
                    clean_at = check_at + days{1000};
                else  // otherwise new size > 1, clean again in 10 days
                    clean_at = check_at + days{10};
            }
            else  // list has only 1 tzdb, clean again in a few years
                clean_at = check_at + days{1000};
        }
        // If the remote version has been updated
        auto rv = remote_version();
        if (rv != get_tzdb().version)
        {
            // download, install and load the new tzdb
            if (remote_download(rv))
            {
                if (remote_install(rv))
                {
                    reload_tzdb();
                    // Schedule cleanup of the old tzdb for 10 days from now
                    clean_at = check_at + days{10};
                }
            }
            // if anything failed, we just try again tomorrow morning
        }
        // Maintenance done, go to sleep for a day
        check_at += days{1};
    }
}
Вышеупомянутое предполагает сборку с DAUTO_DOWNLOAD=0. То есть tz lib не будет автоматически загружать tzdb IANA. Если вы хотите использовать DAUTO_DOWNLOAD=1(это значение по умолчанию в macOS и Linux), приведенный выше код можно немного упростить, изменив:

        auto rv = remote_version();
        if (rv != get_tzdb().version)
        {
            // download, install and load the new tzdb
            if (remote_download(rv))
            {
                if (remote_install(rv))
                {
                    reload_tzdb();
                    // Schedule cleanup of the old tzdb for 10 days from now
                    clean_at = check_at + days{10};
                }
            }
            // if anything failed, we just try again tomorrow morning
        }
к

        auto rv = remote_version();
        if (rv != get_tzdb().version)
        {
            // download, install and load the new tzdb
            reload_tzdb();
            // Schedule cleanup of the old tzdb for 10 days from now
            clean_at = check_at + days{10};
            // if anything failed, we just try again tomorrow morning
        }
Это не самый дружелюбный код, который нужно писать. Но с другой стороны, это дает вам полную свободу действий в tzdbполитике удаления и не наказывает клиентов, которым не нужно обновлять свою tzdbбазу данных.

Более сложные (и дорогие) политики могут отслеживать использование счетчика ссылок для каждого tzdb и удалять его только тогда, когда этот счетчик ссылок упадет до нуля. Его полностью реализуемо извне без каких-либо привилегированных подключений к "tz.h". Этот дизайн основан на философии «не платите за то, чем не пользуетесь».


Преобразование из Unix-времени в дату::year_month_day
( Томми Бандзо )

При работе с API-интерфейсами, которые предоставляют временную метку в виде простого целого числа со знаком или нет, например время Unix, и вы хотите использовать ее для вычислений хроно/даты/времени, вот что можно сделать (вместо использования time_t & местное время):

void callback(int unixtime)
{
    auto t = std::chrono::seconds(unixtime);
    std::chrono::system_clock::time_point tp(t);
    auto d = date::floor<date::days>(tp);
    date::year_month_day ymd(d);
    date::month_day md(ymd.month(), ymd.day());
}
*/
