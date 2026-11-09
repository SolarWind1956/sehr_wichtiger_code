//	more_about_regex.cpp
//
#include <regex>
#include <iostream>
#include <stdexcept>
#include <exception>
#include <string>
#include <fstream>
using namespace std;
bool	
str_accord_pattern	(	const	std::string&	line 			//	Строка для поиска фрагмента в по шаблону
					,			std::string&	matches_str		//	Найденные по regex-шаблону строки
					,	const	std::string&	str_for_regex	//	Строка-шаблон регулярного выражения
					);
void Seg_23_8_6_Character_sets_and_ranges();			
//------------------------------------------------------------------------------
int main()
try
{
	cout << "-------------- I'm void " <<  __func__  << "()" << endl;
	Seg_23_8_6_Character_sets_and_ranges();
	
}
/*
Этот прибамбас есть в библиотеке boost. В стандартной библиотеке пока этого исключения нет.

catch (std::bad_expression e) {
	cerr << "reg expression error: " << e.what() << '\n'; 
    return 1;
}
*/
catch (std::exception& e) {
    cerr << "error: " << e.what() << '\n'; 
    return 1;
}
catch (...) {
    cerr << "Oops: unknown exception!\n"; 
    return 2;
};

//------------------------------------------------------------------------------
//#include "str_accord_pattern.cpp"
#include "Seg_23_8_6_Character_sets_and_ranges.cpp"