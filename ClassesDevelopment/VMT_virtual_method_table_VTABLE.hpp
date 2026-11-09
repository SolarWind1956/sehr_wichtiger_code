//	VMT_virtual_method_table_VTABLE.hpp

//		#pragma once -> SF.8: Use #include guards for all header files
#ifndef VMT_virtual_method_table_VTABLE_hpp_INCLUDE_GUARD
#define VMT_virtual_method_table_VTABLE_hpp_INCLUDE_GUARD

#include <iostream>
using std::cout; using std::endl; using std::boolalpha; using std::cin; using std::cerr;
#include <string>
#include <string_view>
#include <cassert>
#include <memory>
#include "..\..\type_name_as_str.h"
/*
	**********************************************************************************
	Dyzzet| C++ Data Science Алгоритмы Темы · Блог · YouTube
	Полиморфная кухня
	2. Таблицы виртуальных функций
	https://dyzzet.ru/a/vmt/
	----------------------------------------------------------------------------------
	
	Рассмотрим простую иерархию: 
	'розетка' и 'заземлённая розетка'.

                                          Socket                                
                                            ↑
                                      GroundedSocket
*/
namespace 	Electricity {
	
class Socket
{
public:
	// Массив символов может инициализироваться строкой. Размерность массива = (длина строки + завершающий 0).
	char content[32] = "ABCDEFGHAAAAAAAAAAAAAAAAAAAAAAA";
	          
	Socket()	{
		cout << "---COTOR--- I'm " <<  __func__  << "()" << endl;
	}	
    virtual ~Socket()    {
		cout << "---virtual DETOR--- I'm " <<  __func__  << "()" << endl;
	}
    // ...
    unsigned 		GetVoltage	() 	const    {
        return 	voltage_;
    }
 
    virtual bool 	IsGrounded	() 	const    {
        return false;
    }
	auto			get_vptr	()	const    {
		return this;
    }
protected:
	unsigned 		voltage_{220};
};
 
class GroundedSocket : public Socket
{
public:
	GroundedSocket(){
		cout << "---COTOR--- I'm " <<  __func__  << "()" << endl;
	}	
    // ...
    virtual bool 		IsGrounded() const    {
        return true;
    }
protected:
    const 	enum
    {
        TN,
        TN_С,
        TN_S,
        TN_C_S,
        IT,
        ТТ
    } 	groundingType{};
};
}	//	End of namespace Electricity
/*
	****************************************************************************************************
*/
#endif 	// End of VMT_virtual_method_table_VTABLE_hpp_INCLUDE_GUARD
