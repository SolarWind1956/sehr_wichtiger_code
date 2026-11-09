//	Seg_23_8_6_Character_sets_and_ranges.cpp
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
					);
//------------------------------------------------------------------------------
void Seg_23_8_6_Character_sets_and_ranges()
{
	cout << "-------------- I'm void " <<  __func__  << "()" << endl;
	
	std::string		line			{};						//	Строка для поиска соответствия
	std::string		matches			{};						//	Строки, найденные по шаблону
	std::string		str_for_regex	{};						//	Строка-шаблон регулярного выражения
	
	line			=	"zzz TX23456-3456sss ggg TX33456-1234";
	str_for_regex	=	R"(\w{2}\s*\d{5}(-\d{4})?)";	// 	Необработанный Raw (сырой) литерал
    str_accord_pattern	(line,	matches, str_for_regex);
	/*
		Шаблон для непустой строки буквенно-цифровых символов, 
		заключенной в двойные кавычки, 
		может выглядеть так: 
	*/	
	str_for_regex	=	"[[:alnum:]]+"; 
	str_accord_pattern	(line,	matches, str_for_regex);
	str_accord_pattern	(line,	matches, "[[:alnum:]]{4}");
	/*
		Чтобы поместить это реrулярное выражение в строковый литерал, 
		к кавычкам надо добавить обратные косые черты, сделав их управляющими символами: 
		
		string s = "\"[[:alnum:]]+\"; 
	*/
	line			=	"\"zzzzz\" TX23456-3456sss \"ggg\" TX33456-1234";
	str_accord_pattern	(line,	matches, "\"\\w+\"");
	str_accord_pattern	(line,	matches, "\"[[:alnum:]]+\"");
	/*
		Более того, чтобы поместить этот строковый литерал в объект класса std::regex, 
		следует сделать управляющими символами не только кавычки, 
		но и саму обратную косую черту: 
		
		reqex s {"\\\"[[:alnum:]]+\\\""}; 
	*/
	str_accord_pattern	(line,	matches, "\\\"[[:alnum:]]+\\\"");
	str_accord_pattern	(line,	matches, "\\\"\\w+\\\"");
	/*	
		Применение необработанных строковых литералов существенно упрощает жизнь программиста: 
		
		reqex s2 {R"("[[:alnum:]]+")"}; 
	*/
	str_accord_pattern	(line,	matches, R"("[[:alnum:]]+")");
	
	//	------------------------------------------------------------------------------------------
	
}		
#include "str_accord_pattern.cpp"