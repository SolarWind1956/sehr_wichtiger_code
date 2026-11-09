// Ch14_operator_overloading_and_conversions.cpp
/*
Стенли Б. Липпман, Жози Лажойе, Барбара Э. Му. Язык программирования C++. Базовый курс. Пятое издание. 2017
Stanley В. Lippman, Josee Lajoie, Barbara Е. Моо. C++ Primer. 5th Edition. 2017

Часть II. Библиотека С++
Глава 15. Объектно-ориентированное программирование
*/

#include "Ch15_object_oriented_programming.hpp"
#include "type_name_as_str.h"
/*
	Механизм возврата простой ссылки из функции
*/
auto ref_return(Quote& ref_obj) -> Quote&
{
	cout << "===== auto& ref_return(Quote& ref_obj)  -> Quote& " << type_name_as_str<decltype(ref_obj)>() << endl;
	return ref_obj;
};
/*
	Вызов должен быть таким:

	decltype(auto)	any_obj_ref = ref_return(bulk_quote);

	Других вариантов я перебрал уйму, и выстрелила только такая конструкция
*/

void Ch15_object_oriented_programming()
{
	cout << "I am Ch15_object_oriented_programming()" << endl;

	{
		/*
		*/
		cout << "-------------------------------------------------- Тесты со ссылками" << endl;
		//1.
		Disc_quote			disc_quote;

		disc_quote.set_price(543.14);
		disc_quote.set_quantity(100);

		//2.
		Bulk_quote			bulk_quote;

		bulk_quote.set_price(2.14);
		bulk_quote.set_quantity(200);

		//3.
		Quote				any_quote;
		any_quote.set_price(42.14);
		cout << any_quote.get_price() << endl;

		cout << "-------------------------------------------------- Тесты со ссылками и лямбда" << endl;
		auto	ret_derived = [](Quote& any_quote_obj)  -> Quote& {
			cout << "===== auto	ret_derived	= [](Quote& any_quote_obj) -> Quote& ) "
				<< type_name_as_str<decltype(any_quote_obj)>() << endl;
			return any_quote_obj;
			};
		// Так как параметром выступает объект суперкласса иерархии,
		// аргументом может быть также объект любого производного класса этой же иерархии
		// Конечно же, он будет обрезан до части суперкласса (специфика производных классов исчезнет)
		// Так вдобавок лямбда со ссылочным параметром не дает работать механизму динамического связывания
		//4.
		cout << "decltype(auto) bulk_quote_obj	 =	ret_derived(bulk_quote)" << endl;
		decltype(auto) bulk_quote_obj = ret_derived(bulk_quote);
		cout << bulk_quote_obj.get_price() << endl;
		cout << type_name_as_str<decltype(bulk_quote_obj)>() << endl;

		cout << "auto any_obj_ref 	= ref_return(bulk_quote);" << endl;
		decltype(auto) any_obj_ref = ref_return(bulk_quote);
		cout << any_obj_ref.get_price() << endl;
		cout << type_name_as_str<decltype(any_obj_ref)>() << endl;

		cout << "decltype(auto) disc_quote_ob	=	ret_derived(disc_quote);" << endl;
		decltype(auto) disc_quote_ob = ret_derived(disc_quote);
		cout << disc_quote_ob.get_price() << endl;

		cout << "decltype(auto) any_quote_obj		=	ret_derived(any_quote);" << endl;
		decltype(auto) any_quote_obj = ret_derived(any_quote);
		cout << any_quote_obj.get_price() << endl;

		cout << "-------------------------------------------------- Тесты с указателями и лямбда" << endl;
		cout << "Попробуем теперь создать объекты классов этой иерархи в куче" << endl;
		//5.
		Quote* ptr_any_quote = new Quote();
		//6.
		Disc_quote* ptr_disc_quote = new Disc_quote();
		//7.
		Bulk_quote* ptr_bulk_quote = new Bulk_quote();

		ptr_any_quote->set_price(1222.14);
		ptr_disc_quote->set_price(25555.14);
		ptr_bulk_quote->set_price(4777.74);


		/*
			Поскольку параметр ptr_any_quote_obj является указателем на тип Quote,
			эту функцию можно вызвать как для объекта класса Quote, так и для объекта класса Bulk_quote,
			и для объекта класса Disc_quote.
		*/
		auto	ptr_ret_derived = [](Quote* ptr_any_quote_obj) { return ptr_any_quote_obj; };

		auto 	ptr_any_obj_by_hierarchy = ptr_ret_derived(ptr_bulk_quote);
		cout << ptr_any_obj_by_hierarchy->get_price() << endl;

		ptr_any_obj_by_hierarchy = ptr_ret_derived(ptr_disc_quote);
		cout << ptr_any_obj_by_hierarchy->get_price() << endl;
		//cout << ptr_any_obj_by_hierarchy->get_quantity() << endl;

		ptr_any_obj_by_hierarchy = ptr_ret_derived(ptr_any_quote);
		cout << ptr_any_obj_by_hierarchy->get_price() << endl;

		Quote* ptr_any_quote_else = ptr_bulk_quote;	// Объект будет Bulk_quote НО!!! обрезан до Quote
		ptr_any_obj_by_hierarchy = ptr_ret_derived(ptr_any_quote_else);
		cout << ptr_any_obj_by_hierarchy->get_price() << endl;
		// cout << ptr_any_obj_by_hierarchy->get_quantity() << endl;	Объект Bulk_quote обрезан до Quote

		cout << "Явно вызываем деструкторы" << endl;
		delete	ptr_any_quote;		ptr_any_quote = nullptr;
		delete	ptr_disc_quote;		ptr_disc_quote = nullptr;
		delete	ptr_bulk_quote;		ptr_bulk_quote = nullptr; ptr_any_quote_else = nullptr; // <- Чтобы не висел
		cout << "Теперь отработают деструкторы, запущенные компилятором" << endl;
	}
	{
		cout << "-------------------------------------------- Тесты с указателями и ссылками вперемешку" << endl;
		/*
			Поскольку производная часть объекта соответствует его базовому классу (классам),
			объект производного типа можно использовать так, как будто это объект его базового класса (классов).

			В частности, ссылку или указатель на базовый класс можно связать с частью базового класса производного объекта.
		*/
		Quote 		item; 				// объект базового типа
		item.set_price(77.7);

		Disc_quote 	disc; 				// объект производного типа
		disc.set_price(555.5);

		Quote* r_ptr = &item; 	// r_ptr указывает на объект Quote
		cout << r_ptr->get_price() << endl;

		r_ptr = &disc; 				// r_ptr указывает на часть Quote объекта disc
		cout << r_ptr->get_price() << endl;

		Quote& ref = disc; 	// ref связан с частью Quote объекта disc
		cout << ref.get_price() << endl;

	}

}
