/*
 
*/ 

#include <iostream>
using std::cin; using std::cout; using std::endl;

#include <string>
using std::string;

#include <regex>
using std::regex; using std::sregex_iterator; using std::smatch;
using std::regex_error;

int main()
{
	try {
		// one or more alphanumeric characters followed by a '.' 
		// followed by "cpp" or "cxx" or "cc"
		regex r("[[:alnum:]]+\\.(cpp|cxx|cc)$", regex::icase);

		smatch results; // object to hold details about the match
		string filename;
		while (cin >> filename) 
			if (regex_search(filename, results, r))
				cout << results.str() << endl;  // print the match
	} catch (regex_error e) 
		{ cout << e.what() << " " << e.code() << endl; }

	return 0;
}
