
//
// This is example code from Chapter 23.9 "Matching with regular expression" of
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//

#include "std_lib_facilities.h"
//------------------------------------------------------------------------------
struct bad_from_string : std::bad_cast
// class for reporting string cast errors
{
    const char* what() const throw() // override bad_cast's what()
    {
        return "bad cast from string";
    }
};

//------------------------------------------------------------------------------
template<class T> 
T 	from_string(const string& s)
{
    istringstream 	is(s);
    T 				t;
    if (!(is >> t)) 
		throw bad_from_string();
    
	return t;
}

//------------------------------------------------------------------------------
int main()
try
{
    ifstream 	in("table.txt");    	// input file
    if (!in) 
		error("no input file");

    string 	line;            		// input buffer
    int 	lineno = 0;
	/*
		Теперь можем перейти к верификации данных в таблице. 
		
		Мы подсчитаем количество мальчиков ("drenge") и девочек ("piger"), 
		учащихся в школе. 
		
		Для каждой строки мы проверяем, действительно ли в последнем поле ("ELEVER IALТ") 
		записана сумма первых двух полей. 
		Последняя строка ('Alle klasser") содержит суммы по столбцам. 
		
		Для проверки этого факта модифицируем выражение 'row' так, 
		чтобы текстовое поле имени класса или итоговой строки 
		содержало совпадение с 'подшаблоном' matches[1],
		и можно было распознать строку "Alle klasser". 
		
		matches[0]	-	это вся строка - весь шаблон
	*/
	cout	<< "Проверка формата заголовка" << endl;
    regex header	("^[\\w ]+(	[\\w ]+)*$");
    if (getline(in, line)) {        // check header line
        std::smatch matches;
        if (!std::regex_search(line, matches, header)) {
            error("no header");
        }
    }

    // column totals:
    int boys 		= 0;
    int girls 		= 0;

    // current line values:
    int curr_boy 	= 0;
    int curr_girl 	= 0;
	int curr_total	= 0;
	
	cout	<< "Проверка форматов строк данных" << endl;
	
	regex row		("^([\\w ]+)(	\\d+)(	\\d+)(	\\d+)$");
    while (getline(in, line)) {    // extract and check data
        ++lineno;
        std::smatch 	matches;
        if (!regex_match(line, matches, row)) 
            error("bad line", lineno);
		//cout	<< "Маркер строки " << matches[0] << endl;	вся строка
		//cout	<< "Маркер строки " << matches[1] << endl;	левый столбец с названием класса
        // check row: проверяем строки по остальым группам (подшаблонам)
        curr_boy 	= from_string<int>(matches[2]);
        curr_girl 	= from_string<int>(matches[3]);
        curr_total 	= from_string<int>(matches[4]);

        if (curr_boy + curr_girl != curr_total) 
			error("bad row sum");
		
		cout	<< "Маркер строки - название класса или итоговая строка: " << matches[1] << endl;
		
		//	Последняя итоговая строка таблицы
		if (matches[1] == "Alle klasser") { // last line
			cout	<< "Проверка результатов суммирования столбцов данных" << endl;
			if (curr_boy != boys) 
				error("boys don't add up\n");
			if (curr_girl != girls) 
				error("girls don't add up\n");
			if (!(in >> ws).eof()) 
				error("characters after total line");
		}
		//	Получаем суммы по столбцам с нарастающим итогом
        boys 	+= curr_boy;
        girls 	+= curr_girl;
    }

	cout	<< "Повторная проверка результатов суммирования столбцов данных" << endl;
    // check columns: с учетом результатов суммирования строк в последней строке
    if (2 * curr_boy  != boys)  
		error("boys don't add up");
    if (2 * curr_girl != girls) 
		error("girls don't add up");
	/*
		Последняя строка по смыслу отличается от остальных: в ней содержатся итоговые суммы. 
		
		Мы распознаем ее по метке ("Alle klasser"). 
		
		Мы решили, что после последнего символа не должны стоять символы, 
		не являющиеся пробельными 
		(для распознавания этого факта используется методика из функции to<>(); см. раздел 23.2), 
		и выдаем сообщение об ошибке в случае их обнаружения. 
		
		Для того чтобы извлечь числа из полей данных, 
		мы использовали функцию from_string() из раздела 23.2. 
		
		Мы уже убедились. что эти поля содержат только цифры, 
		поэтому проверять правильность преобразования 
		объекта класса string в переменную типа int 
		необязательно. 
	*/
}
catch (std::exception& e) {
    cerr << "error: " << e.what() << '\n'; 
    return 1;
}
catch (...) {
    cerr << "Oops: unknown exception!\n"; 
    return 2;
}
//------------------------------------------------------------------------------
