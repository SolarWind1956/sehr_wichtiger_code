#pragma once
/*
Ch_14_Or_16_Time_Point_Duration_and_Clock.hpp

Rainer Grimm. 	Concurrency with Modern C. What every professional C++ programmer should know about concurrency. 2022
Райнер Гримм. 	Параллельное программирование на современном языке C++ 
 
	PART 		LAST.			Further Information
	Chapter 	16. 			The Time Library
	Paragraph	16.1. 			The Interplay of Time Point, Time Duration, and Clock
	
	Часть 		Последняя.		ДОПОЛНИТЕЛЬНЫЕ СВЕДЕНИЯ
	Глава		14. 			Библиотека для работы со временем
	Параграф 	14.1.			Взаимосвязь моментов, промежутков времени и часов
*/
#include "my_short_headers_list.hpp"
/*
	******************************	14.1.	Взаимосвязь моментов, промежутков времени и часов
	* или в английском варианте:
	******************************	16.1. 	The Interplay of Time Point, Time Duration, and Clock
	
	Момент времени (Time point)				определяется точкой отсчёта – 
											так называемым началом эпохи (Epoch) –	
											и промежутком времени, отсчитываемым от начала эпохи.

	Промежуток времени (Time duration)	–	это промежуток между двумя моментами времени. 
											Он измеряется количеством определённых единиц.

	Часы (Clock)							характеризуются "начальной точкой" 
											и "единицей измерения" (time tick - тиках). 

											Они позволяют определить текущий момент времени 
											как промежуток от начальной точки, 
											измеренный в данных единицах - тиках.

	Моменты времени можно между собой сравнивать. 
	
	Также можно к моменту времени 
	прибавить промежуток и 
	получить другой момент времени.

	"Единица измерения" определяет "точность", 
	с которой часы могут измерять промежутки времени. 
	
	Например, в нашей традиции за начало эпохи берут год предполагаемого рождения Христа; 
	для многих практических задач подходящей единицей измерения является год.

	**************************		14.2. Моменты времени
	
	Моменты времени моделируются объектами типа std::chrono::time_point.

	Это шаблон с двумя параметрами. 
	
	Первый параметр задаёт тип часов. 
	
	Второй параметр – тип промежутка времени – необязательный: 
	по умолчанию тип промежутка берётся из типа часов.

	Шаблон класса std::chrono::time_point

	template<class Clock, class Duration = typename Clock::duration>
	class time_point;
	
	С часами связаны четыре особых момента времени:

	•	epoch	–	точка отсчёта часов (starting point);

	•	now		–	текущий момент времени (current time);

	•	min		–	наименьший (наиболее давний) момент времени, 
					который может быть измерен этими часами;

	•	max		–	наибольший момент времени, который может быть измерен этими часами.

	"Точность" (accuracy) наименьшего и наибольшего момента времени 
	может быть различна у разных часов. 
	
	В стандартной библиотеке имеются часы
	
	-	std::chrono::system_clock, 
	
	-	std::chrono::steady_cock 
	
	и 
	
	-	std::chrono::high_resolution_clock.

	Стандарт языка C++ не даёт гарантий относительно 
	точности, точки отсчёта и диапазона (valid time range) часов. 
	
	За начало отсчёта часов std::chrono::system_clock обычно берётся 1 января 1970 года – 
	начало эпохи UNIX. 
	
	Как явствует из названия, 
	часы std::chrono::high_resolution_clock имеют наибольшую точность.

	***********************		14.2.1. Перевод моментов времени в календарный формат
	
	У часов std::chrono::system_clock есть функция to_time_t(), 
	которая позволяет преобразовывать моменты времени, отмеренные с помощью этих часов, 
	в значения типа std::time_t - количество секунд от эпохи UNIX. 
	
	Хотя это и не определено, почти всегда это целочисленное значение, 
	содержащее количество секунд (не считая дополнительных секунд) с 00:00 1 января 1970 года 
	по всемирному координированному времени, что соответствует "времени POSIX".

	Время Unix — это представление даты и времени, широко используемое в вычислениях. 
	
	Оно измеряет время по количеству секунд, прошедших с 00:00:00 UTC 1 января 1970 года, 
	эпохи Unix, без корректировок, внесенных из-за дополнительных секунд. 
	
	В современных вычислениях значения иногда хранятся с более высокой степенью детализации, 
	например, в микросекундах или наносекундах.

	Время Unix возникло как системное время операционных систем Unix. 
	
	Оно стало широко использоваться в других компьютерных операционных системах, 
	файловых системах, языках программирования и базах данных.

	Это значение, в свою очередь, можно преобразовать функцией std::gmtime() в "календарное время",
	выраженное в стандарте UTC - Coordinated Universal Time (всемирное координированное время). 
	
	Наконец, это календарное время можно передать функции std::asctime(), 
	чтобы получить текстуальное представление даты и времени.

	Отображение даты и времени в текстовом виде
*/
int	t_time_to_string(std::time_t clock, string& time_string)
{
	struct tm	newtime;
	__time64_t	aclock = static_cast<__time64_t>(clock);
	char		buffer[64];
	errno_t		errNum;
	
	_time64(&aclock);						// Get time in seconds.
	_localtime64_s(&newtime, &aclock);		// Convert time to struct tm form.

	errNum = asctime_s(buffer, 64, &newtime);
	if (errNum)
	{
		printf("I am t_time_to_string() - Error code: %d", (int)errNum);
		return 1;
	}
	time_string		=	std::string(buffer);
	return	0;
}
int	time_point_test() {
	cout << "\n------------------ I'am time_point_test()" << endl;
	string		time_string;
	
	std::chrono::time_point<std::chrono::system_clock>		sysTimePoint;

	std::time_t		tp = std::chrono::system_clock::to_time_t(sysTimePoint);

	t_time_to_string(tp, std::ref(time_string));
	cout << "Epoch: " << time_string << endl;
	
	tp				=	std::chrono::system_clock::to_time_t(sysTimePoint.min());
	t_time_to_string(tp, std::ref(time_string));
	cout << "Time min: " << time_string << endl;

	tp				=	std::chrono::system_clock::to_time_t(sysTimePoint.max());
	t_time_to_string(tp, std::ref(time_string));
	cout << "Time max: " << time_string << endl;

	sysTimePoint	=	std::chrono::system_clock::now();
	tp				=	std::chrono::system_clock::to_time_t(sysTimePoint);
	t_time_to_string(tp, std::ref(time_string));
	cout << "Time now: " << time_string << endl;

	return	0;
}

void  Ch_14_Or_16_Time_Point_Duration_and_Clock()
{
	cout << "\n------------------ I'am Ch_14_Or_16_Time_Point_Duration_and_Clock()" << endl;
	
	time_point_test();

}	