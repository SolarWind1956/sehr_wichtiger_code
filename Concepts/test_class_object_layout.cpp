//	test_class_object_layout.cpp

#include <iostream>
using std::cout; using std::cerr; using std::endl; using std::boolalpha;

#include <iomanip>
using std::setw;

#include <concepts>

#include <string_view>
using std::string_view;

#include <type_traits>

#include "../../include/color_output.hpp"
using namespace	color_output;

template <typename T>
void 	print_object_layout	(	const char* 	name
							, 	const T& 	obj
							)
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	
    const unsigned char* 	base = reinterpret_cast<const unsigned char*>(&obj);

    cout << "\n====================================================\n";
    cout << "  Object layout for type : " << name 								<< "\n";
    cout << "  Sizeof(T) =            : " << sizeof(T) 						<< "\n";
    cout << "  Address of object =    : " << static_cast<const void*>(base) 	<< "\n";
    cout << "====================================================\n";

    if constexpr (std::is_class_v<T>) {

        cout << "\n-- Base classes (static_cast) --------------------------------\n";

        // Первый базовый класс
        if constexpr (requires { static_cast<const typename T::B1*>(std::declval<const T*>()); }) 
		{
            auto* 		b1 		= 	static_cast		<const typename T::B1*>	(&obj);
            ptrdiff_t 	off 		= 	reinterpret_cast	<const unsigned char*>(b1) - base;
            cout << "B1 subobject offset = " << off 
                 << "   addr = " << static_cast<const void*>(b1) << "\n";
        }

        if constexpr (requires { static_cast<const typename T::B2*>(std::declval<const T*>()); }) {
            auto* 		b2 		= 	static_cast		<const typename T::B2*>	(&obj);
            ptrdiff_t 	off 		= 	reinterpret_cast	<const unsigned char*>(b2) - base;
            cout << "B2 subobject offset = " << off
                 << "   addr = " << static_cast<const void*>(b2) << "\n";
        }

        if constexpr (requires { static_cast<const typename T::B3*>(std::declval<const T*>()); }) {
            auto* 		b3 		= 	static_cast		<const typename T::B3*>	(&obj);
            ptrdiff_t 	off 		= 	reinterpret_cast	<const unsigned char*>(b3) - base;
            cout << "B3 subobject offset = " << off
                 << "   addr = " << static_cast<const void*>(b3) << "\n";
        }

        cout << "\n-- Data members ----------------------------------------------\n";

        // 	Используем `лямбда-трюк` с `pointer-to-member`
        auto 	show = [&](auto ptr, string_view memname) {
            auto* 		p 		= 	&(obj.*ptr);
            ptrdiff_t 	off 		= 	reinterpret_cast	<const unsigned char*>(p) - base;
            cout << setw(20) << memname 
                 << " offset = " << off 
                 << "   addr = " << static_cast<const void*>(p) << "\n";
        };

        if constexpr (requires { &T::i; }) show(&T::i, "i");
        if constexpr (requires { &T::j; }) show(&T::j, "j");
        if constexpr (requires { &T::k; }) show(&T::k, "k");
        if constexpr (requires { &T::x; }) show(&T::x, "x");
        if constexpr (requires { &T::y; }) show(&T::y, "y");
        if constexpr (requires { &T::z; }) show(&T::z, "z");
    }

    cout << "====================================================\n";
}
void test_1_class_object_layout() {
	cout << bright_white << "-------------- I'm " << __PRETTY_FUNCTION__ << reset << endl;	
	struct B1 { int i; int j; };   		// 	Первый базовый класс (стандартная схема размещения)
	struct B2 { int k; int x; };       	// 	Второй базовый класс (стандартная схема размещения)
	struct B3 { int y; int z; };       	// 	Третий базовый класс (стандартная схема размещения)
	struct D : B1, B2, B3 {          	// 	Множественное наследование (стандартная схема размещения)
		
	};
	D	d;
	print_object_layout	(	"struct D : B1, B2, B3"
						, 	d
						);
}	

/*
	*************************************************************************************************************************
*/
int main()	{
	color_output::init();
	cout << cyan << "-------------- I'm " << __PRETTY_FUNCTION__ << " of Chapter_23.cpp" << reset << endl;
	
	test_1_class_object_layout();
	
	system("pause");
}		