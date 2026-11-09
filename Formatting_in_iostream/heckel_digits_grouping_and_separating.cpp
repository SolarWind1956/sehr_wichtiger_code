//
//     heckel_digits_GroupingAndSeparating.cpp
//
//  	Sergej Edgar Heckel 
//  	20.02.2026
//
// 	Demonstration program: prints arithmetical types variables and literals
//						  with grouping and group separating of digits
//	Code refined after technical consultation with ChatGPT (OpenAI), 2026.
//	
#include <iostream>
using std::cout; using std::endl; 
#include <string>
#include <algorithm>
#include <bitset>										//	for comparison and checking, but...
#include <concepts>
#include <limits>
#include <cassert>
#include <locale>
#include <type_traits>
#include <cstddef>										//	for std::byte

//	------------------------------------ 	template <> 		std::string 	integ_bin_str()
template <typename T>
concept integral_or_byte =
    std::is_integral_v<T> || std::is_same_v<T, std::byte>;	// 	Restrict only integral types and std::byte
	
template <integral_or_byte T = int>		
std::string 		integr_to_bin_str_with_sep(T integr_param, char separator = '\'')
{
	if constexpr (std::same_as<T, bool>) {
        return integr_param ? "0b1" : "0b0";				//	result string for bool 
	}	
	using 			unsigned_t 	= 	std::conditional_t	
									<	std::is_same_v<T, bool> || std::is_same_v<T, std::byte>
									,	std::type_identity<unsigned char> 	// 	Wrapper for bool (C++20)
									, 	std::make_unsigned<T>              	// 	Wrapper for another types
									>::type; 								// 	Type of only the selected branch
	constexpr 	
	std::size_t		bits_n		=	std::numeric_limits<unsigned_t>::digits;
		
	std::string		res_string{};											//	Result binary number string 
	
	res_string.reserve(bits_n + bits_n / 4 + 2);
	
	std::size_t		res_string_sz	= 	bits_n;
	
	res_string 						+= 	"0b";	
	
	unsigned_t 		value 			= 	static_cast<unsigned_t>(integr_param);
	
	for (std::size_t i = 0; i < bits_n; ++i)
    {
        std::size_t 		r_shift 	= 	bits_n - 1 - i;							//	Shift entire value to the right
		
        res_string 				+= 	((value >> r_shift) & 1u) ? '1' : '0';

        if ((i + 1) % 4 == 0 && i + 1 != bits_n)								//	It need,nt endin separator
            res_string += separator;
    }

    return res_string;
}
//	--------------------------------------- 	template <>		class set_group_and_separator
//	'D' - decimal, 'H' - hexdecimal, 'O' - octal; Digits Group Separator, default = apostrophe 
template	<char BaseTag = 'D', 	char Separator	= '\''>
	requires (BaseTag == 'D' || BaseTag == 'H' || BaseTag == 'O')
class GroupingAndSeparating
{
 private:	
	static constexpr int 		group_sz =  (BaseTag == 'H') ? 4 : 3;	//	4 or 3 means repeat grouping of 4 or 3 digits
	std::locale 				old_locale;
	std::ostream& 			out_stream;			

	//	According to usage of std::numpunct contract
	struct custom_numpunct : std::numpunct<char> 	
	{
		//	Define digit grouping separator symbol (e.g., apostrophe -  '\'')
		char				do_thousands_sep()	const override 	{ return Separator; }  
		
		//	Define digit grouping pattern (e.g., group_sz for repeat grouping)
		std::string 		do_grouping() const override  { return std::string(1, static_cast<char>(group_sz)); }	
	};
 public:			
	//	CTOR and DETOR
	explicit GroupingAndSeparating(std::ostream& 	os)
        : 	out_stream(os)
		,   old_locale(os.imbue(std::locale(os.getloc(), new custom_numpunct)))	//	save old locale and set new one
    {}

    ~GroupingAndSeparating()    { out_stream.imbue(old_locale); }	//	restore old locale
};
//	----------------------------------- void 	template <typename T> print_bin()
template <typename T>
void 	print_bin(T number_par, char Separator = '\''){
	
		cout << "Number: " << integr_to_bin_str_with_sep<T>(number_par)	<< endl; 
}
//	----------------------------------- void 	print_dec_oct_hex()	
template		<typename T,	char	 BaseTag, char Separator>
void 	print_dec_oct_hex(T number_par)
{
	GroupingAndSeparating<BaseTag, Separator> 	set_gs(std::cout);
	
	if 	constexpr (BaseTag == 'D'){
        cout << std::dec;
		
	}	
	else if constexpr (BaseTag == 'O')
        cout << std::oct;
    else
        cout << std::hex;

	cout << "Number: " << std::uppercase << std::showbase << +number_par << endl;	
}
//	--------------------------------------------	void 	print_with_separator()
template <typename T>
concept ArithmeticTypes 	= 	(std::is_arithmetic_v<std::remove_cvref_t<T>> 
						|| 	std::is_same_v<std::remove_cvref_t<T>, std::byte>); 

template <auto BaseTag>
concept BaseTagRestr 		= 	(BaseTag == 'D' || BaseTag == 'H' || BaseTag == 'O' || BaseTag == 'B');

template <ArithmeticTypes T = int, auto BaseTag = 'D', char Separator = '\''>
	requires BaseTagRestr<BaseTag>
void 	print_with_separator(T 	number_par)
{
	if constexpr (BaseTag == 'B') {						//	For binary literal and bool variable
		print_bin<T>					(number_par, Separator);
	} 	else 	{										//	For rest types variables and literals
		print_dec_oct_hex<T, BaseTag, Separator>	(number_par);
	}
}

//  	---------------------------------------------		main()
int		main()
{
	cout << "-------------- I'm " << __PRETTY_FUNCTION__ << endl;
	
	print_with_separator<int>(1'777'789);
	print_with_separator<int>(-1'777'789);
	
	print_with_separator<int, 'D', ','>(1'777'789);
	
	print_with_separator<int, 'O'>(01'777'751);
	
	print_with_separator<long long, 'H'>(0x1'ABC7'DEF7);

	print_with_separator<double>(1'777.789);
	
	print_with_separator<int, 'B'>(0b1101'1101);
	print_with_separator<short, 'B'>(0b1101'1101);
	
	const std::byte	byte_var 	= std::byte{0b1101'1101};
	print_with_separator<std::byte, 'B'>(byte_var);
	
	print_with_separator<unsigned char, 'D'>(static_cast<unsigned char>(byte_var));
	print_with_separator<char, 'D'>(static_cast<char>(byte_var));

	print_with_separator<unsigned char, 'B'>(static_cast<unsigned char>(byte_var));
	print_with_separator<char, 'B'>(static_cast<char>(byte_var));
	
	print_with_separator<bool, 'B'>(true);
	print_with_separator<bool, 'B'>(false);
	
	print_with_separator<int, 'D'>(221);
	print_with_separator<int, 'D'>(-35);
	
	print_with_separator<int, 'B'>(221);
	print_with_separator<int, 'B'>(-35);

	return 0;
}