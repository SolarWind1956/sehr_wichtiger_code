// demonstration_concepts.cpp
// Using concepts to select overloads.
#include <array>
#include <format>
using std::format;
#include <iostream>
using std::cout; using std::endl; using std::boolalpha; using std::cin; using std::cerr;
#include <iterator>
#include <list>
#include <ranges>
#include <span>
#include <vector>
#include <string>

#include "..\type_name_as_str.h"
/*
	****************************************************************************************
	15.6.6. Демонстрационные концепты 

	В документе стандарта С++ более 400 раз встречаются примечания 
	"только в демонстрационных целях" ('for the sake of exposition'. 'exposition only'). 
	
	Они выделены курсивом и указывают на элементы языка, 
	которые пока только обсуждаются.
	
	Часто такие фрагменты стандарта демонстрируют, что и как может быть реализовано в будущем. 
	
	Например, там упоминается демонстрационный концепт 'has-arrow' библиотеки <ranges>, 
	описывающий тип итераторов, поддерживающих оператор->(): 
*/	
template<class I> 
	concept 	has_arrow 	= 
		std::input_iterator<I> && (		std::is_pointer_v<I> 
								  || 	requires	(I i) 	{ 
											i.operator->(); 
										}
								  )
		; 
//	https://www.ece.uvic.ca/~frodo/cppbook/cppdraft/n4861/range.utility.helpers#:has-arrow				
/*					
	Другой пример из стандарта - 
	демонстрационный концепт decrementable библиотеки <ranges>, 
	описывающий типы итераторов, 
	поддерживающих оператор--():
*/	
template<class I> 
	concept 	decrementable 	= 
		std::incrementable<I>   && 	requires	(I 	i) 	{ 
										{ --i	} -> std::same_as<I&>; 
										{ i--	} -> std::same_as<I>; 
									}
		; 
//	https://www.ece.uvic.ca/~frodo/cppbook/cppdraft/n4861/range.iota.view#:decrementable					
/*

	У вас может появиться вопрос: почему концепт decrementable требует, 
	чтобы тип соответствовал концепту incrementable? 
	
	Напомним, что все итераторы поддерживают оператор++, 
	поэтому итератор decrementable должен также быть и incrementable. 
	
	Мы перечислили демонстрационные концепты в следующей таблице. 
	
	Вы можете найти каждый из них в разделе 'Index of library concepts' стандарта С++.
	
	https://www.ece.uvic.ca/~frodo/cppbook/cppdraft/n4861/conceptindex
*/	

void 	test_1(){
	cout << "\n-------------- I'm inline void " <<  __func__  << "()" << endl;
	
}
/*
	
*/

void 	test_2(){
	cout << "\n-------------- I'm inline void " <<  __func__  << "()" << endl;
	
}
/*
*/	
int main() 
{
	cout << "-------------- I'm int " <<  __func__  << "() of the demonstration_concepts.cpp" << endl;
	
	std::ostream_iterator<int> 				output	{std::cout, " "};
	std::ostream_iterator<char> 			char_output	{std::cout, " "};
	std::ostream_iterator<std::string> 		string_output	{std::cout, " "};
	
	
	std::array 	ints1	{1, 2, 3, 4, 5}; 		// has random-access iterators
	std::list 	ints2	{1, 2, 3}; 				// has bidirectional iterators
	std::vector<std::string>	strings_vector {"Hello, ", "World!"};
	std::vector<int>	int_vector {1, 2, 3, 4, 5};
	//	-------------------------------------------------------------------------------
	test_1();	cout << endl;

	//	-------------------------------------------------------------------------------
	test_2();			cout << endl;
	
	//	-------------------------------------------------------------------------------
}
/*
*/