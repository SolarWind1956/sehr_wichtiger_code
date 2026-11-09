//	Стандартная библиотека
#include "my_std_library.h"
using namespace std;
/*
Возможные варианты синтаксиса лямбда функций

[ capture ] ( params ) mutable exception attribute -> ret { body }
[ capture ] ( params ) -> ret { body }
[ capture ] ( params ) { body }
[ capture ] { body }

mutable         Спецификация - использование mutable позволяет модифицировать копии объектов,
                         которые были захвачены копированием. В обычном варианте они не будут модифицироваться.
                          (Optional).

*/
/*
  *     ********************************************************************
  *     Основная функция
  *     ********************************************************************
  */
void  mutable_lambda()
{
        cout << '\n' << "Работает <mutable_lambda()>  " << '\n' << endl;


}