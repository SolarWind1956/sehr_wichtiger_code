//	Стандартная библиотека
#include "my_std_library.h"
using namespace std;
/*
Возможные варианты синтаксиса лямбда функций

[ capture ] ( params ) mutable exception attribute -> ret { body }
[ capture ] ( params ) -> ret { body }
[ capture ] ( params ) { body }
[ capture ] { body }

exception     (throw() - спецификация)  - обеспечивает спецификацию исключения,
                        то есть лямбда функции также как и обычные функции могут выкидывать исключения.
                         (Optional).

                         Тело лямбда-выражения выполняет правила как для структурированной обработки исключений
                         (SEH), так и для обработки исключений C++. 
                         Можно обработать возникшее исключение в теле лямбда-выражения 
                         или перенести обработку исключения во включающий фрагмент.

*/

/*
  *     ********************************************************************
  *     Основная функция
  *     ********************************************************************
  */
void  exception_in_lambda()
{
        cout << '\n' << "Работает <exception_in_lambda()>  " << '\n' << endl;

        // Create a vector that contains 3 elements.
        vector<int> elements(3);

        // Create another vector that contains index values.
        vector<int> indices(3);
        indices[0] = 0;
        indices[1] = -1; // This is not a valid subscript. It will trigger an exception.
        indices[2] = 2;

        // Use the values from the vector of index values to
        // fill the elements vector. This example uses a
        // try/catch block to handle invalid access to the
        // elements vector.
        try
        {
                for_each(indices.begin(), indices.end(), [&](int index) {
                        elements.at(index) = index;
                        });
        }
        catch (const out_of_range& e)
        {
                cerr << "Caught '" << e.what() << "'." << endl;
        };
}
/*
Примечания.
1. https://docs.microsoft.com/ru-ru/cpp/cpp/examples-of-lambda-expressions?view=vs-2019

*/