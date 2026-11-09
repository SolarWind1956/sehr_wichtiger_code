// 	trivially_type_tester.hpp
#ifndef INCLUDE_GUARDS_trivially_type_tester_HPP
#define INCLUDE_GUARDS_trivially_type_tester_HPP

#include <iostream>
using std::cout; using std::endl;
#include <type_traits>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>

std::string 	yesno(bool b)	{ 
	return b ? "Yes !" : "No  !"; 
}

template<typename T>
void analyze_trivially_type(const std::string& name) {
	
    using U = T;
	
    bool is_triv    						= 	std::is_trivial_v						<U>;
    bool is_triv_copy 					= 	std::is_trivially_copyable_v				<U>;
    bool is_standard 					= 	std::is_standard_layout_v					<U>;
    bool is_aggregate 					= 	std::is_aggregate_v						<U>;
    bool is_pod 							= 	(is_triv && is_standard); 				// 	for reporting
    //bool is_literal 						= 	std::is_literal_type_v			<U>;
    //	bool has_user_dtor 					= 	!std::is_trivially_destructible_v			<U>;
	//	Ваше исправление
	bool has_user_dtor 					= 	std::is_destructible_v					<U> 
										&& 	!std::is_trivially_destructible_v			<U>
										&&  !std::is_trivially_copyable_v				<U>; 	// 	эвристика
										
    bool default_constructible 			= 	std::is_default_constructible_v			<U>;
	bool is_triv_default_constructible		=	std::is_trivially_default_constructible_v	<U>;
	bool is_nothrow_default_constructible	=	std::is_nothrow_default_constructible_v	<U>;
 
    bool has_user_default_ctor 			= 	false;
    
	/*
		Heuristics: 
		if not `default_constructible`
		but there's a declared deleted default ctor,
		it's still considered trivial, 
		if other conditions hold (handled by is_trivial).
		
		Эвристика:

		если не `default_constructible`,
		но есть объявленный `удалённый `конструктор по умолчанию`,
		тип всё равно считается `тривиальным`, 
		если выполняются другие условия (обрабатываются функцией std::is_trivial).
		
		We print a small report:
	*/
    cout << "================ " << name << " ===================\n";
    cout << std::left << std::setw(40) 
						<< "std::is_trivial" << " : " 				<< yesno(is_triv) << "\n";
    cout << std::left << std::setw(40) 
						<< "std::is_trivially_copyable" << " : " 		<< yesno(is_triv_copy) << "\n";
    cout << std::left << std::setw(40) 
						<< "std::is_standard_layout" << " : " 		<< yesno(is_standard) << "\n";
    cout << std::left << std::setw(40) 
						<< "std::is_aggregate" << " : " 				<< yesno(is_aggregate) << "\n";
    cout << std::left << std::setw(40) 
						<< "POD (trivial && standard layout)" << " : "	<< yesno(is_pod) << "\n";
    //cout << std::left << std::setw(40) 
	//					<< "std::is_literal_type_v" << " : " 			<< yesno(is_literal) << "\n";
    cout << std::left << std::setw(40) 
						<< "std::is_default_constructible" << " : " 	<< yesno(default_constructible) << "\n";
    cout << std::left << std::setw(40) 
						<< "user-provided dtor (heuristic)" << " : " 	<< yesno(has_user_dtor) << "\n";
	cout << std::left << std::setw(40) 
						<< "std::is_trivially_default_constructible" << " : " 	
						<< yesno(is_triv_default_constructible) << "\n";
    cout << std::left << std::setw(40) 
						<< "std::is_nothrow_default_constructible" << " : " 	
						<< yesno(is_nothrow_default_constructible) << "\n";
	 
	cout << "\nShort explanation:\n";
	
    if (is_triv) 
		cout << " - Type is TRIVIAL: can be memcopied, has trivial default/copy/dtor semantics.\n";
    else 
		cout << " - Type is NON-TRIVIAL: has user-provided special member or other non-trivial features.\n";
	
    if (is_standard) 
		cout << " - Type has STANDARD-LAYOUT: compatible memory layout rules.\n";
    else 
		cout << " - Not standard-layout: might have mixing of access specifiers, multiple incompatible bases, etc.\n";
	
    if (is_pod) 
		cout << " - Type is POD (Plain Old Data).\n";
    cout << "===========================================================\n\n";
}
//	--------------------------------------------------------------------------------------------------------
#endif	//	#ifndef INCLUDE_GUARDS_trivially_type_tester_HPP
