#pragma once
//#ifndef MY_STD_LIBRARY_H
//#define MY_STD_LIBRARY_H
// my_std_library.h
// Language support
// #include <codecvt>
// #include <compare>
// #include <contract>
// #include <coroutine>
#include <csetjmp>
#include <csignal>
#include <cstdarg>
// #include <stdarg.h>	;	DOS
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <exception>
#include <initializer_list>
#include <limits>
#include <new>
#include <typeinfo>
// #include <version>
#include <optional>					// std::optional<>

// Localization
#include <clocale>					// Параметры стран и регионов
// #include <codecvt>
// #include <cvt/wbuffer>
// #include <cvt/wstring>
#include <locale>

// I/O and formatting
// Потоки ввод-вывод
#include <istream>					// Объект класса istream преобразовывает поток символов (байтов) 
									// в объекты, имеющие тип. 
#include <ostream>					// Объект класса ostream преобразовывает объекты, 
									// имеющие тип, в поток символов (байтов). 
#include <iostream>
using std::cout; using std::endl; using std::cerr;
#include <ios>
#include <iomanip>					// (для манипуляторов, получающих аргументы)
#include <cinttypes>
#include <cstdio>
#include <iterator>					// for ostream_iterator

// #include <filesystem>
#include <fstream>
#include <iosfwd>
#include <sstream>
#include <streambuf>
// #include <strstream>
#include <iomanip>					// Несколько манипуляторов, 
									// каждый из которых принимает один аргумент:
									// iostreams

// Multithreading - Поддержка параллельных вычислений
#include <mutex>					// Взаимоисключения 
#include <shared_mutex>																						
#include <future>					// Межпоточные коммуникации
#include <condition_variable>
#include <thread>					// Потоки
#include <atomic> 
#include <execution>					// Все, что связано с параллельными алгоритмами

// Контейнеры
#include <array>						// Массив
#include <list>						// Список (связной двунаправленный)
#include <forward_list>				// Список (связной однонаправленный)
#include <vector>					// Класс vector (динамически расширяемый)
#include <string>
#include <valarray>					// Динамический числовой массив, заточенный на вычисления
#include <map>						// Отображение - Ассоциативный контейнер (отсортированный)
#include <unordered_map>			// и не отсортированное
#include <set>						// Множество (отсортированное)
#include <unordered_set>			// и не отсортированное
#include <bitset>					// заголовочный файл битовых полей
#include <initializer_list>				// 

// Адаптеры контейнеров
#include <queue>					// Классы queue и priority_queue
#include <stack>

// Работа с числами - Math and numerics
#include <limits>				// Определен шаблонный класс numeric_limits<T>
#include <limits.h>				// Макросы, определяющие основные свойства целых чисел 
#include <climits>
#include <float.h>				// и чисел с плавающей точкой
#include <cfloat>
#include <random>				// Случайные числа 
#include <ratio>				// Дроби
// Математические функции 
#include <cmath>
#include <complex>

// Алгоритмы стандартной библиотеки
#include <algorithm>			// Алгоритмы: sort(), find() и т.д.
#include <numeric>				// Обобщенные версии распространенных операций 
								// над последовательностями числовых значений

// Работа с функциональными объектами
#include <functional>			// ref() и cref(); std::reference<>; std::wrapper<>; std::function<>
#include <any>				// std::any, 	std::any_cast
// Вспомогательные компоненты
#include <utility>				// Класс pair<T, U> - пара и другие, а также
								// Функции и операторы стандартной библиотеки C++, 
								// которые помогают создавать пары объектов и управлять ими
#include <tuple>				// Класс tuple<...> - кортеж

// Управление памятью - Memory management
#include <memory>			// Указатели shared_ptr<>, 
								// weak_ptr<>, bad_weak_ptr<> и enable_shared_from_this<>;
								// unique_ptr<>
// #include <allocators>
// #include <memory_resource>
#include <scoped_allocator>

// Работа со строками - Strings and character data
#include <cctype>					// Операции классификации символов
#include <string>					// Соответствующие операции со строкми
#include <cstring>					// Поддержка С - строк
#include <string_view>
// Regular expressions
#include <regex>					// Регулярные выражения

// Работа со временем (Time)
#include <chrono>					// Средства для измерения времени
#include <ctime>
#include <time.h>

// Шаблоны
#include <type_traits>				// Часть библиотеки поддержки типов (свойств типов).
#include <ppltasks.h>
// Исключения
/*
Стандартные классы исключений можно разделить на три категории,
1. Языковая поддержка.
2. Логические ошибки.
3. Ошибки времени выполнения.
*/
// 1. Классы исключений для поддержки языка
#include <typeinfo>			// bad_cast (ген. dynamic_cast, use_facet()), 
							// bad_typeid (ген. typeid) 
#include <exception>		// bad_exception (ген. unexpected() )
// 2. Классы исключений для логических ошибок 
// обычно являются производными от класса logic_error
#include <stdexcept>		// invalid_argument
							// length_error
							// out_of_range
							// domain_error

#include <future>			// future_error
// 3. Классы исключений для ошибок времени выполнения
#include <stdexcept>		// system_error
							// range_error
							// overflow_error
							// underflow_error
							// bad_alloc
							// bad_array_new_length (ген. опреатором new)
#include <new>				
#include <memory>			// bad_weak_ptr
#include <functional>		// bad_function_call
#include <ios>				// ios_base::failure
#include <system_error>		// system_error

#include <exception>		// exception и badexception
							// объекты типа exception_ptr
// experimental library - похоже, она не в полном объеме реализована
// Даже, если файлы и есть в каталоге \experimental
// компилятор ругается
//#include <experimental/filesystem>
//#include <experimental/future>


// etc.
#include <windows.h>
#include <cassert>
 
#include <dos.h>

//#include <cxxabi.h>					// abi::__cxa_demangle(mangled_name, NULL, NULL, &status); 
											// Изменение имени - https://ru.qaz.wiki/wiki/Name_mangling

//#endif
