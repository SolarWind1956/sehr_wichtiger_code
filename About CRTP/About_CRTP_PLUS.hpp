// 	About_CRTP_PLUS.hpp 
#ifndef INCLUDE_GUARDS_About_CRTP_PLUS_HPP
#define INCLUDE_GUARDS_About_CRTP_PLUS_HPP
//	--------------------------------------------------------------------------------------------------------
#include <iostream>
using std::cout; using std::cerr; using std::endl; using std::boolalpha;
#include <memory>
#include <type_traits>
#include <cassert>

/*
	******************************************************************************************************
	CRTP и управление временем жизни объектов
 
	Этот пример демонстрирует корректное использование паттерна CRTP
	(Curiously Recurring Template Pattern) вместе с умными указателями
	(std::unique_ptr и std::shared_ptr) для управления временем жизни
	объектов, создаваемых в куче.
 
	Использование CRTP позволяет сделать базовый класс "осведомлённым" о
	типе производного класса и тем самым делегировать 
	удаление или вызовы финализаторов без виртуальности.
	
	Основная идея:
	- 	`Базовый` класс `шаблонизируется` по `производному`.
	- 	`Производный` класс передаёт себя в качестве `параметра шаблона` `базовому`.
	- 	Это даёт возможность реализовать 
		безопасное удаление без виртуального деструктора.
 
	Дополнительно:
	- 	Использование std::unique_ptr гарантирует автоматическое освобождение памяти.
	- 	std::make_unique предпочтителен перед `new`.
*/
template <class Derived>
class Base {
  public:
	Base()	{
		cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	}
	~Base()	 noexcept {
		cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	}
		
	void AnyMethod()
	{
		cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
		static_cast<Derived*>(this)->AnyMethod();
	}
};
//	--------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 	Фабрика, которая создаёт правильный shared_ptr
template <typename DERIVED, typename... Args>
std::shared_ptr<Base<DERIVED>> make_polymorphic(Args&&... args) {
	return std::shared_ptr<Base<DERIVED>>	(	new DERIVED	(	std::forward<Args>(args)...)
														,	[](Base<DERIVED>* ptr) { 
																delete static_cast<DERIVED*>(ptr); 
															}
										);
}
//	--------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 	Альтернативная фабрика уже с unique_ptr (ещё безопаснее и дешевле)
template <typename DERIVED>
auto make_unique_polymorphic(auto&&... args) {
    return std::unique_ptr	<	Base<DERIVED>
							, 	void(*)(Base<DERIVED>*)
							>	
							(	new DERIVED	(	std::forward<decltype(args)>(args)...)
											,   [](Base<DERIVED>* ptr){ 
													delete static_cast<DERIVED*>(ptr); 
												}
											);
}
//	-------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//	Нешаблонный производный класс
class Derived : public Base<Derived> {
  public:
	Derived()	{
		cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	}
	~Derived()  noexcept 	{
		cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	}
	// 	Реализация методов
	void AnyMethod()	{
		cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	}
};
static_assert(std::is_base_of_v<Base<Derived>, Derived>, "Derived must inherit from Base");

//	-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Шаблонный производный класс
template <typename T>
class TemplateDerived : public Base<TemplateDerived<T>> {
  public:
	TemplateDerived(T m_int_data_) : m_int_data{m_int_data_}	{
		cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	}
	~TemplateDerived()  noexcept {
		cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	}
	// 	Реализация методов
	void AnyMethod()	{
		cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << "m_int_data == " << m_int_data << endl;
	}
	
	T 	m_int_data{T{}};
};
static_assert(	std::is_base_of_v<Base<TemplateDerived<int>>, TemplateDerived<int>>
			, 	"TemplateDerived must inherit from Base"
			);
static_assert(	std::is_base_of_v<Base<TemplateDerived<double>>, TemplateDerived<double>>
			, 	"TemplateDerived must inherit from Base"
			);

void second_example_2() {
	cout << "-------------- I'm " << __PRETTY_FUNCTION__ << endl;
	cout << "\n--- Пока создаем объекты в стеке ----------------------------------------- " <<  endl;
	{
		cout << "-------------------------------------------- " <<  endl;
		Base	<Derived>	base;
		base.AnyMethod();		
		cout << "--------------------------------------------- " <<  endl;
		Derived			derived;
		derived.AnyMethod();		
		cout << "--------------------------------------------- " <<  endl;
		TemplateDerived	templ_derived_1{11};
		templ_derived_1.AnyMethod();		
		cout << "--------------------------------------------- " <<  endl;
		TemplateDerived	templ_derived_2{3.141};
		templ_derived_2.AnyMethod();		
		cout << "--------------------------------------------- " <<  endl;
		//	Деструкторы отработают в обратном порядке
	}
	cout << "\n--- Теперь создаем производные объекты в куче напрямую ----------------------------- " <<  endl;
	{
		cout << "-------------------------------------------- " <<  endl;
		auto base_ptr				=	new Base	<Derived>	;
		base_ptr->AnyMethod();		
		cout << "--------------------------------------------- " <<  endl;
		auto derived_ptr				=	new Derived;
		derived_ptr->AnyMethod();		
		cout << "--------------------------------------------- " <<  endl;
		auto templ_derived_1_ptr 		=	new TemplateDerived(11);
		templ_derived_1_ptr->AnyMethod();		
		cout << "--------------------------------------------- " <<  endl;
		auto templ_derived_2_ptr 		=	new TemplateDerived(3.141);
		templ_derived_2_ptr->AnyMethod();		
		cout << "--------------------------------------------- " <<  endl;
	
		delete base_ptr; 			base_ptr = nullptr;
		delete derived_ptr; 			derived_ptr = nullptr;
		delete templ_derived_1_ptr; 	templ_derived_1_ptr = nullptr;
		delete templ_derived_2_ptr; 	templ_derived_2_ptr = nullptr;
	}	
	cout << "\n--- Теперь создаем производные объекты также в куче но через базовый класс ----- " <<  endl;
	//	Используем разделяемые указатели и их фабрику с делитером
	{
		cout << "-------------------------------------------- " <<  endl;
		cout << "-------------------------------------------- " <<  endl;
		auto base_derived_shptr			=	make_polymorphic<Derived>(); 
		base_derived_shptr->AnyMethod();		
		cout << "--------------------------------------------- " <<  endl;
		auto base_templ_derived_1_shptr 	=	make_polymorphic<TemplateDerived<int>>(42);
		base_templ_derived_1_shptr->AnyMethod();		
		cout << "--------------------------------------------- " <<  endl;
		auto base_templ_derived_2_shptr 	=	make_polymorphic<TemplateDerived<double>>(3.141);
		base_templ_derived_2_shptr->AnyMethod();		
		cout << "--------------------------------------------- " <<  endl;
	}
}
//	Используем уникальные указатели и их фабрику с делитером
void second_example_3() {
	cout << "-------------- I'm " << __PRETTY_FUNCTION__ << endl;
	cout << "\n--- Cоздаем производные объекты также в куче но через базовый класс ----- " <<  endl;
	{
		cout << "-------------------------------------------- " <<  endl;
		auto base_derived_uptr			=	make_unique_polymorphic<Derived>(); 
		base_derived_uptr->AnyMethod();		
		cout << "--------------------------------------------- " <<  endl;
		auto base_templ_derived_1_uptr 	=	make_unique_polymorphic<TemplateDerived<int>>(42);
		base_templ_derived_1_uptr->AnyMethod();		
		cout << "--------------------------------------------- " <<  endl;
		auto base_templ_derived_2_uptr 	=	make_unique_polymorphic<TemplateDerived<double>>(3.141);
		base_templ_derived_2_uptr->AnyMethod();		
		cout << "--------------------------------------------- " <<  endl;
	}
}
//	--------------------------------------------------------------------------------------------------------
#endif	//	#ifndef INCLUDE_GUARDS_About_CRTP_PLUS_HPP
