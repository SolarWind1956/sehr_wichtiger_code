//#include "my_std_library.h"
//using namespace std;
#include <iostream>
using std::cout; using std::cerr; using std::endl; using std::boolalpha;
#include <iomanip>
using std::setw;

#include <valarray>
using std::valarray;

// Исключения
/*
Стандартные классы исключений можно разделить на три категории,
1. языкова¤ поддержка.
2. Логические ошибки.
3. Ошибки времени выполнени¤.
*/
// 1.  Классы исключений для поддержки языка
//#include <typeinfo>		// bad_cast (ген. dynamic_cast, use_facet()), 
						// bad_typeid (ген. typeid) 
#include <exception>		// bad_exception (ген. unexpected() )
// 2.  Классы исключений для логических ошибок 
// обычно являются производными от класса logic_error
//#include <stdexcept>		// invalid_argument
						// length_error
						// out_of_range
						// domain_error

#include <future>			// future_error
// 3.  Классы исключений для ошибок времени выполнени¤
//#include <stdexcept>		// system_error
						// range_error
						// overflow_error
						// underflow_error
						// bad_alloc
						// bad_array_new_length (ген. опреатором new)
#include <new>				
#include <memory>			// bad_weak_ptr
#include <functional>		// bad_function_call
#include <ios>			// ios_base::failure
#include <system_error>	// system_error

#include <exception>		// exception и badexception
						// объекты типа exception_ptr
/*
Таблица 5.1. Стандартные классы исключений, определенные в заголовке stdexcept

exception 				Наиболее общий вид проблемы

runtime_error 			Проблема, которая может быть обнаружена только во время выполнения

range_error 			Ошибка времени выполнения: полученный результат превосходит допустимый диапазон значения

overflow_error 		Ошибка времени выполнения: переполнение регистра при вычислении

underflow_error 		Ошибка времени выполнения: недополнение регистра при вычислении

logic_error 			Ошибка в логике программы

domain_error 			Логическая ошибка: аргумент, для которого не существует результата

invalid_argument 		Логическая ошибка: неподходящий аргумент

length_error 			Логическая ошибка: попытка создать объект большего размера, чем максимально допустимый для данного типа

out_of_range 			Логическая ошибка: используемое значение вне допустимого диапазона

*/

template <typename T>
void process_code_exception(const T& e)
{
	auto c = e.code();
	cout << endl;
	cout << "category: " << c.category().name() << endl;
	cout << "value: " << c.value() << endl;
	cout << "msg: " << c.message() << endl;

	cout << endl;
	cout << "def category: " << c.default_error_condition().category().name() << endl;
	cout << "def value: " << c.default_error_condition().value() << endl;
	cout << "def msg: " << c.default_error_condition().message() << endl;
}

void main_exception_test()
{
	cout << " \n";
	cout << "Ќачинаем фундаментально изучать исключени¤ \n" << endl;
	valarray<double> my_dbl_arr{ 123.2, 32.44, 0.23, 56.23, 25.15 };

	auto b_of_my_dbl_arr = std::begin(my_dbl_arr);
	auto e_of_my_dbl_arr = std::end(my_dbl_arr);

	try 
	{
		//e_of_my_dbl_arr++;

		if (e_of_my_dbl_arr > std::end(my_dbl_arr))
			// throw std::out_of_range ("out_of_range (somewhere, somehow)");
			/*
				system_error( std::error_code ec, const char* what_arg );
				или
				system_error( int ev, const std::error_category& ecat,
										const std::string& what_arg);

				ec			-	error code
				ev			-	underlying error code in the enumeration associated with ecat
				ecat		-	the category of error
				what_arg	-	explanatory string
			*/
			//throw std::system_error( EINVAL, std::generic_category(), "out_of_range (somewhere, somehow)" );
			throw std::system_error(std::make_error_code(std::errc::invalid_argument), "out_of_range (somewhere, somehow) - за границами массива");
		std::sort(b_of_my_dbl_arr, e_of_my_dbl_arr);

	}
	catch (const std::system_error & exept)
	{
		cout << "SYSTEM_ERROR: " << exept.what() << endl;
		cout << endl;
		cout << "def category: " << exept.code().default_error_condition().category().name() << endl;
		cout << "def value: " << exept.code().default_error_condition().value() << endl;
		cout << "def msg: " << exept.code().default_error_condition().message() << endl;

		// process_code_exception( exept );
	}
	catch (const std::exception & exept)
	{
		/*
		class exception
			public:
				virtual const char* what() const noexcept;
			...
		*/
		cout << "EXCEPTION: " << exept.what() << endl;

	}
	catch (...)
	{
		cout << "EXCEPTION: деление на ноль"  << endl;
	}

	for (auto i = 0; i != my_dbl_arr.size(); i++)
		cout << my_dbl_arr[i] << endl;

	for (auto elem : my_dbl_arr)
		cout << elem << endl;
}
int f_may_throw_anything();              		// 	Функция f() может генерировать любые исключения

//int g_may_throw_integer() throw(true);   		// 	Функция g() может генерировать исключения для целых типов
	//	error: ISO C++17 does not allow dynamic exception specifications

int h_not_allowed_throw_anything() throw();  	// 	Функции h() не разрешается генерировать какие-либо исключения

//	Оператор `noexcept` рассматривается только по отношению к объявлению функции:
static_assert(!noexcept(f_may_throw_anything()), "");   		// 	OK: может генерировать любые исключения

//static_assert(!noexcept(g_may_throw_integer()), "");   		// 	OK: может генерировать исключения для типа int
	//	error: 'g_may_throw_integer' was not declared in this scope

static_assert( noexcept(h_not_allowed_throw_anything()), ""); 	// 	OK: не может генерировать никакие исключения
/*
	-------------------------------------------
	int main()
	-------------------------------------------
*/
int main() {
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	main_exception_test();
	
	system("pause");
}