#pragma once
/*
	separate_number_with_apostrophe.hpp
*/
#include <locale>
#include <string>
#include <iostream>
using std::cout; using std::endl; 
/* 	
	Надо бы доработать класс в отношении RAII 
	
	- 	добавить конструктор с параметрами
	
	-	добавить деструктор с включенным в него методом восстановления локали
*/	
class separate_number_with_apostrophe
{
private:
	template<int GROUP = 3>
	struct space_out : std::numpunct<char> 
	{
		char			do_thousands_sep()		const { return separator_code; }  	// separate with apostrophe
		std::string		do_grouping()			const 
		{ 
			switch (GROUP)
			{
			case 4:	
				return "\4"; 
			default:
				return "\3";
			}	
		}	// groups of 3 or 4 digit
		// ~space_out(){ cout << "Не волнуйтесь, меня убили! :)" << endl; }
	};
public:			
	template<int GROUP = 3>
	void set_separator()	{	
		if (flag_set_separator == true){
			locale_restore();
		}	
		flag_set_separator = true;
		old_locale = cout.imbue(std::locale(std::cout.getloc(), new space_out<GROUP>));	
	};
	void locale_restore()	
	{ 	
		flag_set_separator = false;
		cout.imbue(old_locale);														
	};
private:
	bool						flag_set_separator	=	false;	
	// Нельзя два раза подряд устанавливать локаль без промежуточного сброса
	
	static constexpr char 		separator_code		= 	0x27;	// Код апострофа
	
	// Для отключения апострофа как разделителя десятичных или шестнадцатеричных разрядов
	std::locale 				old_locale;				
};