//	Стандартная библиотека
#include "my_std_library.h"
#include <tuple>
using namespace std;

/*
	Тип лямбда-выражения

	Variadic - универсальный лямбда;

	Variadic Template - Вариативный шаблон или шаблон с переменным числом аргументов, 
	заранее неизвестным числом аргументов, которые формируют один или несколько 
	так называемых пакетов параметров.

	Пакет параметров шаблона (англ. template parameter pack) - параметр шаблонной функции или класса, 
	состоящий из произвольного количества разнородных элементов. 
	Фактически представляет из себя краткую запись 
	гетерогенного набора аргументов вариативного шаблона, 
	определяемую с помощью многоточия...  (ellipsis ... ).

	Этот механизм  можно применить к как к шаблонам классов, так и к шаблонам функций. 

	Шаблоны с переменным числом параметров предоставляют средство создания шаблонных функций 
	и шаблонных классов, которые принимают переменное количество аргументов.

	В шаблонах с переменным числом аргументов многоточие используется двумя способами. 
		- слева от имени параметра обозначается пакет параметров, 
		- справа от имени параметра - пакеты параметров разворачиваются в отдельные имена.

	template< typename... Arguments > 
	class class_name;                                       // Класс class_name может принять любое число входных параметров.

	Обратите внимание, что используется соглашение, согласно которому
	многоточие примыкает к имени типа - typename.

	template < typename... Arguments > 
	return_type function_name ( Arguments... args );         // функция  может принять любое число параметров.

	Лямбда-выражение может также принимать заранее неизвестное количество параметров

	template<class... Args>
	void f(Args... args) 
	{
		auto x = [args...] 
					{ 
						return g ( args... );
					};
		x();
	}
*/

// Необхоимая перегруженная функция
void print() {}

template < typename First, typename... Rest >
void print ( const First& first, Rest&&... args )
{
	std::cout << first << std::endl;
	print ( args... );
}

template<typename... T>
void foo( T&& ...pars ) 
{
	auto bar = [] ( auto&& ...pars ) 
				{ 
					return sizeof...( pars );  
				};

	auto lbd = [ args = forward_as_tuple ( pars... ), bar ]() 
				{
					return apply ( bar, args );
				};

	cout << "Size of the parameters pack: " << lbd() << endl;
}


/*
  *     ********************************************************************
  *     Основная функция
  *     ********************************************************************
  */
void variadic_universal_lambda()
{
	cout << '\n' << "Работает <variadic_universal_lambda()>  " << '\n' << endl;
       
	cout << "Первый пример" << endl;
	auto variadic_generic_lambda = []( auto... param ) 
	{
		print ( param... );
	};

	variadic_generic_lambda ( "Первый аргумент для печати", 7, "lol", 5.73 );

	cout << '\n' << "Второй пример" << endl;
	int			a = 1;
	double		b = 3.0;
	string		c = "ololo";
	char		d = 'h';

	foo ( a, b, c, d, 77, vector<int>(3) );
}

/*
Примечания.
1. Ссылки на сайты:
https://ru.wikipedia.org/wiki/Вариативный_шаблон
https://docs.microsoft.com/ru-ru/cpp/cpp/ellipses-and-variadic-templates?view=vs-2019

*/
