// fig19_02.cpp 
// get, put and eof member functions.
#include <format>
using std::format;
#include <iostream>
using std::cout; using std::endl; using std::boolalpha; using std::cin; using std::cerr;
#include <iomanip>

#include <cctype>
#include <climits>
//#include <stdio.h>
#include <conio.h>
#include <windows.h>
int main() 
{
	int 			character	{}; 				// 	use int, because char cannot represent EOF
	std::string		string_of_number;				//	Строка символов - цифр десятичного числа
	// prompt user to enter line of text
	cout 	<< 	format	(	
				"Before input, cin.eof(): {}", 
										 std::cin .eof())
			<< 	"\nEnter a number followed by Enter and end-of-file:\n";

	// use getch() to read each character; use put() to display it
	
	std::cout << "Вводите цифры" << std::endl;

	int points_counter{0};
	// 	Пока не будет нажата клавиша ENTER, вводим десятичные цифры, и, возможно, только одну точку
	while ((character = getch()) != VK_RETURN) {		
		//	Отсечом на клавиатуре все клавиши, кроме цифровых, точки и backspace
		if (!((std::isdigit(character) != 0) | ('.' == character) | ('\b' == character))){
			if (std::isprint(character) != 0){
				std::cout .put (character);		// Отображение введеного АЦ-символа в строке ввода числа
			}	
			MessageBox	(	nullptr
						, 	"Вводите цифры!!!"
						, 	"Ошибка ввода"
						, 	MB_OK | MB_ICONERROR
						);
			//	От некорректного символа в строке ввода избавимся - курсор влево, символ забить, и еще раз BS
			cout << '\b' << ' ' << '\b';  		//	Нивелируем заодно ляп в g++	- два раза выводит MessageBox
		}
		else {
			std::cout .put (character);			// Отображение введеного символа в строке ввода числа			
			
			if (('.' == character)){
				++points_counter;
				if (points_counter > 1){
					MessageBox	(	nullptr
								, 	"Десятичная точка может быть только одна!!!"
								, 	"Ошибка ввода"
								, 	MB_OK | MB_ICONERROR
								);
					--points_counter;			
					//	От некорректного символа надо избавиться - курсор влево
					cout << '\b' << ' ' << '\b';
					continue;			//	Возврат в начало цикла while(){}
				}
			}	
			if (('\b' == character)){
				if(!string_of_number .empty()){
					if ('.'	==	string_of_number. back())
						--points_counter;
					string_of_number. pop_back();
				}	
				cout << ' ' << '\b';
				continue;			//	Возврат в начало цикла while(){}
			}
			
			string_of_number. push_back(static_cast<unsigned char>(character));
		}			
	}	//	end while cycle
	cout << "\nСтрока введенного числа ->  " << string_of_number << endl;
	
	double	double_number	{std::stod(string_of_number)};
	cout 	<<	"Преобразовнная строка числа в формат double ->   " 
			<< 	std::setprecision(12) 
			<< 	double_number 
			<< 	endl;
	
	// display end-of-file character
	cout 	<< std::hex << 	format	(
							"\nEOF on this system is: {:#X}\n",
													 character)
			<< 	format	(
				"After EOF input, cin.eof(): {}\n", 
											std::cin .eof());
	system("pause");
}
