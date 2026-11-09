	//		#pragma once -> SF.8: Use #include guards for all header files
#ifndef INCLUDE_GUARDS_COMPARISON_CLASSES_HPP
#define INCLUDE_GUARDS_COMPARISON_CLASSES_HPP
/*
	Comparison_Classes.hpp
*/	
#include	<iostream>
using std::cout; using std::endl; using std::boolalpha; using std::cin; using std::cerr;
#include	<string>
using std::string;
/*
	***************************************************************************************************
	17.1.4.1. Операция сравнения "<"	
	
	Допустим, мы хотим отсортировать контейнер 'fruit', 
	используя нечувствительное к регистру букв сравнение. 
	
	Мы можем это сделать, определив класс функциональных объектов, 
	выполняющий сравнение, будучи вызванным для пары значений типа string:
*/
class 	Nocase 		// 	case-insensitive string compare
{
 public:
	bool 	operator 	() 	(const string&, const string&) 	const;
};

// return true if x is lexicographically less than y, not taking case into account
bool 	Nocase::operator	() 	(const string& 	x, 	const string& 	y) 	const
{
	string::const_iterator 	x_ptr 	= 	x.begin();
	string::const_iterator 	y_ptr 	= 	y.begin();
	
	//	Цикл будет крутиться до тех пор, пока символы с одинаковыми индексами будут равны, 
	//	или пока не завершится одна из строк
	while (x_ptr != x.end() && y_ptr != y.end() && toupper(*x_ptr) == toupper(*y_ptr))
	{
		++x_ptr;
		++y_ptr;
	}
	//	Если завершилась первая строка
	if (x_ptr == x.end()) 
		return y_ptr != y.end();	//	то она будет меньше второй, которая еще не завершилась, так как оказалась короче
	//	Если завершилась вторая строка, а первая не завершилась, то первая будет больше второй - она длиннее
	if (y_ptr == y.end()) 
		return false;
	//	Ни одна строка не завершилась, значит, имело место лексикографическое неравенство пары символов
	return 	toupper(*x_ptr) < toupper(*y_ptr);		//	с одинаковыми итераторами из обеих строк 
}
/*
*/
class 	String_cmp 
{
	// тип, представляющий критерий сравнения на этапе выполнения
	// String_cmp (int n = 0);
	// ...
 public:
	bool 	operator 	() 	(const string&, const string&) 	const;
};
// return true if x is lexicographically less than y, not taking case into account
bool 	String_cmp::operator	() 	(const string& 	x, 	const string& 	y) 	const
{
	string::const_iterator 	x_ptr 	= 	x.begin();
	string::const_iterator 	y_ptr 	= 	y.begin();
	
	//	Цикл будет крутиться до тех пор, пока символы с одинаковыми индексами будут равны, 
	//	или пока не завершится одна из строк
	while (x_ptr != x.end() && y_ptr != y.end() && *x_ptr == *y_ptr)
	{
		++x_ptr;
		++y_ptr;
	}
	//	Если завершилась первая строка
	if (x_ptr == x.end()) 
		return y_ptr != y.end();	//	то она будет меньше второй, которая еще не завершилась, так как оказалась короче
	//	Если завершилась вторая строка, а первая не завершилась, то первая будет больше второй - она длиннее
	if (y_ptr == y.end()) 
		return false;
	//	Ни одна строка не завершилась, значит, имело место лексикографическое неравенство пары символов
	return 	*x_ptr < *y_ptr;		//	с одинаковыми итераторами из обеих строк 
}
/*
	****************************************************************************************************
*/
#endif 	// INCLUDE_GUARDS_COMPARISON_CLASSES_HPP
