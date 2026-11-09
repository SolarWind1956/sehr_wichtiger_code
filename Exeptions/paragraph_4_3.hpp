#pragma once
/*
paragraph_4_3.hpp

	Scott Meyers "Effective Modern С++"
	Скотт Мейерс "Эффективный и современныи С++. 42 рекомендации по использованию С++// и С++14"

	ГЛАВА 4.			Интеллектуальные указатели
	Chapter 4. 			Smart Pointers

	Параграф	4.3.	Используйте std::weak_ptr для std::shared_рtr-подобных указатепей, которые моrут быть висячими.
	Paragraph	4.3.	Item 20: Use std::weak_ptr for std::shared_ptrlike pointers that can dangle.
 */
#include <thread>
#include <vector>
using std::vector;
#include <string>
#include <cstring>
#include <iostream>
using std::string; using std::cout; using std::endl; using std::cerr; using std::strcmp; using std::boolalpha;
#include <iterator>
using std::begin; using std::end;
#include <functional>
#include <map>
using std::pair;
#include <exception>		// bad_exception (ген. unexpected() )
#include <memory>			// bad_weak_ptr
using std::shared_ptr; using std::unique_ptr; using std::make_unique; using std::make_shared;
#include <type_traits>
using std::is_pointer_v;	using std::is_reference_v;	using std::forward;
#include <cassert>
#include <cmath>

#include "../../ScottMeyers_42.hpp"
#include "../MeyersVidgets/MeyersVidgetsClasses.h"
#include "type_name_as_str.h"
#include <system_error>
#include <system_error>
/*
	Не для всех видов прерывание...
	std::system_error - да, канает
	std::future_error - да, канает

	Для остальных видов - вопрос!!!
	Надо смотреть каждый класс, для всех, видимо, есть только what()
*/
//	https://en.cppreference.com/w/cpp/error/error_code.html
template <typename T>
	requires requires (T t) {
	t.code();
	t.code().category().name();
	t.code().value();
	t.code().message();
	t.code().default_error_condition().category().name();
	t.code().default_error_condition().value();
	t.code().default_error_condition().message();
}
void process_error_code_exception(const T& error_code_)
{
	cout << "------ void process_code_exception(const T& exception) -------- Детализация исключения ---- " << endl;
	auto c = error_code_.code();
	cout << endl;
	cout << "category: " << error_code_.code().category().name() << endl;
	cout << "value: " << error_code_.code().value() << endl;
	cout << "msg: " << error_code_.code().message() << endl;

	cout << endl;
	cout << "def category: " << error_code_.code().default_error_condition().category().name() << endl;
	cout << "def value: " << error_code_.code().default_error_condition().value() << endl;
	cout << "def msg: " << error_code_.code().default_error_condition().message() << endl;
};
void about_weak_ptr_step_11()
{
	cout << "--------------------------------------------- void about_weak_ptr_step_11()" << endl;
	// after shptr_widget is constructed, the pointed-to Widget_for_weak_ptr's .ref count (RC) is 1. 
	auto shptr_widget = std::make_shared<Widget_for_weak_ptr>();
	cout << "shptr_widget.use_count() before std::weak_ptr - " << shptr_widget.use_count() << endl;
	
	// wptr_widget points to same Widget as shptr_widget. RC remains 1
	std::weak_ptr<Widget_for_weak_ptr>	wptr_widget(shptr_widget);
	cout << "shptr_widget.use_count() after std::weak_ptr - " << shptr_widget.use_count() << endl;
	cout << boolalpha << "wptr_widget.expired() == " << wptr_widget.expired() << endl;
	auto shptr_from_weak = wptr_widget.lock();			// Объектом уже владеют два указателя
	cout << "Count of references of shptr_w after wptr_widget.lock(); : " << shptr_from_weak.use_count() << endl;
	
	// Объектом уже владеют два указателя
	shptr_from_weak	= nullptr;
	shptr_widget	= nullptr;

	// Еще раз попытаемся восстановить общий указатель из мягкого, но уже просраченного 
	// std::shared_ptr<Investments43> 
	shptr_from_weak = wptr_widget.lock();
	cout << "Count of references of shptr_widget after wptr_widget.lock(); : " << shptr_from_weak.use_count() << endl;
	// Увы, уже ничего не вышло
	cout << "-------------- Проверим прерывание ----------- " << endl;
	{
		cout << boolalpha << "wptr_widget.expired() == " << wptr_widget.expired() << endl;
		/*
			Второй разновидностью является конструктор std::shared_ptr, принимающий std::weak_ptr в качестве аргумента.
			В этом случае, если std::weak_ptr просрочен, генерируется исключение std::bad_weak_ptr.
		*/
		try {
			std::shared_ptr<Widget_for_weak_ptr>	shptr_from_weak(wptr_widget);
		}
		catch (const std::bad_weak_ptr & exept)
		{
			cout << "BAD WEAK PTR: " << exept.what() << endl;
			//	Генерируем исключение std::system_error
			
			//		Этой страницы не нашел... https://en.cppreference.com/w/cpp/error/bad_weak_ptr
			//
			//	https://en.cppreference.com/w/cpp/error/error_code
			//	https://en.cppreference.com/w/cpp/error/errc
			//	https://en.cppreference.com/w/cpp/error/error_condition
			//	https://en.cppreference.com/w/cpp/error/system_error
			//	https://en.cppreference.com/w/cpp/error/make_error_code
			//	https://en.cppreference.com/w/cpp/error/make_error_condition
			//	https://en.cppreference.com/w/cpp/error/errc
			//	https://en.cppreference.com/w/cpp/error/condition
			//	https://en.cppreference.com/w/cpp/error/throw_with_nested
			//	https://en.cppreference.com/w/cpp/error/nested_exception
			//	https://en.cppreference.com/w/cpp/error/rethrow_if_nested
			//	https://en.cppreference.com/w/cpp/error/current_exception
			//	https://en.cppreference.com/w/cpp/error/uncaught_exceptions
			//	https://en.cppreference.com/w/cpp/error/uncaught_exception
			//	https://en.cppreference.com/w/cpp/error/terminate
			//	https://en.cppreference.com/w/cpp/error/set_terminate
			//	https://en.cppreference.com/w/cpp/error/get_terminate
			//	https://en.cppreference.com/w/cpp/error/unexpected
			//	throw std::system_error(std::make_error_code(std::errc::bad_weak_ptr), "bad_weak_ptr)";
		}
		catch (const std::exception & exept)
		{
			cout << "EXCEPTION: " << exept.what() << endl;
		}
		catch (...)
		{
			cout << "EXCEPTION (unknown)" << endl;
		};
	}
	cout << "-------------- ПровериЛИ прерывание ----------- " << endl;
	cout << "void about_weak_ptr_step_11() --------- Конец работы функции ----------------------- " << endl;
};
