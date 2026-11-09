#ifndef MY_POLYGON_H
#define MY_POLYGON_H
// Polygon.h
// Стандартная библиотека
// #include "my_std_library.h"
#include <chrono>	
#include "type_name_as_str.h"
// Функции установки локали
void set_concole_cp_1251();
// Функции вывода временных параметров из std::chrono
void heckel_print_duration(std::chrono::duration<double> duratio);
void heckel_print_time_point(std::chrono::time_point<std::chrono::system_clock> t_p);


// Пока разберусь с числом ПИ, возьму из Википедии эту дробь
constexpr double MY_PI { 103'993.0 / 33'102.0 };	
/*
Из 
David VANDEVOORDE + Nicolai M.JOSUTTIS + Douglas GREGOR" "C++ Templates.The Complete Guide."
Дэвид ВАНДЕВУРД, Николаи М.ДЖОСАТТИС, Дуглас ГРЕГОР "Шаблоны C++ Справочник разработчика."

Клиент, например:
        std::cout << "pi_dnd<double> == " << pi<double> << '\n';
        std::cout << "pi_dnd<float> == " << pi<float> << '\n';
*/
template<typename T>
constexpr T pi_dnd{ /*3.1415926535897932385*/ 103'993.0 / 33'102.0 };

/*
* 
//      **********************************************************************************************
// Шаблон, определяющий полный тип переменной
// Это я слизал у знающих людей вот с этого сайта:
// https://ru.stackoverflow.com/questions/722557/Определение-полного-типа-назначаемого-компилятором-через-auto/722564
// Клиентская часть: type_name_as_str<decltype(i)>()
// Описатель типа decltype Возвращает 'тип' указанного выражения. 
// а еще :                 type_name_as_str<decltype( (i) )>() - только я еще не вдуплил, почему... :(
// После Раздела 15.10.3. вдуплил: decltype( (i) ) - скобки означают 'ссылку' - а дальше нужно, видимо,
// учитывать правила свертки ссылок: & + & = &; && + & = &; & + && = &; && + && = &&.
// Если i - это тип, а не объект, то канает вызов: type_name_as_str<i>()
// Я и таким синтаксисом пользовался
// typeid(decltype(int_var_1932)).name(), но typeid() игнорирует ссылочность, так что первый вариант верный:
//      type_name_as_str<decltype(i)>()
// auto - игнорирует ссылочность
// нужно пользоваться decltype(auto)


template <typename T>
std::string type_name_as_str()
{
        //typedef typename std::remove_reference<T>::type TR;
        using TR = typename std::remove_reference_t<T>;                          // C++17
//    Как следствие, порядок удаления ссылочнОСТИ и const имеет значение:  
//      std::remove_const_t		< std::remove_reference_t<int const&> >		// int 
//      std::remove_reference_t	< std::remove_const_t<int const&> >			// int const

        std::unique_ptr<char, void(*)(void*)> own
        (
#ifndef _MSC_VER
                abi::__cxa_demangle(typeid(TR).name(), nullptr, nullptr, nullptr),
#else
                nullptr,
#endif
                std::free
        );

        std::string result = ( own != nullptr ) ? own.get() : typeid(TR).name();

        if ( std::is_const<TR>::value )
                result += " const";
        if ( std::is_volatile<TR>::value )
                result += " volatile";
        if ( std::is_lvalue_reference<T>::value )
                result += "&";
        else if ( std::is_rvalue_reference<T>::value )
                result += "&&";

        return result;
}
*/
//      **********************************************************************************************


#endif
