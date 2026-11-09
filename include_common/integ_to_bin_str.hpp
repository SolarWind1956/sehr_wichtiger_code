/*
	integ_to_bin_str.hpp
*/ 

#include <iostream>
using std::cin; using std::cout; using std::endl;

#include <string>
using std::string;

#include <limits>

template <class T>
std::string 	integ_to_bin_str(T integral_par)
{
	if (std::is_integral_v<T> != true){
		cout << "template <class T> std::string	integ_to_bin_str(T integral_par)Integral type required. Sorry!" << endl;
		exit(1);
	}
	T 							integral_size		= sizeof(static_cast<T>(integral_par));
	
	std::string					res_string;	
	T 							integral_par_val 	= static_cast<T>(integral_par);
	T 							res_string_sz		= integral_size * 8;
	
	// std::numeric_limits<int>::digits  - количество бит, не считая знакового
	// Устанавливаем знаковый бит целого числа
	decltype(integral_par_val)	control_bit			=	1 << std::numeric_limits<int>::digits;

	char						bit_char;
	
	for (int index = 0; index != res_string_sz; ++ index)
	{
		// Проверяем слева направо текущий бит в переменной-параметре
		if ( (integral_par & control_bit) == control_bit)
			bit_char	=	'1';
		else
			bit_char	=	'0';
		
		res_string.append(1, bit_char);		// Если верит что написано, append сильно быстрее, чем push_back() или insert() 
		/*
			Что касается std::string, то из трех методов вставки в конец строки:
			insert(), push_back() и append() поседний, вроде как, самый быстрый.
			https://stackoverflow.com/questions/62220271/why-is-stdstringappend-so-much-faster-than-push-back-in-this-benchmark
		*/
		if (index == 0)
			// Учитывая особенности сдвига вправо знaковых целых, устанавливаем бит целого числа сразу за знаковым
			control_bit		=	1 << (std::numeric_limits<int>::digits - 1);
		else
			// сдвигаем единицу вправо для всех последующих бит
			control_bit 	= 	control_bit >> 1;
	}
	//	Надо число разбивать на тетрады, или не надо
	if (res_string.size() <= 4)
		return res_string;
	// Теперь надо разбить двоичное числа на тетрады с апострофом-разделителем
	char	apostrophe{0x27};
	std::string		intermediate_string{};
	int 			index 			= res_string.size() - 1;
	int				tetrad_count{4};
	do 
	{
		intermediate_string.append(1, res_string[index--]);	
		if (--tetrad_count <= 0){
			
			intermediate_string.append(1, apostrophe);	
			tetrad_count	=	4;
		}	
	}	while (index >= 0);
	// 	Реверс промежуточной строки в результирующую
	res_string.clear();
	for (int index = intermediate_string.size() - 1; index >=0; --index)
		res_string.append(1, intermediate_string[index]);
		
 	return res_string;
}

#include <limits>
#include <bitset>
using std::bitset;
using std::string;
using std::numeric_limits;

int bitset_example()
{
	cout << "I'm int bitset_example()" << endl;

	// print some numbers in binary representation
	cout << "267 as binary short:     " << bitset<numeric_limits<unsigned short>::digits>(267)	<< endl;

	cout << "267 as binary long:      "	<< bitset<numeric_limits<unsigned long>::digits>(267)	<< endl;

	cout << "10,000,000 with 24 bits: "	<< bitset<24>(1e7) << endl;

	// write binary representation into string
	string	str = bitset<42>(12'345'678).to_string();
	cout << "12,345,678 with 42 bits: " << str << endl;

	// transform binary representation into integral number
	cout << "\"1000101011\" as number:  "	<< bitset<100>("1000101011").to_ullong() << endl;

	return 0;
}