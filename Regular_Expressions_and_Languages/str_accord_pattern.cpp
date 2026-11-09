//	more_about_regex.cpp
//
#include <regex>
#include <iostream>
#include <stdexcept>
#include <string>
#include <fstream>
using namespace std;
bool	
str_accord_pattern	(	const	std::string&	line 			//	Строка для поиска фрагмента в по шаблону
					,			std::string&	matches_str		//	Найденные по regex-шаблону строки
					,	const	std::string&	str_for_regex	//	Строка-шаблон регулярного выражения
					)
{	
	cout << "-------------- I'm bool " <<  __func__  << "()" << endl;
	cout	<< 	"Исходная строка для поиска фрагментов по шаблону:            "	<<	line 			<< endl;
	cout	<<	"Строка для инициализации шаблона регулярного выражения:      "	<<	str_for_regex 	<< endl;
	/*
		Шаблон 'regex_patrn' является разновидностью строки, 
		так что мы инициализируем его строкой. 
		
		Здесь мы использовали необработанный строковый литерал. 
		
		Однако std::regex - это не только строка, 
		но и некоторый сложный механизм сопоставления строки шаблону, 
		который скрыто создается 
		при инициализации объекта типа std::regex нашим шаблоном 
		для почтового индекса (или при присваивании ему этого шаблона) 
		и который выходит за рамки нашей книги. 
		
		Однако после инициализации объекта std::regex строкой-шаблоном 
		его можно  применять к каждой строке нашего файла. 
	*/
	std::regex 		regex_patrn{str_for_regex};
	std::smatch 	matches;								
	if (std::regex_search	(	line			
							, 	matches			
							, 	regex_patrn		
							)
		)
	/*
		Функция regex_search(line, matches, regex_patrn) ищет в строке 'line' все, 
		что соответствует регулярному выражению, хранящемуся в объекте regex_patrn, 
		и если она находит какое-либо соответствие, то сохраняет его в объекте matches. 
		
		Естественно, если соответствие не обнаружено. 
		функция regex_search(line, matches, regex_patrn) возвращает значение false.
	*/
	{
		cout	<<	"Фрагменты исходной строки, соответствующие шаблону:          "	<<	endl;
		for (auto& elem : matches)
			cout << elem << endl;
	
		matches_str		=	matches[0];
		
		return	true;
	} else	{
		cout	<< "Фрагмент по шаблону не найден" << endl;
		return	false;
	}	
}
//------------------------------------------------------------------------------
