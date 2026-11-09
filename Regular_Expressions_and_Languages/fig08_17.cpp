// fig08_17.cpp
// Regular expression replacements.
#include <format>
using std::format;
#include <iostream>
using std::cout; using std::endl; using std::boolalpha; using std::cin; using std::cerr;
#include <regex>
#include <string>

int main() {
	// replace tabs with commas
	std::string 	s1{"1\t2\t3\t4"};
	/*
		8.20.2. Замена подстрок 

		Заголовок <regex> предоставляет функцию regex_replace() 
		для замены фрагментов в строке. 
		
		В следующем примере мы преобразуем строку элементов, 
		разделенных табуляцией, в строку элементов, разделенных запятыми. 
		
		Функция regex_replace() получает три аргумента: 

		♦ 	строку для поиска ("1\t2\t3\t4"); 
		
		♦ 	регулярное выражение (символ табуляции "\t"); 
		
		♦ 	текст для замены (","). 
		
		Функция возвращает измененную строку. 
		
		Инструкция std::regex{"\t"} 
		создает временный объект regex, инициализирует его 
		и сразу передает в функцию regex_replace(). 
		
		Это удобно, если регулярное выражение используется только ОДИН раз. 
	*/
	cout 	<< format("Original string : {}\n", R"(1\t2\t3\t4)")
			<< format("After replacing tabs with commas: {}\n",
														std::regex_replace(s1, std::regex{"\t"}, ","));
}