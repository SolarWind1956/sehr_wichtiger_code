#include "my_std_library.h"
#include "my_process_of_exeptions.h"

// »сключени¤
/*
—тандартные классы исключений можно разделить на три категории,
1. языкова¤ поддержка.
2. Ћогические ошибки.
3. ќшибки времени выполнени¤.
*/
// 1.  лассы исключений дл¤ поддержки ¤зыка
#include <typeinfo>			// bad_cast (ген. dynamic_cast, use_facet()), 
							// bad_typeid (ген. typeid) 
#include <exception>		// bad_exception (ген. unexpected() )
// 2.  лассы исключений дл¤ логических ошибок 
// обычно ¤вл¤ютс¤ производными от класса logic_error
#include <stdexcept>		// invalid_argument
							// length_error
							// out_of_range
							// domain_error

#include <future>			// future_error
// 3.  лассы исключений дл¤ ошибок времени выполнени¤
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
	
using namespace std;
/*
	Не для всех видов прерывание...
	std::system_error - да, канает
	std::future_error - да, канает
	
	Для остальных видов - вопрос!!!
	Надо смотреть каждый класс, для всех, видимо, есть только what()
	
*/
template <typename T>
void process_code_exception(const T& e)
{
	auto c = e.code();
	cout << endl;
	cout << "category: "	<< c.category().name()	<< endl;
	cout << "value: "		<< c.value()				<< endl;
	cout << "msg: "		<< c.message()			<< endl;

	cout << endl;
	cout << "def category: "	<< c.default_error_condition().category().name()	<< endl;
	cout << "def value: "		<< c.default_error_condition().value()				<< endl;
	cout << "def msg: "		<< c.default_error_condition().message()			<< endl;
};
//
void my_process_of_exeptions()
{
	cout << "<my_process_of_exeptions()>" << endl;
	try
	{
		throw; 	// повторно генерируем исключение чтобы обработать его здесь
	}
	catch (const ios_base::failure& e)
	{
		cerr << "I/O EXCEPTION: " << e.what() << endl;
		//process_code_exception(e);
	}
	catch (const std::system_error& e)
	{
		cerr << "SYSTEM EXCEPTION: " << e.what() << endl;
		process_code_exception(e);
	}
	catch (const std::future_error& e)
	{
		cerr << "FUTURE EXCEPTION; " << e.what() << endl;
		process_code_exception(e);
	}
	catch (const std::bad_alloc& e)
	{
		cerr << "BAD ALLOC EXCEPTION: " << e.what() << endl;
	}
	catch (const std::exception& e)
	{
		cerr << "EXCEPTION: " << e.what() << endl;
	}
	catch (...)
	{
		cerr << "EXCEPTION (unknown)" << endl;
	}
};

