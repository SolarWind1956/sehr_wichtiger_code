/*
 
*/ 

#include <iostream>
using std::cin; using std::cout; using std::endl;

#include <string>
using std::string;

#include <regex>
using std::regex; using std::sregex_iterator; using std::smatch;

void checkPattern(const regex &r, const string &s)
{
	smatch results;
	if (regex_search(s, results, r))
		cout << results.str() << endl;
	else 
		cout << "no match for " << s << endl;
}

int main()
{
	// phone has 10 digits, optional parentheses around the area code
	// components are separated by an optional space, ',' or '-'
	string pattern = "\\(?\\d{3}\\)?[-. ]?\\d{3}[-. ]?\\d{4}";
	regex r(pattern);  // a regex to match our pattern

	// some numbers to try to match
	string mtch1 = "(908) 555-0181";
	string mtch2 = "(908)555-0182";
	string mtch3 = "908 555-0183";
	string mtch4 = "908.555-0184";
	string mtch5 = "9085550185";

	smatch results;
	checkPattern(r, mtch1);
	checkPattern(r, mtch2);
	checkPattern(r, mtch3);
	checkPattern(r, mtch4);
	checkPattern(r, mtch5);

	string s;
	while (getline(cin, s)) 
	{
		checkPattern(r, s);
	}
	
	return 0;
}

