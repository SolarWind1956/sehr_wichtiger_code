// fig08_18.cpp
// Matching patterns throughout a string.
#include <format>
using std::format;
#include <iostream>
using std::cout; using std::endl; using std::boolalpha; using std::cin; using std::cerr;
#include <regex>
#include <string>

int main() {
	// performing a simple match
	std::string 	s1{"Programming is fun"};
	cout << format("s1: {}\n\n", s1);
	/*
		8.20.3. Поиск в тексте по шаблонам 
		
		Вы можете искать в тексте интересующие вас фрагменты с помощью функции regex_search(), 
		которая возвращает true, если какой-либо фрагмент строки соответствует шаблону. 
		
		Эта функция также может предоставить вам 
		доступ к найденному фрагменту через объект шаблона класса match_results, 
		который вы передаете в качестве аргумента. 
		
		Существуют псевдонимы match_results для разных типов строк: 
		
		♦ 	для поиска в std::strings используйте smatch (произносится как "эс мач"); 
		
		♦ 	для поиска в строках в стиле С и в строковых литералах используйте cmatch 
			(произносится как "си мач"). 
	
		На данный момент заголовок <regex> 
		не содержит средств для работы с представлениями std::string_view. 
	*/
	cout << "Search anywhere in s1:\n"
		<< format	("Programming: {}; fun: {}; fn: {}\n\n",
								  std::regex_search(s1, std::regex{"Programming"}),
										  std::regex_search(s1, std::regex{"fun"}),
												  std::regex_search(s1, std::regex{"fn"}));

	// ignoring case
	std::string 	s2{"SAM WHITE"};
	std::smatch 	match; 				// store the text that matches the pattern
	cout << format("s2: {}\n\n", s2);
	
	cout 	<< "Case insensitive search for Sam in s2:\n"
			<< format	("Sam: {}\n", std::regex_search	(	s2
														, 	match
														,	std::regex{"Sam", std::regex_constants::icase}
														)
						)
			<< 	format	("Matched text: {}\n\n", match.str());
	/*
		Константы std::regex_constants из заголовка <regex> 
		позволяют настраивать параметры поиска. 
		
		Например, по умолчанию поиск чувствителен к регистру, 
		но константа regex_constants::icase 
		позволяет искать соответствия без учета регистра. 
	*/
	// finding all matches
	std::string 	contact	{
								"Wally White, Home: 555-555-1234, Work: 555-555-4321"
							};
	std::regex 		phone{R"(\d{3}-\d{3}-\d{4})"};

	cout 	<< format("Finding phone numbers in:\n{}\n", contact);
	
	while (std::regex_search(contact, match, phone)) {
		cout << format("	{}\n", match.str());
		contact = match.suffix();
	}
}
