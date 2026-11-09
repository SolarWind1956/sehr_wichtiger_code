#pragma once
// #include "my_std_library.h"
#include <string>
using std::string;
#include <type_traits>

//      **********************************************************************************************
//      Определение полного типа, назначаемого компилятором через auto
//
// Шаблон, определяющий полный тип переменной
// Это я слизал у знающих людей вот с этого сайта:
// https://ru.stackoverflow.com/questions/722557/Определение-полного-типа-назначаемого-компилятором-через-auto/722564
//
// B C++11 появился decltype(x), который превращает выражение в его тип. 
// И decltype() идёт вместе со своим набором интересных правил. 
// Например, decltype(a) и decltype( (a) ), с дополнительными круглыми скобками, 
// будут обычно давать различные типы.
//
// Клиентская часть: type_name_as_str<decltype(i)>()
// 
// Описатель типа decltype Возвращает 'тип' указанного выражения. 
// а еще :                 
//			type_name_as_str<decltype( (i) )>() - только я еще не вдуплил, почему дополнительные скобки... :(
// 
// После Раздела 15.10.3. вдуплил: 
//			
//			decltype( (i) ) - скобки означают 'ссылку' - а дальше нужно, видимо,
// 
// учитывать правила свертки ссылок: & + & = &; && + & = &; & + && = &; && + && = &&.
// 
// Если i - это тип, а не объект, то канает вызов: type_name_as_str<i>()
// 
// Я и таким синтаксисом пользовался
// 
// typeid(decltype(int_var)).name(), но typeid() игнорирует ссылочность, так что первый вариант верный:
// 
//      type_name_as_str<decltype(i)>()
// 
// auto - также игнорирует ссылочность - нужно пользоваться decltype(auto)
//

template <typename T>
std::string 	type_name_as_str()
{
	std::string  cv_qualifiers{""};
	// Проверим константность и волатильность
	if (std::is_const<std::remove_reference_t<T>>::value) {
		cv_qualifiers += " const";
	}	
	if (std::is_volatile<std::remove_reference_t<T>>::value)
		cv_qualifiers += " volatile";

	std::string  value_reference{""};
	// Уточним, какая была ссылка: L-value или R-value, если она, конечно, была
	if (std::is_lvalue_reference<T>::value)
		value_reference += " &";
	else if (std::is_rvalue_reference<T>::value)
		value_reference += " &&";

	using 		base_type 		= 	std::remove_cv_t<std::remove_reference_t<T>>;
	std::string 	type_name		=	typeid(base_type).name();
	// 	typeid() игнорирует ссылочность, константность и волатильность, 
	// 	поэтому мы их и убрали, для начала	
	// 	(std::remove_cv_t<std::remove_reference_t<T>> - именно в такой последовательности)
	
	return cv_qualifiers + " " + type_name + " " + value_reference; 
}

//      **********************************************************************************************
/*
	std::remove_reference_t<std::remove_cv_t<T>> или std::remove_cv_t<std::remove_reference_t<T>>

*/
