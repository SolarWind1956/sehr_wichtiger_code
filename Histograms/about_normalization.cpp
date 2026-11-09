//	about_normalization.cpp
//
//  	Sergii Edgar Hekkel (Heckel)
//  	15.02.2026
//
#include <iostream>
using std::cout; using std::endl; using std::boolalpha;
#include <cmath>

double 	normalized_number(double diff)
{								
    double 	exp 		= 	std::floor(std::log10(diff));	
    double 	norm 	= 	diff / std::pow(10.0, exp);
    return 	norm;
}			
int main()
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << "about_normalization" << endl;
	
	double test_dbl{100.0};
	
	for (auto delta = 10; delta < 100; delta+=10) {
		cout << "normalized number : " << normalized_number(test_dbl - delta) << endl;
	}
}
/*
Вывод на консоль:

-------------- I'm int main()about_normalization
normalized number : 9
normalized number : 8
normalized number : 7
normalized number : 6
normalized number : 5
normalized number : 4
normalized number : 3
normalized number : 2
normalized number : 1
*/