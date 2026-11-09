//	Стандартная библиотека
#include "my_std_library.h"
using namespace std;
/*
	Тип лямбда-выражения

	higher order lambda expresion - лямбда-функции высшего порядка

	Многие языки программирования поддерживают концепцию функции более высокого порядка.
	Функция высшего порядка представляет собой лямбда-выражение, 
	которое принимает другое лямбда-выражение в качестве аргумента 
	или возвращает лямбда-выражение. 
	Можно использовать класс Function , чтобы разрешить C++ лямбда-выражение 
	работать как функция высшего порядка.

*/

/*
  *     ********************************************************************
  *     Основная функция
  *     ********************************************************************
  */
void higher_order_lambda()
{
	cout << '\n' << "Работает <higher_order_lambda()>  " << '\n' << endl;

	/*
	  *     В следующем примере показано лямбда-выражение, 
	  *     которое возвращает объект function, и лямбда-выражение, 
	  *     которое принимает объект function в качестве аргумента.
	  */
	// The following code declares a lambda expression that returns another lambda expression that adds two numbers.
	// The returned lambda expression captures parameter x by value.
	// Это лямбда-функция низшего порядка, она имеет сигнатуру <int(int)>
	auto addtwointegers = [](int x) -> function<int(int)> 
								{
									cout << "Я - лямбда низшего порядка :(" << endl;
									cout << "Параметр внешней лямбда " << x << endl;
									return [=](int y) 
											{ 
												cout << "Я - вложенная лямбда лямбды низшего порядка :(" << endl;
												cout << "Параметр вложенной лямбда " << y << endl;
												return x + y; 
											};
								};

	// The following code declares a lambda expression that takes another lambda expression as its argument.
	// The lambda expression applies the argument z to the function f and multiplies by 2.
	// Это лямбда-функция высшего порядка, она имеет сигнатуру <int(int), int>
	auto higherorder = [](const function<int(int)>& f, int z) 
							{
								return f ( z ) * 2;
							};

	// Call the lambda expression that is bound to higherorder.
	// Второй (правый) аргумент буде передан внутренней (вложенной) лямбда объекта низшего порядка
	// а первый аргумент достанется внешней лямбда объекта низшего порядка в соответствии
	// с ее сигнатурой
	auto answer = higherorder ( addtwointegers(7), 8 );

	// Print the result, which is (7+8)*2.
	cout << "answer = higherorder ( addtwointegers(7), 8 ) == " << answer << endl;

	/*
	  *	Попробуем сконструировать трехуровневую структуру лямбда-выражени низшего порядка 
	  */
	  // Это лямбда-функция низшего порядка, она имеет сигнатуру function<int(function<int(int)>)>
	/*
	auto fibonacci = 
	[](auto x) -> function<int(function<int(int)>)>
	{
		cout << "Я - лямбда низшего порядка :(" << endl;
		cout << "Параметр лямбда 1-го уровня " << x << endl;
		return x + [](auto y) ->function<int(int)>
					{
						cout << "Параметр лямбда - 2 уровня " << y << endl;
						return y + [](auto z)->int
									{
										cout << "Параметр лямбда - 3 уровня " << z << endl;
										return z;
									};
					};
	};
	*/

}
/*
 Примечания:
 1. Документация Microoft
https://docs.microsoft.com/ru-ru/cpp/cpp/examples-of-lambda-expressions?view=vs-2019
2.
*/