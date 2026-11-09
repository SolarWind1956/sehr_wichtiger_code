/* 
	Own_Exception_Classes.cpp
*/
#include <iostream>
using std::cout; using std::endl; using std::boolalpha; using std::cin; using std::cerr;
#include <fstream>
#include <sstream>

#include <new>			//	Как выяснилось, не обязательное явное включение заголовка
#include <cstddef>
#include <memory>
#include <iterator>
#include <map>
#include <list>
#include <exception>
#include	<bitset>
#include <vector>

#include <ranges>			//	Для gcc этот заголовок обязателен??? , clang вроде как обошелся без него

#include <numeric>
#include <limits>
#include <climits>
#include <cmath> 
#include <math.h>
#include <cerrno>
#include <cstdlib> 
#include <stdlib.h> 
using namespace std;

/*
	**************************************************************************************
*/
static void		test_1()	{
	cout << "-------------- I'm " <<	__PRETTY_FUNCTION__	<< endl;
}
/*
*/
static void		test_2()	
{
	cout << "-------------- I'm " <<	__PRETTY_FUNCTION__	<< endl;
}	
class No_class  : public std::exception	{
  public:	
	virtual const char* what() const throw()	override { return "My class No_class"; };
};
class Unknown  : public std::exception	{
  public:
	virtual const char* what() const throw()	override { return "My class Unknown"; };
};
#if 0	
class Error : public std::exception {
  public:
    Error(const char *msg, State 	state);
    State 	GetState() const;
private:
    State 	state_;
};
#endif
/*
	-----------------------------------------------------------------------------------------------
*/
static void		test_3()	{	
	cout << "-------------- I'm " <<	__PRETTY_FUNCTION__	<< endl;
	try {
		throw No_class();
	}	
	catch (const No_class& e) {
		cerr << "Exception: No_class : --->   " << e.what() << endl;
		throw Unknown();
		return;
	}	
	catch (const Unknown& e) {
		cerr << "Exception: Unknown : --->   " << e.what() << endl;
		return;
	}
	catch (const std::exception& e) {
		cerr << "Exception: std::exception --->    " << e.what() << endl;
		return;
	}
	catch (...) {
		cerr << "Exception: catch(...) --->   " << endl;
		return;
	}	
	
}
/*
	-------------------------------------------------------------------------------------------
	-------------------> .hpp
*/
static void		test_4()	{	
	cout << "-------------- I'm " <<	__PRETTY_FUNCTION__	<< endl;
	}
/*
	****************************************************************************************
*/
static void		test_5()	{	
	cout << "-------------- I'm " <<	__PRETTY_FUNCTION__	<< endl;
}

static void		test_6()	{	
	cout << "\n-------------- I'm " <<	__PRETTY_FUNCTION__	<< endl;

}	
/*
	***************************		main()		******************************************
*/
int main()
{
	cout << "-------------- I'm " <<	__PRETTY_FUNCTION__	<< " of Ch_25_1_Kinds_of_Classes"	<< endl;
	
	try {
		//test_1();
		//test_2();
		test_3();
		//test_4();
		//test_5();
		//test_6();
				
		system("pause");
		}
		catch (const std::exception& e) {
				cerr << "Exception: catch (const std::exception& e) in main() --->   " << e.what() << endl;
				return	EXIT_FAILURE;
		}
	system("pause");
}
/*
https://ru.stackoverflow.com/questions/235352/Отличия-абстрактного-класса-от-интерфейса-abstract-class-and-interface
Абстрактный класс — это «заготовка» класса: реализовано большинство методов (включая внутренние), кроме нескольких. Эти несколько нереализованных методов вполне могут быть внутренними методами класса, они лишь уточняют детали имплементации. Абстрактный класс — средство для повторного использования кода, средство, чтобы указать, какой метод обязан быть перекрыт для завершения написания класса.

Интерфейс же — это своего рода контракт: интерфейсы используются в определениях чтобы указать, что объект, который будет использован на самом деле, должен реализовывать (для входных параметров) или будет гарантированно реализовывать (для выходных параметров) набор методов и (что намного важнее!) иметь определённую семантику. Интерфейс вполне может быть и пустым, тем не менее, имплементировать интерфейс означает поддерживать данную семантику.

Абстрактные классы идеологически схожи с шаблонами C++: и те, и другие являются заготовками классов, но шаблону для получения класса нужно специфицировать шаблонные типы, а абстрактному классу — абстрактные методы.

Интерфейсы идеологически схожи с заголовочными файлами C++: они раскрывают методы и скрывают конкретную реализацию.

Вопрос о том, является ли интерфейс или абстрактный класс собственно классом — техническая подробность реализации, зависящая от конкретного языка программирования. Например, в C++ интерфейсы отсутствуют вовсе, и их приходится эмулировать классами без данных. Абстрактный класс в C++ как таковой также отсутствует, но им можно считать любой класс с абстрактными методами. (Отсюда ограничение C++: как минимум 1 абстрактный метод в абстрактном классе.) Также в C++ можно (непрямо) инстанциировать абстрактный класс, вызвать абстрактный метод и (возможно) получить ошибку времени выполнения. В C# интерфейсы и абстрактные классы встроены в язык.
*/