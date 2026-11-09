//	heckel_integ_bin_str.cpp
//
//  	Sergej Edgar Heckel 
//  	17.02.2026
//
// 	Demonstration program: Converts an integer to a formatted binary string 
//	separated into tetrads by a user-defined symbol
#include <iostream>
using std::cout; using std::endl;
#include <string>
#include <algorithm>
#include <bitset>				//	for comparison and checking, but...
#include <concepts>
#include <limits>
#include <cassert>

//	------------------------------------ integ_bin_str()
template <	typename 	T 	= int
		, 	char 		BDS 	= 0x27						//	Binary Digit Separator, default = apostrophe - ASCII '0x27'
		>
	requires std::is_integral_v<T> 						// 	Restrict integral types only
std::string 		integ_bin_str(T integr_par)
{
	constexpr 
	std::size_t		type_sz_bits		=	sizeof(T) * 8;	//	Type size in bits
	
	std::string		res_string;							//	Result binary number string representation 
	std::size_t		res_string_sz	= 	type_sz_bits;
	
	auto mask_bit	=	1 << (type_sz_bits - 1);			//	Set sign bit in mask_bit	
	
	char				bit_char;							//	'1' or '0'
	
	//	Change bit value in integral to binary symbols in result string
	for (int chr_idx = 0; chr_idx != res_string_sz; ++chr_idx)
	{
		bit_char	= ((integr_par & mask_bit) == mask_bit) ? '1' : '0';
		
		res_string.append(1, bit_char);
		
		mask_bit	=	(chr_idx == 0) ? 1 << (type_sz_bits - 2) : mask_bit >> 1;		// 	Next mask
	}
	assert(res_string == std::bitset<(type_sz_bits)>(integr_par).to_string());
													//	It would be possible to immediately use std::bitset<> class 
													//	to obtain unformatted binary string, 
													//	but then object file would become about 3 KB larger.
													//	res_string = std::bitset<(type_sz_bits)>(integr_par).to_string();
	// 	Split binary number into tetrads with separator
	char				separator = BDS;					//	BDS - user-defined separator between 4-bit groups 
	std::string		auxil_string{};					//	Auxiliary reverse string
	int				bit_in_tetrad{4};				//	Counter bits in tetrad
	int 				str_chr_idx 	= res_string.size() - 1;
	
	//	Form  auxiliary string but inversed and with separator symbols for tetrad
	do 
	{
		auxil_string.append(1, res_string[str_chr_idx--]);	
		
		if (--bit_in_tetrad <= 0){
			auxil_string.append(1, separator);		
			bit_in_tetrad	=	4;
		}	
	}	while (str_chr_idx >= 0);
	
	auxil_string.append({'b', '0'});					//	Binary literal prefix
	
	if (std::is_same_v<T, bool>) {					//	For T == bool:  1000'0000'b0 or 0000'0000'b0
		auxil_string.erase(2, 7); 					//	leave only 10'b0 or 00'b0
	}
	
	// 	Reverse auxiliary string into resulting one
	res_string.clear();
	
	std::copy(auxil_string.rbegin(), auxil_string.rend(), std::back_inserter(res_string));
	
 	return res_string;
}
//	--------------------------------------------	
template <	typename 	T 	= int>
void test_print(T integr){
	cout << "Number: " << integr << "; binary string: std::bitset(integr)   : " 
		 << std::bitset<(std::numeric_limits<T>::digits + 1)>(integr) 	<< endl; 
	cout << "Number: " << integr << "; binary string: integ_bin_str(integr) : " 
		 << integ_bin_str<T>(integr)				<< endl;	
}

//  	---------------------------------------------		main()
int		main()
{
	// 	Print some numbers in binary representation
	#if 1	
	test_print<int>(1'777'789);
	test_print<int>(-1'777'789);
	
	test_print<short>(777);
	test_print<short>(-777);
	#endif	
	
	#if 0	
	test_print<short>(1'777'789);
	test_print<short>(-1'777'789);
	#endif	

	#if 1	
	test_print<bool>(true);
	test_print<bool>(false);
	#endif	
	return 0;
}