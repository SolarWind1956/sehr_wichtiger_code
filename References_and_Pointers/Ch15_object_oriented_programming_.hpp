#pragma once
// Ch15_object_oriented_programming.hpp

/*
	********************************************************************************************
	
	********************************************************************************************
*/
#include <functional>
using namespace std::placeholders;
using std::function;

#include <memory>

#include <vector>
using std::vector;

#include <map>
using std::map;

#include <string>
using std::string;

#include <iterator>
using std::inserter;

#include <iostream>
using std::cin; using std::cout; using std::endl;

#include <algorithm>
using std::transform;

/*

*/
class Quote
{
public:
	Quote() { cout << "I am class Quote CTOR" << endl; }

	void 			set_price(double pr_ce) { price = pr_ce; }
	virtual double 	get_price() const { cout << "Quote   "; return price; }

	virtual ~Quote() /*= default;*/ 					// динамическое связывание для деструктора
	{
		cout << "I am virtual ~Quote() DETOR" << endl;
	}
private:
	// Международный стандартный номер книги
	std::string 	bookNo; 						// ISBN number of this item
protected:
	double 			price = 0.0; 					// normal, undiscounted price
};

class Disc_quote : public Quote
{
public:
	Disc_quote() { cout << "I am class Disc_quote CTOR" << endl; }

	void 			set_quantity(std::size_t quaty) { quantity = quaty; }
	std::size_t  	get_quantity() { return quantity; }
	double 			get_price() const override { cout << "Disc_quote  "; return price; }
protected:
	std::size_t 	quantity = 0; 		// purchase size for the discount to apply
	double 			discount = 0.0;    	// fractional discount to apply
};

class Bulk_quote : public Disc_quote 				// Bulk_quote inherits from Quote
{
public:
	Bulk_quote() { cout << "I am class Bulk_quote CTOR" << endl; }
	double 			get_price() const override { cout << "Bulk_quote  "; return price; }
private:

};



/*
	********************************************************************************************
	* 
	********************************************************************************************
*/