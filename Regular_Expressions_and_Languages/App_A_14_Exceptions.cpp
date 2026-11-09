//	App_A_14_Exceptions.cpp

#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <stdlib.h>
#include <cstring>
#include <exception>

using namespace std;

#include "..\type_name_as_str.h"

//	****************************************************************************
/*
	А.14. Исключения 

	Исключения используются (с помощью инструкции throw) для того, 
	чтобы сообщить вызывающей функции об ошибке, 
	которую невозможно обработать локально.
*/	
#if 0
namespace std
{
    class exception
    {
    public:
        exception	() 					noexcept;
        exception	(const exception&) 	noexcept;
		
        exception& 	operator=	(const exception&) 	noexcept;
        virtual 		~exception	(); 							// Destructor
		
        virtual const char* 	what	() 	const 	noexcept; 			// возвращает сообщение об исключении
    };
}
#endif
/*
	Standard exceptions

	-	logic_error
		-	invalid_argument
		-	domain_error
		-	length_error
		-	out_of_range
		-	future_error 						(since C++11)
	
	-	runtime_error
		-	range_error
		-	overflow_error
		-	underflow_error
		-	regex_error 						(since C++11)
		-	system_error 						(since C++11)
			-	ios_base::failure 				(since C++11)
			-	filesystem::filesystem_error 	(since C++17)
			
		-	tx_exception (TM TS)
		-	nonexistent_local_time 				(since C++20)
		-	ambiguous_local_time 				(since C++20)
		-	format_error 						(since C++20)
		
	-	bad_typeid
		-	bad_cast
		
	-	bad_any_cast 							(since C++17)
	
	-	bad_optional_access 					(since C++17)
	
	-	bad_expected_access 					(since C++23)
	
	-	bad_weak_ptr 							(since C++11)
	
	-	bad_function_call 						(since C++11)

	-	bad_alloc
		-	bad_array_new_length 				(since C++11)

	-	bad_exception

	-	ios_base::failure 						(until C++11)
		
	-	bad_variant_access 						(since C++17)
*/
struct Bad_size {
	int 		sz;
	Bad_size		(int 	size) 
				: sz	{size} 
				{}
	std::string	what(){
		return	static_cast<std::string>	(	"Заданные размеры вектора: " 
										+ 	std::to_string(sz) 
										+ 	" элементов - вне допустимых пределов"
										);
	}			
};
class Vector 
{
public:
	const int	maxsize{50};
	Vector(int 	size) 
	{ 
		if (size < 0 || maxsize < size) 
			throw 	Bad_size{size}; 	// 	Создаем временный объект пользовательского исключения
									//	и выбрасывает его в надежде, что его поймает ветвь catch
	}
	// ...
};
//	------------------------------------------------------------------------------
void auxil_func_1(int x)
{
	try {
		Vector 	v(x); // may throw
		// . . .
	}
	catch (Bad_size 	bs) {
		cerr << "Exception: " << bs.what() << endl;
		throw;
	}
	catch (std::exception 	e) {
		cerr << "Exception: " << e.what() << endl;
		// ...
	}
	catch (...){
		cerr << "All Exceptions in auxil_func_1()" << endl;
	}	
}
/*
	Инструкция throw без аргументов (т.е. просто throw;) 
	повторно генерирует текущее исключение. 
	
	Рассмотрим пример. 
*/
#if 0
try { 

}
catch (Some_exception& 	е) { 
			// 	локальная очистка 
	throw; 	// 	Остальное сделает вызывающая функция 
}
#endif
/*
	В качестве исключений можно использовать типы, 
	определенные пользователем. 
	
	В стандартной библиотеке определено несколько типов исключений, 
	которые также можно использовать. 
	
	Никогда не используйте в качестве исключений встроенные типы 
	(это может сделать кто-то еще, и ваши исключения могут внести путаницу). 
	
	Когда генерируется исключение, 
	система поддержки времени выполнения языка С++ 
	ищет вверх по стеку вызовов раздел саtch, 
	тип которого соответствует типу генерируемого объекта. 
	
	Другими словами, она просматривает инструкции try 
	в функции, генерирующей исключение, 
	затем 
	в функции, вызвавшей функцию, сгенерировавшую исключение, 
	затем 
	в функции, вызвавшей функцию, 
	вызвавшую функцию ... и так далее, 
	пока не найдет соответствие. 
	
	Если соответствие не найдено, программа прекращает работу. 
	
	В каждой функции, обнаруженной на этом пути, 
	и в каждой области видимости, в которой проходит поиск, 
	для выполнения очистки вызываются все деструкторы. 
	
	Этот процесс называется 'раскруткой стека' (stack unwinding). 
	
	Объект считается созданным в тот момент, 
	когда заканчивает работу его конструктор. 
	
	Он уничтожается либо в процессе раскрутки стека, 
	либо при каком-либо ином выходе из области видимости. 
	
	Отсюда следует, что частично созданные объекты 
	(у которых некоторые члены или базовые объекты созданы, а некоторые - нет),
	массивы и переменные, находящиеся в области видимости, 
	обрабатываются корректно. 
	
	Подобъекты уничтожаются тогда и только тогда, 
	когда они уже были созданы. 
*/
void test1()
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	try {
		auxil_func_1(100);
	}
	catch (std::exception& 	err) {
		cerr << "Exception: " << err.what() << endl;
		// ...
	}
	catch (...){
		cerr << "All Exceptions in test1()" << endl;
		throw;
	}
	cout << "Интересно, выбросит нас сюда раскрутка стека?" << endl;
	auxil_func_1(10);
	cout << "Вот теперь все хоккей" << endl;
}

//	----------------------------------------------------------------------------------------------------
/*
*/
void test2()
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
}	
//------------------------------------------------------------------------------
int main()
{
	cout << "----- App_A_14_Exceptions.cpp --------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	
	try {
		test1();
	}
	catch (std::exception& 	err) {
		cerr << "Exception in main(): " << err.what() << endl;
		// ...
	}
	catch (...){
		cerr << "All Exceptions in main()" << endl;
	}
	
	//test2();
	//test3(); 
	//test4(); 
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
