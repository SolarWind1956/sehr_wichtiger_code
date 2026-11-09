//	Стандартная библиотека
#include "my_std_library.h"
using namespace std;
/*
Возможные варианты синтаксиса лямбда функций

[ capture ] ( params ) mutable exception attribute -> ret { body }
[ capture ] ( params ) -> ret { body }
[ capture ] ( params ) { body }
[ capture ] { body }

attribute       - обеспечивает спецификацию атрибута,
                        таких атрибутов в спецификации C++ определено всего два:
                        ([[noreturn]], [[carries_dependency]])
                         (Optional).

*/

/*
  *     ********************************************************************
  *     Основная функция
  *     ********************************************************************
  */
void attribute_of_lambda()
{
        cout << '\n' << "Работает <attribute_of_lambda()>  " << '\n' << endl;

}