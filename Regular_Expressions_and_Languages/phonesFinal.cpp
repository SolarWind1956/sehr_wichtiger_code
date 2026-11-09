/*
 
*/ 

#include <iostream>
using std::cin; using std::cout; using std::endl;

#include <string>
using std::string;

#include <regex>
using std::regex; using std::sregex_iterator; using std::smatch;
using std::regex_error;
using std::regex_constants::format_no_copy;

int main()
{
	// phone has 10 digits, optional parentheses around the area code
	// components are separated by an optional space, ',' or '-'
	string phone = "\\(?\\d{3}\\)?[-. ]?\\d{3}[-. ]?\\d{4}";

	// parentheses indicate subexpressions;
	// our overall expression has 7 subexpressions: 
	//     ( ddd ) space ddd - dddd
	// the subexpressions 1, 3, 4, and 6 are optional
	// the subexpressions 2, 5, and 7 hold the digits of the number
	phone = "(\\()?(\\d{3})(\\))?([-. ])?(\\d{3})([-. ])?(\\d{4})";
	regex r(phone);  // a regex to find our pattern
	smatch m;        // a match object for the results
	string s;

	// generate just the phone numbers:  use a new format string
	string fmt = "$2.$5.$7"; // reformat numbers to ddd.ddd.dddd
	string fmt2 = "$2.$5.$7 "; // adds space at the end as a separator

	// read each record from the input file
	while (getline(cin, s))
	{
		cout << regex_replace(s, r, fmt) << endl;

		// tell regex_replace to copy only the text that it replaces
		cout << regex_replace(s, r, fmt2, format_no_copy) << endl;
	}

	return 0;
}
